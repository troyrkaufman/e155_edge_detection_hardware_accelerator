module vga (
    input logic clk,
    nrst,
    output logic vgaClk,
    output logic hSync,
    vSync,
    output logic syncB,
    blankB,
    output logic [3:0] r,
    g,
    b
);
  logic [9:0] x, y;
  vgaPll pll (
      .ref_clk_i(clk),
      .rst_n_i(nrst),
      .outcore_o(),
      .outglobal_o(vgaClk)
  );

  vgaController vgaCont (
      vgaClk,
      nrst,
      hSync,
      vSync,
      blankB,
      syncB,
      x,
      y
  );

  videoGen videoGen (
      vgaClk,
      x,
      y,
      r,
      g,
      b
  );
endmodule
