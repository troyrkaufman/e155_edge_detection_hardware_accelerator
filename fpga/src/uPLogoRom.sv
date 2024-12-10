module uPLogoRom (
    input logic clk,
    input logic [9:0] xVal,
    yVal,
    output logic pixelPresent
);
  logic [2:0][7:0] logoRom [0:307200]; // 640x480 to store RGB values
  initial begin
    // Initialize logoRom with RGB values
    $readmemh("hex.txt", logoRom);
  end

  // Initialize logoRom with RGB values
  initial $readmemb("hex.txt", logoRom);

  always_ff @(posedge clk) begin
    pixelPresent <= logoRom[yVal[2:0]][xVal[4:0]];
  end
endmodule
