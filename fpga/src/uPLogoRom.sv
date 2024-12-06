module uPLogoRom (
    input logic [9:0] xVal,
    yVal,
    output logic outVal
);
  logic [0:18] logoRom[0:8];
  initial begin
    // Initialize logoRom with RGB values
    $readmemb("hex.txt", logoRom);
  end
  logic [18:0] temp;
  assign temp   = logoRom[yVal[2:0]];
  assign outVal = logoRom[yVal[2:0]][xVal[3:0]];
endmodule
