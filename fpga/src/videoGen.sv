module videoGen (
  input logic [9:0] x, y,
  output logic [7:0] r, g, b
);
  logic [2:0][7:0] pixel;
  uPLogoRom uPLogoRom(x, y, pixel);

  assign r = pixel[0];
  assign g = pixel[1];
  assign b = pixel[2];  
endmodule