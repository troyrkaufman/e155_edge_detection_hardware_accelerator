module spiReceive #(parameter messageBits = 8) (
  input logic spiClk, nRst,
  input logic sdi, cs,
  output logic [messageBits-1:0] writeData,
  output logic writeEnable
);
  if (messageBits <= 8) begin
    logic [2:0] bitCounter, nextBitCounter;
  end else begin
    logic [3:0] bitCounter, nextBitCounter;
  end
  always_ff @( negedge spiClk, negedge nRst ) begin : mainLogic
    if (~nRst | ~cs) begin
      bitCounter <= 0;
    end else begin
      writeData[bitCounter] <= sdi;
      bitCounter <= nextBitCounter;

      if (bitCounter == messageBits-1) begin
        writeEnable <= 1;
        bitCounter <= 0;
      end
    end
  end  

  assign nextBitCounter = bitCounter + 1;
endmodule