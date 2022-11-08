#include "Vtop.h" 
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp" //this contains all vbuddy functions

int main(int argc, char **argv, char **env) {
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    Vtop* top = new Vtop; 
    Verilated::traceEverOn(true); 
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("top.vcd");

    // init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: top");

    top->clk = 1;
    top->rst = 1;
    top->en = 0;
    
    for (i=0; i<1000; i++) { 

        for (clk=0; clk<2; clk++) {
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval (); 
        }

        // ++++ Send count value to Vbuddy
        vbdHex(4, int(top->bcd) >> 12 & 0xF);
        vbdHex(3, int(top->bcd) >> 8 & 0xF);
        vbdHex(2, int(top->bcd) >> 4 & 0xF);
        vbdHex(1, int(top->bcd) & 0xF);
        vbdCycle(i+1);
        // ---- end of Vbuddy output section

        // input stimuli
        top->rst = (i<2);
        top->en = vbdFlag(); 
        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose(); // necessary step
    tfp->close();
    exit(0);
}

