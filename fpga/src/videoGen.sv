module videoGen (
    input logic clk,
    input logic [9:0] x,
    y,
    output logic [3:0] r,
    g,
    b
);
  // logic [2:0][3:0] pixel;
  logic pixelPresent;

  uPLogoRom logoRom (
      .clk(clk),
      .xVal(x),
      .yVal(y),
      .pixelPresent(pixelPresent)
  );

  always_comb begin
    r = pixelPresent ? 4'b1111 & {4{y[1]}} : 'b0;
    g = pixelPresent ? 4'b1111 & {4{~y[1]}} : 'b0;
    b = pixelPresent ? 4'b1111 & {4{y[2]}} : 'b0;
  end
endmodule
