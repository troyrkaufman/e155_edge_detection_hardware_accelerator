module uPLogoRom (
    input logic clk,
    input logic [9:0] xVal,
    yVal,
    output logic pixelPresent
);
  logic [4:0] logoRom[2:0];  // 640x480 to store RGB values

  // Initialize logoRom with RGB values
  initial $readmemh("hex.txt", logoRom);

  always_ff @(posedge clk) begin
    pixelPresent <= logoRom[yVal[2:0]][xVal[2:0]];
  end
endmodule
