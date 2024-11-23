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
    rst,
    output logic hSync,
    vSync,
    syncB,
    blankB,
    output logic [9:0] hCount,
    vCount
);

  always @(posedge vgaClk, posedge rst) begin
    if (rst) begin
      hCount <= 0;
      vCount <= 0;
    end else begin
      hCount++;
      if (hCount == HMAX) begin
        hCount <= 0;
        vCount++;
        if (vCount == VMAX) begin
          vCount <= 0;
        end
      end
    end

    assign hSync = (hCount >= HSYNC) && (hCount < HSYNC + HBP);
    assign vSync = (vCount >= VSYNC) && (vCount < VSYNC + VBP);
    assign syncB = hSync && vSync;
    assign blankB = hSync || vSync;
  end

endmodule
