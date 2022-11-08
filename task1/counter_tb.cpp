#include "Vcounter.h" //this is made by verilator, as the executable file from Vcounter.sv
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter; //this is the DUT itself (pointer to it). 
    // init trace dump
    Verilated::traceEverOn(true); //I think this turns on signal tracing - boolean true
    VerilatedVcdC* tfp = new VerilatedVcdC; //tfp is a pointer that points to variable type VerilatedVcdC
    //VerilatedVcdC object is the one that directly related to vcd file for waveform
    top->trace (tfp, 99); //trace is a variable pointed to by top, inside Vcounter class, with tfp, 99 being two of the values in it
    tfp->open ("counter.vcd"); //dumps waveform into vcd file-line28

    //initialise simulation inputs
    top->clk = 1; //starts on 1 because it changes to 0 line 32
    top->rst = 1;
    top->en = 0;
    int cycles = 0;
    //so the DUT begins with these values, note en=0. 

    // run simulation for many clock cycles
    for (i=0; i<300; i++) { //for each i, the clock 0,1.

        // dump variables into VCD file and toggle clock
        //vcd is "value change dump"
        for (clk=0; clk<2; clk++) { //clk goes 0,1,0,1...
            tfp->dump (2*i+clk);//what is this? it goes 0,1,2,3,4 - Every unit is a clk change, so each cycle is 2 units - in ps
            top->clk = !top->clk;
            top->eval (); //what does this do? Probably runs the actual systemverilog
        }
        top->rst = (i<2) || (i==20); //means rst=1 when i<2
        top->en = (i>4); //en = 1 on this condition

        if (int(top->count)==9 && cycles < 3){
            cycles++;
            top->en = 0;
        }

        if (cycles == 3){
            cycles = 0;
            top->en = 1;
        }


        //is there a way of getting the count value and comparing it to 9?

        if (Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}

