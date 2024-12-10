module spiReceive #(
    parameter messageBits = 8
) (
    input logic spiClk,
    nreset,
    input logic sdi,
    ncs,
    output logic [11:0] writeData,
    output logic writeEnable
);
  logic [ 3:0] bitCounter;
  logic [15:0] writeDataBuffer;
  typedef enum logic [1:0] {
    WAITING,
    RECEIVING,
    DONE
  } spiState_t;

  spiState_t currentState, nextState;
  always_ff @(negedge spiClk, negedge nreset) begin : stateLogic
    if (~nreset) begin
      currentState <= WAITING;
    end else begin
      currentState <= nextState;
    end
  end

  always_comb begin : nextStateLogic
    case (currentState)
      WAITING: nextState = ~ncs ? RECEIVING : WAITING;
      RECEIVING: nextState = (bitCounter == 0) ? DONE : RECEIVING;
      DONE: nextState = ~ncs ? RECEIVING : WAITING;
      default: nextState = WAITING;
    endcase
  end


  always_ff @(negedge spiClk, negedge nreset) begin : bitCounterLogic
    if (~nreset) bitCounter <= messageBits - 1;
    else if (nextState == RECEIVING) bitCounter <= bitCounter - 1;
    else bitCounter <= messageBits - 1;
  end

  logic temp;

  always_ff @(negedge spiClk, negedge nreset) begin : writeDataLogic
    if (~nreset) writeDataBuffer <= 'b0;
    else if (nextState == RECEIVING) begin
      temp <= sdi;
      writeDataBuffer[bitCounter] <= sdi;
    end
  end

  assign writeEnable = (nextState == DONE);
  assign writeData   = writeDataBuffer[15:4];
endmodule
