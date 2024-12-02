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

  

endmodule
