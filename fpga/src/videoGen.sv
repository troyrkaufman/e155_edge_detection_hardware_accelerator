module videoGen (
    input logic clk,
    input logic [9:0] x,
    y,
    output logic [3:0] r,
    g,
    b
);
  // logic [2:0][3:0] pixel;

  always_comb begin
    r = x[3:0];
    g = x[7:4];
    b = {x[9:8], 'b00};
  end
endmodule
