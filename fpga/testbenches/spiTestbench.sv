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

  logic [23:0] sendData;
  always_ff @(posedge clk) begin
    if (we) begin
      $display("Received data: %b", writeData);
    end else if (~nRst) begin
      $display("Reset");
      cs <= 0;
      sendData <= 'b100111011111111100000000;
      counter <= 24;
    end else if (counter == 0)
      $finish;
    else begin
      cs <= 1;
      counter <= counter - 1;
    end
  end

  assign sdo = counter < 24 ? sendData[counter] : 0;
endmodule
