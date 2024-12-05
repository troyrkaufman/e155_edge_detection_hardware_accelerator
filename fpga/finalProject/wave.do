onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /top_tb/spiClk
add wave -noupdate /top_tb/sdi
add wave -noupdate /top_tb/ncs
add wave -noupdate /top_tb/nreset
add wave -noupdate /top_tb/hSync
add wave -noupdate /top_tb/vSync
add wave -noupdate /top_tb/syncB
add wave -noupdate /top_tb/blankB
add wave -noupdate /top_tb/red
add wave -noupdate /top_tb/green
add wave -noupdate /top_tb/blue
add wave -noupdate -group spram /top_tb/dut/spramCont/mainClk
add wave -noupdate -group spram /top_tb/dut/spramCont/nreset
add wave -noupdate -group spram /top_tb/dut/spramCont/addressRead
add wave -noupdate -group spram /top_tb/dut/spramCont/addressWrite
add wave -noupdate -group spram /top_tb/dut/spramCont/writeData
add wave -noupdate -group spram /top_tb/dut/spramCont/load
add wave -noupdate -group spram /top_tb/dut/spramCont/vgaClk
add wave -noupdate -group spram /top_tb/dut/spramCont/outVal
add wave -noupdate -group spram /top_tb/dut/spramCont/controlState
add wave -noupdate -group spram /top_tb/dut/spramCont/nextState
add wave -noupdate -group spram /top_tb/dut/spramCont/writeDataBuffer
add wave -noupdate -group spram /top_tb/dut/spramCont/writeDataLine
add wave -noupdate -group spram /top_tb/dut/spramCont/address
add wave -noupdate -group spram /top_tb/dut/spramCont/addressBuffer
add wave -noupdate -group spram /top_tb/dut/spramCont/readData0
add wave -noupdate -group spram /top_tb/dut/spramCont/readData1
add wave -noupdate -group spram /top_tb/dut/spramCont/readData2
add wave -noupdate -group spram /top_tb/dut/spramCont/readData3
add wave -noupdate -group spram /top_tb/dut/spramCont/readData
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/mainClk
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/spiClk
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/nreset
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/sdi
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/ncs
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/pixelData
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/pixelDataValid
add wave -noupdate -expand -group spiCont -radix decimal /top_tb/dut/spiCont/spiXVal
add wave -noupdate -expand -group spiCont -radix decimal /top_tb/dut/spiCont/spiYVal
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/pixelDataReg
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/spiLine
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/modCounter
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/nextModCounter
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/spiDone
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/nextSpiXVal
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/nextSpiYVal
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/spiState
add wave -noupdate -expand -group spiCont /top_tb/dut/spiCont/nextSpiState
add wave -noupdate -group spiReceive /top_tb/dut/spiCont/spiReceiveInst/messageBits
add wave -noupdate -group spiReceive /top_tb/dut/spiCont/spiReceiveInst/spiClk
add wave -noupdate -group spiReceive /top_tb/dut/spiCont/spiReceiveInst/nreset
add wave -noupdate -group spiReceive /top_tb/dut/spiCont/spiReceiveInst/sdi
add wave -noupdate -group spiReceive /top_tb/dut/spiCont/spiReceiveInst/ncs
add wave -noupdate -group spiReceive /top_tb/dut/spiCont/spiReceiveInst/writeData
add wave -noupdate -group spiReceive /top_tb/dut/spiCont/spiReceiveInst/writeEnable
add wave -noupdate -group spiReceive /top_tb/dut/spiCont/spiReceiveInst/bitCounter
add wave -noupdate -group spiReceive /top_tb/dut/spiCont/spiReceiveInst/writeDataBuffer
add wave -noupdate -group spiReceive /top_tb/dut/spiCont/spiReceiveInst/currentState
add wave -noupdate -group spiReceive /top_tb/dut/spiCont/spiReceiveInst/nextState
add wave -noupdate -group spiReceive /top_tb/dut/spiCont/spiReceiveInst/temp
add wave -noupdate -group vga /top_tb/dut/vgaCont/vgaClk
add wave -noupdate -group vga /top_tb/dut/vgaCont/nreset
add wave -noupdate -group vga /top_tb/dut/vgaCont/hSync
add wave -noupdate -group vga /top_tb/dut/vgaCont/vSync
add wave -noupdate -group vga /top_tb/dut/vgaCont/syncB
add wave -noupdate -group vga /top_tb/dut/vgaCont/blankB
add wave -noupdate -group vga /top_tb/dut/vgaCont/hCount
add wave -noupdate -group vga /top_tb/dut/vgaCont/vCount
add wave -noupdate -group vga /top_tb/dut/vgaCont/hCountInternal
add wave -noupdate -group vga /top_tb/dut/vgaCont/vCountInternal
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/clk
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/nreset
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/inputValid
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/spiXVal
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/spiYVal
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/edgeVal
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/edgeValValid
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/edgeXVal
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/edgeYVal
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/pipelineState
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/nextState
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/row1ValReg
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/row3ValReg
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/nextRow1Val
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/nextRow3Val
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/col1ValReg
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/col3ValReg
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/nextCol1Val
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/nextCol3Val
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/rowDiffReg
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/colDiffReg
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/nextRowDiff
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/nextColDiff
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/rowSquare
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/colSquare
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/squareSumReg
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/nextSquareSum
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/sqrtValReg
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/nextSqrtVal
add wave -noupdate -group edge /top_tb/dut/edgeDetectInst/nextEdgeVal
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {49626667 ns} 0}
quietly wave cursor active 1
configure wave -namecolwidth 264
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {49626574 ns} {49626686 ns}
