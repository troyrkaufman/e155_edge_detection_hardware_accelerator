module bitCompress (
  input logic [6:0] inVal,
  output logic [1:0] outVal
);

  always_comb begin // this is done assuming the max inVal is 85
    if (inVal < 21) outVal = 2'b00;
    else if (inVal < 42) outVal = 2'b01;
    else if (inVal < 63) outVal = 2'b10;
    else outVal = 2'b11;
  end
  
endmodule