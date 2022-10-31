module counter #(
    parameter WIDTH = 8
)(
    // interface signals
    input   logic               clk,    // clock
    input   logic               rst,    // reset
    input   logic               en,     // counter enable
    output  logic [WIDTH-1:0]   count   // count output 
);

always_ff @ (posedge clk) //this specifies a clocked circuit
if(rst) count <= {WIDTH{1'b0}}; //Uses concatenation operator
else    count <= count + {{WIDTH-1{1'b0}}, en}; 

endmodule

// note: filename and module name must be the same

