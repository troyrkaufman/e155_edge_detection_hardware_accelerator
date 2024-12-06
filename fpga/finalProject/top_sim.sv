
module top_sim (
    input logic spiClk,
    sdi,
    ncs,
    input logic nreset,
    output logic hSync,
    vSync,
    syncB,
    blankB,
    output logic [1:0] red,
    green,
    blue
);
  logic oscClk, vgaClk;
  logic [1:0] outVal, edgeVal;
  logic pixelDataValid, edgeValid;

  logic [18:0] addressRead, addressWrite, addressSpi;

  logic [3:0] pixelData[3][3];

  HSOSC #(
      .CLKHF_DIV("0b00")
  ) hsosc (
      .CLKHFEN(1'b1),
      .CLKHFPU(1'b1),
      .CLKHF  (oscClk)
  );

  spramController spramCont (
      .mainClk(oscClk),
      .nreset(nreset),
      .addressRead(addressRead),
      .addressWrite(addressWrite),
      .writeData(edgeVal),
      .load(edgeValid),
      .vgaClk(vgaClk),
      .outVal(outVal)
  );

  spiController spiCont (
      .mainClk(oscClk),
      .spiClk(spiClk),
      .nreset(nreset),
      .sdi(sdi),
      .ncs(ncs),
      .pixelData(pixelData),
      .pixelDataValid(pixelDataValid),
      .spiXVal(addressSpi[9:0]),
      .spiYVal(addressSpi[18:10])
  );

  vgaController vgaCont (
      .vgaClk(vgaClk),
      .nreset(nreset),
      .hSync (hSync),
      .vSync (vSync),
      .syncB (syncB),
      .blankB(blankB),
      .hCount(addressRead[9:0]),
      .vCount(addressRead[18:10])
  );

  edgeDetect edgeDetectInst (
      .clk(oscClk),
      .nreset(nreset),
      .inputValid(pixelDataValid),
      .pixelData(pixelData),
      .spiXVal(addressSpi[9:0]),
      .spiYVal(addressSpi[18:10]),
      .edgeVal(edgeVal),
      .edgeValValid(edgeValid),
      .edgeXVal(addressWrite[9:0]),
      .edgeYVal(addressWrite[18:10])
  );

  assign red   = syncB ? 2'b00 : outVal;
  assign green = syncB ? 2'b00 : outVal;
  assign blue  = syncB ? 2'b00 : outVal;


endmodule