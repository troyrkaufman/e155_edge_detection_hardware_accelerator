module edgeDetect (
  // input logic clk, nreset,
  input logic inputValid,
  input logic [3:0] pixelData [3][3],
  output logic [1:0] edgeVal,
  output logic edgeValValid
);

  logic [4:0] row1Val, row3Val; // max val is 15+30+15 = 60
  logic [4:0] col1Val, col3Val; // max val is 15+30+15 = 60

  logic [4:0] rowDiff, colDiff; // max val is 60 - 0 = 60

  logic [11:0] rowSquare, colSquare; // max val is 60^2 = 3600 = 12'b111000010000

  logic [12:0] squareSum; // max val is 3600 + 3600 = 7200 = 13'b111000100000

  logic [6:0] sqrtVal; // max val is sqrt(7200) = 85 = 7'b1010101

  sqrtRom sqrtRom (
    .squareSum(squareSum),
    .sqrtVal(sqrtVal)
  );

  bitCompress bitCompress (
    .inVal(sqrtVal),
    .outVal(edgeVal)
  );

  always_comb begin : rowColCalcs
    row1Val = pixelData[0][0] + pixelData[0][1] << 2 + pixelData[0][2];
    row3Val = pixelData[2][0] + pixelData[2][1] << 2 + pixelData[2][2];
    col1Val = pixelData[0][0] + pixelData[1][0] << 2 + pixelData[2][0];
    col3Val = pixelData[0][2] + pixelData[1][2] << 2 + pixelData[2][2];

    rowDiff = row1Val > row3Val ? row1Val - row3Val : row3Val - row1Val;
    colDiff = col1Val > col3Val ? col1Val - col3Val : col3Val - col1Val;

    rowSquare = rowDiff * rowDiff; // change this to use the IP
    colSquare = colDiff * colDiff;

    squareSum = rowSquare + colSquare;
  end

  assign edgeValValid = inputValid;
endmodule
