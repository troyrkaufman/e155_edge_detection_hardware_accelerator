module edgeDetect (
    input logic clk,
    nreset,
    input logic inputValid,
    input logic [3:0] pixelData[3][3],
    output logic [1:0] edgeVal,
    output logic edgeValValid
);

  typedef enum logic [2:0] {
    WAIT,
    ADD,
    SUB,
    SQUARE,
    SQRT
  } pipelineState_t;
  pipelineState_t pipelineState, nextState;

  logic [3:0] pixelDataReg[3][3];
  logic [4:0] row1ValReg, row3ValReg, nextRow1Val, nextRow3Val;  // max val is 15+30+15 = 60
  logic [4:0] col1ValReg, col3ValReg, nextCol1Val, nextCol3Val;  // max val is 15+30+15 = 60

  logic [4:0] rowDiffReg, colDiffReg, nextRowDiff, nextColDiff;  // max val is 60 - 0 = 60

  logic [11:0] rowSquare, colSquare;  // max val is 60^2 = 3600 = 12'b111000010000

  logic [12:0] squareSumReg, nextSquareSum;  // max val is 3600 + 3600 = 7200 = 13'b111000100000

  logic [6:0] sqrtValReg, nextSqrtVal;  // max val is sqrt(7200) = 85 = 7'b1010101
  logic [1:0] nextEdgeVal;

  sqrtRom sqrtRom (
      .squareSum(squareSumReg),
      .sqrtVal  (nextSqrtVal)
  );

  bitCompress bitCompress (
      .inVal (sqrtValReg),
      .outVal(nextEdgeVal)
  );

  always_ff @(posedge clk, negedge nreset) begin : stateLogic
    if (~nreset) begin
      pipelineState <= WAIT;
    end else begin
      pipelineState <= nextState;
    end
  end

  always_comb begin : nextStateLogic
    case (pipelineState)
      WAIT: nextState = inputValid ? ADD : WAIT;
      ADD: nextState = SUB;
      SUB: nextState = SQUARE;
      SQUARE: nextState = SQRT;
      SQRT: nextState = WAIT;
      default: nextState = WAIT;
    endcase
  end

  always_ff @(posedge clk, negedge nreset) begin : regLogic
    if (~nreset) begin
      pixelDataReg <= '{default: 0};
      row1ValReg <= 0;
      row3ValReg <= 0;
      col1ValReg <= 0;
      col3ValReg <= 0;
      rowDiffReg <= 0;
      colDiffReg <= 0;
      squareSumReg <= 0;
      sqrtValReg <= 0;
      edgeVal <= 0;
    end else begin
      pixelDataReg <= pixelData;
      row1ValReg <= nextRow1Val;
      row3ValReg <= nextRow3Val;
      col1ValReg <= nextCol1Val;
      col3ValReg <= nextCol3Val;
      rowDiffReg <= nextRowDiff;
      colDiffReg <= nextColDiff;
      squareSumReg <= nextSquareSum;
      sqrtValReg <= nextSqrtVal;
      edgeVal <= nextEdgeVal;
    end
  end

  always_comb begin : rowColCalcs
    nextRow1Val = pixelDataReg[0][0] + (pixelDataReg[0][1] << 1) + pixelDataReg[0][2];
    nextRow3Val = pixelDataReg[2][0] + (pixelDataReg[2][1] << 1) + pixelDataReg[2][2];
    nextCol1Val = pixelDataReg[0][0] + (pixelDataReg[1][0] << 1) + pixelDataReg[2][0];
    nextCol3Val = pixelDataReg[0][2] + (pixelDataReg[1][2] << 1) + pixelDataReg[2][2];

    nextRowDiff = row1ValReg > row3ValReg ? row1ValReg - row3ValReg : row3ValReg - row1ValReg;
    nextColDiff = col1ValReg > col3ValReg ? col1ValReg - col3ValReg : col3ValReg - col1ValReg;

    rowSquare = rowDiffReg * rowDiffReg;  // change this to use the IP
    colSquare = colDiffReg * colDiffReg;

    nextSquareSum = rowSquare + colSquare;
  end

  assign edgeValValid = pipelineState == SQRT;
endmodule
