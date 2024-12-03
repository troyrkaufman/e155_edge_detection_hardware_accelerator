module sqrtRom (
  input logic [12:0] squareSum,
  output logic [6:0] sqrtVal
);

  logic [6:0] sqrtRom [0:8191]; // 2^13 = 8192
  initial begin
    // Initialize sqrtRom with sqrt values
    $readmemb("sqrts.txt", sqrtRom);
  end

  assign sqrtVal = sqrtRom[squareSum];
  
endmodule