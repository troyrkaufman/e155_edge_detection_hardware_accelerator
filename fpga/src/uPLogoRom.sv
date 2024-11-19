module uPLogoRom (
  input logic [9:0] xVal, yVal,
  output logic [2:0][7:0] pixel
);
  logic [2:0][7:0] logoRom [0:307200] // 640x480 to store RGB values
  initial begin
    // Initialize logoRom with RGB values
    $readmemh("hex.txt", logoRom);
  end

  assign pixel = logoRom[xVal + yVal * 640];
endmodule
