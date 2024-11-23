module spiTestbench ();
  logic nrst, cs, clk, sdo, we;

  logic [7:0] writeData;

  spiReceive (.nrst(nrst), .cs(cs), .clk(clk), .sdi(sdo), .we(we), .writeData(writeData));

  logic [7:0] sendData;

  always begin
    clk = 1;
    #5;
    clk = 0;
    #5;
  end

  initial begin
    nrst = 0;
    #27;
    nrst = 1;
    sendData = 'b10011100;
  end

  always_ff @( posedge clk ) begin
    sdo <= sendData[0];
    sendData <= sendData >> 1;

    if (sendData == 'b0) begin
      $finish;
    end
  end
endmodule