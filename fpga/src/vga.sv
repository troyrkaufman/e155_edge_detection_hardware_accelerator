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
  //   logic idk;
  vgaPll pll (
      .ref_clk_i(clk),
      .rst_n_i(nrst),
      .outcore_o(vgaClk),
      .outglobal_o()
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

  always_comb begin
    r = x[3:0];
    g = x[7:4];
    b = {x[9:8], 2'b00};
  end
endmodule
