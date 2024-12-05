
module top_tb();
	logic spiClk, sdi, ncs;
	logic nreset;
	logic hSync, vSync, syncB, blankB;
	logic [1:0] red, green, bluel;


	dut top(spiClk, sdi, ncs, nreset, hSync, vSync, syncB, blankB);

	// generate clock
	always begin
		spiClk = 1; #5; spiClk = 0; #5;
	end

	initial begin
		nreset = 0; #22ns; nreset = 1; 
		sdi = 16'h0000; 
		ncs = 1; #10;  ncs = 0; #10;
		sdi = 16'hccb0; #20; sdi = 16'h0000; 
		ncs = 1; #10;  ncs = 0; #10;
		sdi = 16'hbcc0; #20; sdi = 16'h0000; 
		ncs = 1; #10;  ncs = 0; #10;
		sdi = 16'hbcc0; #20; sdi = 16'h0000; 
		ncs = 1; #10;
	end

endmodule 
		
