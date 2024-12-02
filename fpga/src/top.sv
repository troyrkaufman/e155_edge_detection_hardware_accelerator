module top (
    input logic spiClk,
    sdi,
    ce,
    input logic nreset,
    output logic hSync,
    vSync,
    syncB,
    blankB,
    output logic [1:0] red,
    green,
    blue
);
  logic oscClk, mainClk, vgaClk;

  HSOSC #(
      .CLKHF_DIV("0b10")
  ) hsosc (
      .CLKHFEN(1'b1),
      .CLKHFPU(1'b1),
      .CLKHF  (oscClk)
  );


  pll50p35 vgaPll (
      .ref_clk_i(oscClk),
      .rst_n_i(nreset),
      .outcore_o(mainClk),
      .outglobal_o()
  );

  spramController spramCont (
      .mainClk(mainClk),
      .nreset(nreset),
      .addressRead(spiData),
      .addressWrite(spiData),
      .writeData(spiData[11:10]),
      .load(spiDone),
      .vgaClk(vgaClk),
      .outVal(outVal)
  );

  spiReceive #(12) spiRec (
      .spiClk(spiClk),
      .nreset(nreset),
      .sdi(sdi),
      .cs(ce),
      .writeData(spiData),
      .writeEnable(spiDone)
  );

  logic [1:0] outVal;
  logic [11:0] spiData;
  logic spiDone;

  assign red   = outVal;
  assign green = outVal;
  assign blue  = outVal;


endmodule
