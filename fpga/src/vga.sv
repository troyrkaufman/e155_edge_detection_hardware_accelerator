module vga (
  input logic clk, rst,
  output logic vgaClk,
  output logic hSync, vSync,
  output logic syncB, blankB,
  output logic [7:0] r, g, b
);
  logic [9:0] x, y;

  pll vgaPll(.inclk0(clk), .c0(vgaClk));

  vgaController vgaCont(vgaClk, rst, hSync, vSync, syncB, blankB, x, y);

  videoGen videoGen(x,y,r,g,b);
endmodule
