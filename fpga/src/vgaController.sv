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
    nreset,
    output logic hSync,
    vSync,
    syncB,
    blankB,
    output logic [9:0] hCount,
    output logic [8:0] vCount
);

  logic [9:0] hCountInternal;
  logic [8:0] vCountInternal;

  assign hCount = hCountInternal >= 320 ? hCountInternal - 320 : hCountInternal;
  assign vCount = vCountInternal >= 240 ? vCountInternal - 240 : vCountInternal;

  always @(posedge vgaClk, negedge nreset) begin
    if (~nreset) begin
      hCountInternal <= 0;
      vCountInternal <= 0;
    end else begin
      hCountInternal <= hCountInternal + 1;
      if (hCountInternal == HMAX) begin
        hCountInternal <= 0;
        vCountInternal <= vCountInternal + 1;
        if (vCountInternal == VMAX) begin
          vCountInternal <= 0;
        end
      end
    end
  end

  assign hSync  = ~((hCountInternal >= (HACTIVE + HFP)) & (hCountInternal < HACTIVE + HFP + HSYNC));
  // not hCountInternal >= 656 aand hCountInternal < 752
  // hCountInternal < 656 or hCountInternal >= 752
  assign vSync  = ~((vCountInternal >= (VACTIVE + VFP)) & (vCountInternal < VACTIVE + VFP + VSYNC));
  // not vCountInternal >= 491 and vCountInternal < 493
  // vCountInternal < 491 or vCountInternal >= 493
  assign syncB  = hCountInternal < HACTIVE & vCountInternal < VACTIVE;
  assign blankB = hSync & vSync;

endmodule
