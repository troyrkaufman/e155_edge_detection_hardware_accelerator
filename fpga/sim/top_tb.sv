
`timescale 1ns/1ns
module top_tb();
	logic spiClk, sdi, ncs;
	logic nreset;
	logic hSync, vSync, syncB, blankB;
	logic [1:0] red, green, blue;

	top dut(spiClk, sdi, ncs, nreset, hSync, vSync, syncB, blankB, red, green, blue);

	// generate clock
	always begin
		spiClk = 1; #25; spiClk = 0; #25;
	end

	initial begin
		nreset = 0; #22ns; nreset = 1; 
		sdi = 16'h0000; 
		ncs = 1; #10;  ncs = 0; #10;
		sdi = 1; #50;
		sdi = 1; #50;
		sdi = 1; #50;
		sdi = 1; #50;
		sdi = 1; #50;
		sdi = 1; #50;
		sdi = 1; #50;
		sdi = 1; #50;
		sdi = 1; #50;
		sdi = 1; #50;
		sdi = 1; #50;
		sdi = 1; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		sdi = 0; #50;
		ncs = 1;
		#5000;
		$finish;
	end

endmodule 
		