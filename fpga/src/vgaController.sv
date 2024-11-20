module vgaController #(
    parameter HBP = 10'd48,
    HACTIVE = 10'D640,
    HFP = 10'D16,
    HSYNC = 10'D96,
    HMAX = HBP + HACTIVE + HFP + HSYNC,
    VBP = 10'D32,
    VACTIVE = 10'D480,
    VFP = 10'D11,
    VSYNC = 10'D2,
    VMAX = VBP + VACTIVE + VSYNC
) (
    input logic vgaClk,
    nrst,
    output logic hSync,
    vSync,
    syncB,
    blankB,
    output logic [9:0] hCount,
    vCount
);

  always_ff @(posedge vgaClk, negedge nrst) begin
    if (~nrst) begin
      hCount <= 0;
      vCount <= 0;
    end else begin
      hCount <= hCount + 1;
      if (hCount == HMAX) begin
        hCount <= 0;
        vCount <= vCount + 1;
        if (vCount == VMAX) begin
          vCount <= 0;
        end
      end
    end
  end

  assign hSync  = ~( (hCount >= (HACTIVE + HFP)) & (hCount < (HACTIVE + HFP + HSYNC)) );
  assign vSync  = ~( (vCount >= (VACTIVE + VFP)) & (vCount < (VACTIVE + VFP + VSYNC)) );
  assign syncB  = 1'b0;
  assign blankB = hSync & vSync;

endmodule
