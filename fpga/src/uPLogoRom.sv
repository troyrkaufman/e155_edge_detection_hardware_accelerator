module uPLogoRom (
    input logic clk,
    input logic [9:0] xVal,
    yVal,
    output logic pixelPresent
);
  logic [0:18] logoRom[8:0];  // 640x480 to store RGB values

  // Initialize logoRom with RGB values
  initial $readmemb("hex.txt", logoRom);

  always_ff @(posedge clk) begin
    pixelPresent <= logoRom[yVal[2:0]][xVal[4:0]];
  end
endmodule
