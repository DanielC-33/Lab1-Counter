#include "Vcounter.h" 
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp" //this contains all vbuddy functions

int main(int argc, char **argv, char **env) {
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    Vcounter* top = new Vcounter; 
    Verilated::traceEverOn(true); 
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    // init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: Counter");

    top->clk = 1;
    top->rst = 1;
    top->ld = vbdFlag();
    // top->v = vbdValue(); //Commented for step 2, which doesn't use v

    vbdSetMode(1);
    
    for (i=0; i<1000; i++) { 

        for (clk=0; clk<2; clk++) {
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval (); 
        }

        // ++++ Send count value to Vbuddy
        // vbdPlot(int(top->count), 0, 255); //can use vbdPlot or vbdHex here (changing respective arguments)
        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);
        vbdCycle(i+1);
        // ---- end of Vbuddy output section

        // input stimuli
        top->rst = (i<2);
        // top->v = vbdValue();
        top->ld = vbdFlag(); 
        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose(); // necessary step
    tfp->close();
    exit(0);
}
