`timescale 1 ns / 1 ns
module spiTestbench ();
  logic nRst, cs, clk, sdo, we;

  logic [7:0] writeData;

  spiReceive sampleSpi (
      .nRst(nRst),
      .cs(cs),
      .spiClk(clk),
      .sdi(sdo),
      .writeEnable(we),
      .writeData(writeData)
  );

  logic [7:0] sendData;

  always begin
    clk = 1;
    #5;
    clk = 0;
    #5;
  end

  initial begin
    nRst = 0;
    #27;
    nRst = 1;
  end

  int counter, nextCounter;

  always_ff @(posedge clk) begin
    if (we) begin
      $display("Received data: %b", writeData);
      $finish;
    end else if (~nRst) begin
      $display("Reset");
      cs <= 0;
      sendData <= 'b10011101;
      counter <= 8;
    end else begin
      cs <= 1;
      counter <= nextCounter;
    end
  end

  assign sdo = counter <= 7 ? sendData[counter] : 0;

  assign nextCounter = counter - 1;
endmodule
