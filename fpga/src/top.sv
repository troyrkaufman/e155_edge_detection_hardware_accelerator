module top (
    input logic nrst,
    output logic hSync,
    vSync,
    syncB,
    blankB,
    output logic [3:0] r,
    g,
    b
);
  logic clk;
  HSOSC #(
      .CLKHF_DIV(2'b00)
  )
      hf_osc (
      .CLKHFPU(1'b1),
      .CLKHFEN(1'b1),
      .CLKHF  (clk)
  );

  logic vgaClk;

  vga vgaMod (
      .clk(clk),
      .nrst(nrst),
      .vgaClk(vgaClk),
      .hSync(hSync),
      .vSync(vSync),
      .syncB(syncB),
      .blankB(blankB),
      .r(r),
      .g(g),
      .b(b)
  );
endmodule
