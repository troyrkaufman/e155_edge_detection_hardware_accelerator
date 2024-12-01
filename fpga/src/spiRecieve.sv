module spiReceive #(
    parameter messageBits = 8
) (
    input logic spiClk,
    nRst,
    input logic sdi,
    cs,
    output logic [messageBits-1:0] writeData,
    output logic writeEnable
);
  logic [3:0] bitCounter;

  typedef enum logic [1:0] {
    WAITING,
    RECEIVING,
    DONE
  } spiState_t;

  spiState_t currentState;

  always_ff @(posedge spiClk, posedge cs, negedge nRst) begin : stateLogic
    if (~nRst) currentState <= WAITING;
    else
      case (currentState)
        WAITING: currentState <= cs ? RECEIVING : WAITING;
        RECEIVING: currentState <= bitCounter == 'hF ? DONE : RECEIVING;
        DONE: currentState <= cs ? RECEIVING : WAITING;
        default: currentState <= WAITING;
      endcase
  end

  always_ff @(negedge spiClk) begin : bitCounterLogic
    if (currentState == RECEIVING) bitCounter <= bitCounter - 1;
    else bitCounter <= messageBits - 1;
  end

  logic temp;

  always_ff @(negedge spiClk) begin : writeDataLogic
    if (currentState == RECEIVING) begin
      temp <= sdi;
      writeData[bitCounter] <= sdi;
    end else begin
      writeData <= 0;
      temp <= 0;
    end
  end

  assign writeEnable = (currentState == DONE);
endmodule
