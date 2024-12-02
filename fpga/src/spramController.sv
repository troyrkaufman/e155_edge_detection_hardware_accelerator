module spramController (
    input logic mainClk,
    input logic nRst,
    input logic [9:0] xCoordVga, xCoordWrite,
    input logic [8:0] yCoordVga, yCoordWrite,
    input logic [1:0] writeData,
    output logic vgaClk,
    output logic [1:0] outVal
);

  //* Clock Dividing

  //* Control Logic
  typedef enum logic [1:0] {
    READ  = 0,
    LOAD  = 1,
    WRITE = 2,
    WAIT  = 3
  } controlState_t;

  controlState_t controlState;

  always_ff @(posedge mainClk, posedge nRst) begin : controlLogic
    if (~nRst) controlState <= READ;
    else controlState <= controlState + 1;
  end

  assign vgaClk = ~controlState[1];

  //* Data Logic
  logic [15:0] writeData;
  logic [15:0] address;
  logic [15:0] readData0, readData1, readData2;

  //* SPARM Instantiation
  SP256K spram0 (
    .AD(address[13:0]),
    .DI(writeData),
    .MASKWE(4'b1111),
    .WE(controlState == WRITE),
    .CS(address[15:14] == 2'b00),
    .CK(mainClk),
    .STDBY(1'b0),
    .SLEEP(1'b0),
    .PWROFF_N(1'b1),
    .DO(readData0)
  );

  SP256K spram1 (
    .AD(address[13:0]),
    .DI(writeData),
    .MASKWE(4'b1111),
    .WE(controlState == WRITE),
    .CS(address[15:14] == 2'b01),
    .CK(mainClk),
    .STDBY(1'b0),
    .SLEEP(1'b0),
    .PWROFF_N(1'b1),
    .DO(readData1)
  );

  SP256K spram2 (
    .AD(address[13:0]),
    .DI(writeData),
    .MASKWE(4'b1111),
    .WE(controlState == WRITE),
    .CS(address[15:14] == 2'b10),
    .CK(mainClk),
    .STDBY(1'b0),
    .SLEEP(1'b0),
    .PWROFF_N(1'b1),
    .DO(readData2)
  );

  //* Address Logic
  always_ff @(posedge mainClk, posedge nRst) begin : addressLogic
    if (~nRst) address <= 0;
    else if (controlState == READ) begin
      address <= {xCoordVga, yCoordVga};
    end else if (controlState == WRITE || controlState == LOAD) begin
      address <= {xCoordWrite, yCoordWrite};
    end
  end

  



  // SP256K spram (
  //   .clk(vgaClk),
  //   .we(writeData[0]),
  //   .addr({xCoord, yCoord}),
  //   .data(writeData[7:1]),
  //   .q(outVal)
  // );

endmodule
