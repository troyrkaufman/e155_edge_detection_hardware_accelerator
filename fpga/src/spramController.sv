module spramController (
    input logic mainClk,
    input logic nreset,
    input logic [18:0] addressRead,
    addressWrite,
    input logic [1:0] writeData,
    input logic load,
    output logic vgaClk,
    output logic [1:0] outVal
);

  //* Clock Dividing

  //* Control Logic
  typedef enum logic [1:0] {
    READ,
    WAIT,
    LOAD,
    WRITE
  } controlState_t;

  controlState_t controlState, nextState;

  always_ff @(posedge mainClk, negedge nreset) begin : controlLogic
    if (~nreset) controlState <= READ;
    else controlState <= nextState;
  end

  always_comb begin
    case (controlState)
      READ: nextState = WAIT;
      WAIT: nextState = LOAD;
      LOAD: nextState = WRITE;
      WRITE: nextState = WAIT;
      default: nextState = WAIT;
    endcase
  end

  assign vgaClk = controlState == READ || controlState == WAIT;

  //* Data Logic
  logic [ 2:0] writeDataBuffer;
  logic [15:0] writeDataLine;
  logic [18:0] address, addressBuffer;
  logic [15:0] readData0, readData1, readData2, readData3, readData;

  //* SPARM Instantiation
  SP256K spram0 (
      .AD(address[16:3]),
      .DI(writeDataLine),
      .MASKWE(4'b1111),
      .WE(controlState == WRITE && writeDataBuffer[0]),
      .CS(address[18:17] == 2'b00),
      .CK(mainClk),
      .STDBY(1'b0),
      .SLEEP(1'b0),
      .PWROFF_N(1'b1),
      .DO(readData0)
  );

  SP256K spram1 (
      .AD(address[16:3]),
      .DI(writeDataLine),
      .MASKWE(4'b1111),
      .WE(controlState == WRITE && writeDataBuffer[0]),
      .CS(address[18:17] == 2'b01),
      .CK(mainClk),
      .STDBY(1'b0),
      .SLEEP(1'b0),
      .PWROFF_N(1'b1),
      .DO(readData1)
  );

  SP256K spram2 (
      .AD(address[16:3]),
      .DI(writeDataLine),
      .MASKWE(4'b1111),
      .WE(controlState == WRITE && writeDataBuffer[0]),
      .CS(address[18:17] == 2'b10),
      .CK(mainClk),
      .STDBY(1'b0),
      .SLEEP(1'b0),
      .PWROFF_N(1'b1),
      .DO(readData2)
  );

  SP256K spram3 (
      .AD(address[16:3]),
      .DI(writeDataLine),
      .MASKWE(4'b1111),
      .WE(controlState == WRITE && writeDataBuffer[0]),
      .CS(address[18:17] == 2'b11),
      .CK(mainClk),
      .STDBY(1'b0),
      .SLEEP(1'b0),
      .PWROFF_N(1'b1),
      .DO(readData3)
  );

  //* Address Logic
  assign address = controlState == READ || controlState == LOAD ? addressRead : addressBuffer;

  //* Read Data Logic
  always_comb begin
    case (address[18:17])
      2'b00: readData = readData0;
      2'b01: readData = readData1;
      2'b10: readData = readData2;
      2'b11: readData = readData3;
    endcase
  end

  //* Write Data Logic
  always_ff @(posedge mainClk, negedge nreset) begin : writeDataLogic
    if (~nreset) begin
      writeDataBuffer <= 0;
      writeDataLine   <= 0;
    end else begin
      if (load && addressWrite != addressBuffer) begin // done to prevent multiple writes to the same address (spi is slow)
        writeDataBuffer <= {writeData, 1'b1};
        addressBuffer   <= addressWrite;
      end

      if (controlState == LOAD && writeDataBuffer[0]) begin // checking 'valid' bit of write buffer
        writeDataLine <= readData;

        case (addressBuffer[2:0])
          3'b000:  writeDataLine[1:0] <= writeDataBuffer[2:1];
          3'b001:  writeDataLine[3:2] <= writeDataBuffer[2:1];
          3'b010:  writeDataLine[5:4] <= writeDataBuffer[2:1];
          3'b011:  writeDataLine[7:6] <= writeDataBuffer[2:1];
          3'b100:  writeDataLine[9:8] <= writeDataBuffer[2:1];
          3'b101:  writeDataLine[11:10] <= writeDataBuffer[2:1];
          3'b110:  writeDataLine[13:12] <= writeDataBuffer[2:1];
          3'b111:  writeDataLine[15:14] <= writeDataBuffer[2:1];
          default: writeDataLine <= 'bx;  // should never happen
        endcase
      end

      if (controlState == WRITE && writeDataBuffer[0]) begin  // unset 'valid' bit after write
        writeDataBuffer <= {writeDataBuffer[2:1], 1'b0};
      end
    end
  end

  //* Output Logic
  always_comb begin
    case (addressRead[2:0])
      3'b000:  outVal = readData[1:0];
      3'b001:  outVal = readData[3:2];
      3'b010:  outVal = readData[5:4];
      3'b011:  outVal = readData[7:6];
      3'b100:  outVal = readData[9:8];
      3'b101:  outVal = readData[11:10];
      3'b110:  outVal = readData[13:12];
      3'b111:  outVal = readData[15:14];
      default: outVal = 'bx;  // should never happen
    endcase
  end

endmodule
