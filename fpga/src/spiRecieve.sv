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
  logic [3:0] bitCounter;

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


  always_ff @(negedge spiClk) begin : bitCounterLogic
    if (currentState == RECEIVING) bitCounter <= bitCounter - 1;
    else bitCounter <= messageBits - 1;
  end

  logic temp;

  always_ff @(negedge spiClk) begin : writeDataLogic
    if (currentState == RECEIVING) begin
      temp <= sdi;
      writeDataBuffer[bitCounter] <= sdi;
    end else begin
      writeDataBuffer <= 0;
      temp <= 0;
    end
  end

  assign writeEnable = (currentState == DONE);
  assign writeData = writeDataBuffer[15:4];
endmodule
