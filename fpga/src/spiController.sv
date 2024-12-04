module spiController (
    input logic mainClk,
    input logic spiClk,
    input logic nreset,
    input logic sdi,
    input logic ncs,
    output logic [3:0] pixelData[3][3],
    output logic pixelDataValid,
    output logic [9:0] spiXVal,
    output logic [8:0] spiYVal
);

  logic [3:0] pixelDataReg[3][3];

  logic [11:0] spiLine;

  logic [1:0] modCounter;

  logic spiDone;

  spiReceive #(
      .messageBits(12)
  ) spiReceiveInst (
      .spiClk(spiClk),
      .nreset(nreset),
      .sdi(sdi),
      .ncs(ncs),
      .writeData(spiLine),
      .writeEnable(spiDone)
  );

  typedef enum logic [1:0] { RECEIVING, DATA_DONE, WAIT_RESET } spiState_t;

  spiState_t spiState, nextSpiState;

  always_ff @(posedge mainClk, negedge nreset) begin : spiStateLogic
    if (~nreset) spiState <= RECEIVING;
    else spiState <= nextSpiState;
  end

  always_comb begin : spiStateNextLogic
    case (spiState)
      RECEIVING: nextSpiState = spiDone ? DATA_DONE : RECEIVING;
      DATA_DONE: nextSpiState = WAIT_RESET;
      WAIT_RESET: nextSpiState = spiDone ? WAIT_RESET : RECEIVING; // this is to account for the SPI clock being much slower
      default: nextSpiState = RECEIVING;
    endcase
  end

  assign pixelDataValid = spiState == DATA_DONE;

  always_ff @(posedge mainClk, negedge nreset) begin : coordLogic
    if (~nreset) begin
      spiXVal <= 0;
      spiYVal <= 0;
    end else if (spiState == DATA_DONE) begin
      spiXVal <= spiXVal + 1;
      if (spiXVal == 640) begin
        spiXVal <= 0;
        spiYVal <= spiYVal + 1;
        if (spiYVal == 480) begin
          spiXVal <= 0;
          spiYVal <= 0;
        end
      end
    end
  end

  always_ff @(posedge mainClk, negedge nreset) begin : dataLogic
    if (~nreset) begin
      pixelDataReg <= '{default: 0};
      modCounter <= 0;
    end else if (spiState == DATA_DONE) begin
      pixelDataReg[0][modCounter] <= spiLine[11:8];
      pixelDataReg[1][modCounter] <= spiLine[7:4];
      pixelDataReg[2][modCounter] <= spiLine[3:0];
      modCounter <= modCounter + 1;

      if (modCounter == 3) begin
        modCounter <= 0;
      end
    end
  end

  always_comb begin : pixelDataLogic
    case (modCounter)
      0: begin
        pixelData[0] = {pixelDataReg[0][1:2], pixelDataReg[0][0]};
        pixelData[1] = {pixelDataReg[1][1:2], pixelDataReg[1][0]};
        pixelData[2] = {pixelDataReg[2][1:2], pixelDataReg[2][0]};
      end
      1: begin
        pixelData[0] = {pixelDataReg[0][2], pixelDataReg[0][0:1]};
        pixelData[1] = {pixelDataReg[1][2], pixelDataReg[1][0:1]};
        pixelData[2] = {pixelDataReg[2][2], pixelDataReg[2][0:1]};
      end
      2: pixelData = pixelDataReg;
      default: pixelData = '{default: 0};
    endcase
  end
endmodule
