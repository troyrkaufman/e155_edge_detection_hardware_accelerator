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
    syncV,
    blankB,
    output logic [9:0] hCount,
    vCount
);

  always @(posedge vgaClk, posedge rst) begin
    if (rst) begin
      hCnt <= 0;
      vCnt <= 0;
    end
    else begin
      hCnt++;
      if (hCnt == HMAX) begin
        
      end
    end
  end

endmodule
