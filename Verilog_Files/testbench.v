

// 
// Module: tb
// 
// Notes:
// - Top level simulation testbench.
//

//`timescale 1ns/1ns
//`define WAVES_FILE "./work/waves-rx.vcd"

module tb();
    
reg        clk          ; // Top level system clock input.
reg rst;
reg neg_clk; 
reg neg_rst ; 
reg        resetn       ;
reg        uart_rxd     ; // UART Recieve pin.

reg        uart_rx_en   ; // Recieve enable
//wire [8:0] res;
wire       uart_rx_break; // Did we get a BREAK message?
wire       uart_rx_valid; // Valid data recieved and available.
wire [7:0] uart_rx_data ; // The recieved data.
wire [31:0] inst ; 
wire [31:0] inst_mem ; 

reg rst_pin ; 
wire write_done ; 


// Bit rate of the UART line we are testing.
localparam BIT_RATE = 9600;
localparam BIT_P    = (1000000000/BIT_RATE);


// Period and frequency of the system clock.
localparam CLK_HZ   = 50000000;
localparam CLK_P    = 1000000000/ CLK_HZ;

reg slow_clk = 0;


// Make the clock tick.
always begin #(CLK_P/2) clk  = ~clk; end   
always begin #(CLK_P/2) neg_clk  = ~neg_clk; end     
always begin #(CLK_P*2) slow_clk <= !slow_clk;end



task write_instruction;
    input [31:0] instruction;
    begin
            @(posedge clk);
            send_byte(instruction[7:0]);
            check_byte(instruction[7:0]);
            @(posedge clk);
            send_byte(instruction[15:8]);
            check_byte(instruction[15:8]);
            
            @(posedge clk);
            send_byte(instruction[23:16]);
            check_byte(instruction[23:16]);
            
            @(posedge clk);
            send_byte(instruction[31:24]);
            check_byte(instruction[31:24]);
    end
    endtask

task send_byte;
    input [7:0] to_send;
    integer i;
    begin


        #BIT_P;  uart_rxd = 1'b0;
        for(i=0; i < 8; i = i+1) begin
            #BIT_P;  uart_rxd = to_send[i];
        end
        #BIT_P;  uart_rxd = 1'b1;
        #1000;
    end
endtask


// Checks that the output of the UART is the value we expect.
integer passes = 0;
integer fails  = 0;
task check_byte;
    input [7:0] expected_value;
    begin
        if(uart_rx_data == expected_value) begin
            passes = passes + 1;
            $display("%d/%d/%d [PASS] Expected %b and got %b", 
                     passes,fails,passes+fails,
                     expected_value, uart_rx_data);
        end else begin
            fails  = fails  + 1;
            $display("%d/%d/%d [FAIL] Expected %b and got %b", 
                     passes,fails,passes+fails,
                     expected_value, uart_rx_data);
        end
    end
endtask


initial 
begin 
    $dumpfile("waveform.vcd");
    $dumpvars(0,tb);
end 

reg [1:0] input_wires; 
wire [2:0] output_wires ; 
wire [2:0] pc ; 


reg [7:0] to_send;
initial begin
    rst=1;
    rst_pin=1; 
    neg_rst = 1; 
    resetn  = 1'b0;
    clk     = 1'b0;
    neg_clk = 1; 
    neg_rst = ~clk ;
    uart_rxd = 1'b1;
    neg_clk = 1'b1; 
    input_wires = 2'b10;
    #4000
    resetn = 1'b1;
    rst=0;
    neg_rst = 0; 
    rst_pin = 0 ; 
  

    uart_rx_en = 1'b1;
    /*
    @(posedge slow_clk);write_instruction(32'h00000000); 
    @(posedge slow_clk);write_instruction(32'h00000000); 
    @(posedge slow_clk);write_instruction(32'hfa010113); 
    @(posedge slow_clk);write_instruction(32'h04812e23); 
    @(posedge slow_clk);write_instruction(32'h06010413); 
    @(posedge slow_clk);write_instruction(32'hfc042823); 
    @(posedge slow_clk);write_instruction(32'hfe042623); 
    @(posedge slow_clk);write_instruction(32'hfe042423); 
    @(posedge slow_clk);write_instruction(32'hfe042223); 
    @(posedge slow_clk);write_instruction(32'hfe042023); 
    @(posedge slow_clk);write_instruction(32'hfc042623); 
    @(posedge slow_clk);write_instruction(32'hfc042e23); 
    @(posedge slow_clk);write_instruction(32'hfd042783); 
    @(posedge slow_clk);write_instruction(32'h00279793); 
    @(posedge slow_clk);write_instruction(32'hfcf42423); 
    @(posedge slow_clk);write_instruction(32'hfec42783); 
    @(posedge slow_clk);write_instruction(32'h00379793); 
    @(posedge slow_clk);write_instruction(32'hfcf42223); 
    @(posedge slow_clk);write_instruction(32'hfe842783); 
    @(posedge slow_clk);write_instruction(32'h00479793); 
    @(posedge slow_clk);write_instruction(32'hfcf42023); 
    @(posedge slow_clk);write_instruction(32'hfc842783); 
    @(posedge slow_clk);write_instruction(32'hfc442703); 
    @(posedge slow_clk);write_instruction(32'hfc042683); 
    @(posedge slow_clk);write_instruction(32'hfe3f7f13); 
    @(posedge slow_clk);write_instruction(32'h00ff6f33); 
    @(posedge slow_clk);write_instruction(32'h00ef6f33); 
    @(posedge slow_clk);write_instruction(32'h00df6f33); 
    @(posedge slow_clk);write_instruction(32'h001f7793); 
    @(posedge slow_clk);write_instruction(32'hfcf42623); 
    @(posedge slow_clk);write_instruction(32'hfcc42783); 
    @(posedge slow_clk);write_instruction(32'h06078063); 
    @(posedge slow_clk);write_instruction(32'hfc042823); 
    @(posedge slow_clk);write_instruction(32'hfe042623); 
    @(posedge slow_clk);write_instruction(32'hfe042423); 
    @(posedge slow_clk);write_instruction(32'hfe042223); 
    @(posedge slow_clk);write_instruction(32'hfe042023); 
    @(posedge slow_clk);write_instruction(32'hfc042e23); 
    @(posedge slow_clk);write_instruction(32'hfd042783); 
    @(posedge slow_clk);write_instruction(32'h00279793); 
    @(posedge slow_clk);write_instruction(32'hfcf42423); 
    @(posedge slow_clk);write_instruction(32'hfec42783); 
    @(posedge slow_clk);write_instruction(32'h00379793); 
    @(posedge slow_clk);write_instruction(32'hfcf42223); 
    @(posedge slow_clk);write_instruction(32'hfe842783); 
    @(posedge slow_clk);write_instruction(32'h00479793); 
    @(posedge slow_clk);write_instruction(32'hfcf42023); 
    @(posedge slow_clk);write_instruction(32'hfc842783); 
    @(posedge slow_clk);write_instruction(32'hfc442703); 
    @(posedge slow_clk);write_instruction(32'hfc042683); 
    @(posedge slow_clk);write_instruction(32'hfe3f7f13); 
    @(posedge slow_clk);write_instruction(32'h00ff6f33); 
    @(posedge slow_clk);write_instruction(32'h00ef6f33); 
    @(posedge slow_clk);write_instruction(32'h00df6f33); 
    @(posedge slow_clk);write_instruction(32'hf99ff06f); 
    @(posedge slow_clk);write_instruction(32'hfec42783); 
    @(posedge slow_clk);write_instruction(32'hf80798e3); 
    @(posedge slow_clk);write_instruction(32'hfe842783); 
    @(posedge slow_clk);write_instruction(32'hf80794e3); 
    @(posedge slow_clk);write_instruction(32'hfcc42783); 
    @(posedge slow_clk);write_instruction(32'hf80790e3); 
    @(posedge slow_clk);write_instruction(32'h002f7793); 
    @(posedge slow_clk);write_instruction(32'hfaf42e23); 
    @(posedge slow_clk);write_instruction(32'hfbc42783); 
    @(posedge slow_clk);write_instruction(32'h4017d793); 
    @(posedge slow_clk);write_instruction(32'hfaf42c23); 
    @(posedge slow_clk);write_instruction(32'hfb842783); 
    @(posedge slow_clk);write_instruction(32'h10078863); 
    @(posedge slow_clk);write_instruction(32'h00100793); 
    @(posedge slow_clk);write_instruction(32'hfcf42823); 
    @(posedge slow_clk);write_instruction(32'hfd042783); 
    @(posedge slow_clk);write_instruction(32'h00279793); 
    @(posedge slow_clk);write_instruction(32'hfcf42423); 
    @(posedge slow_clk);write_instruction(32'hfc842783); 
    @(posedge slow_clk);write_instruction(32'hffbf7f13); 
    @(posedge slow_clk);write_instruction(32'h00ff6f33); 
    @(posedge slow_clk);write_instruction(32'hfe442783); 
    @(posedge slow_clk);write_instruction(32'h00178793); 
    @(posedge slow_clk);write_instruction(32'hfef42223); 
    @(posedge slow_clk);write_instruction(32'hfe442703); 
    @(posedge slow_clk);write_instruction(32'hfe042783); 
    @(posedge slow_clk);write_instruction(32'h00f707b3); 
    @(posedge slow_clk);write_instruction(32'hfaf42a23); 
    @(posedge slow_clk);write_instruction(32'hfc042c23); 
    @(posedge slow_clk);write_instruction(32'h06400793); 
    @(posedge slow_clk);write_instruction(32'hfcf42a23); 
    @(posedge slow_clk);write_instruction(32'hfb442783); 
    @(posedge slow_clk);write_instruction(32'hfaf42823); 
    @(posedge slow_clk);write_instruction(32'h0200006f); 
    @(posedge slow_clk);write_instruction(32'hfd442703); 
    @(posedge slow_clk);write_instruction(32'hfb042783); 
    @(posedge slow_clk);write_instruction(32'h40f707b3); 
    @(posedge slow_clk);write_instruction(32'hfcf42a23); 
    @(posedge slow_clk);write_instruction(32'hfd842783); 
    @(posedge slow_clk);write_instruction(32'h00178793); 
    @(posedge slow_clk);write_instruction(32'hfcf42c23); 
    @(posedge slow_clk);write_instruction(32'hfd442703); 
    @(posedge slow_clk);write_instruction(32'hfb042783); 
    @(posedge slow_clk);write_instruction(32'hfcf75ee3); 
    @(posedge slow_clk);write_instruction(32'hfd842783); 
    @(posedge slow_clk);write_instruction(32'hfaf42623); 
    @(posedge slow_clk);write_instruction(32'hfac42703); 
    @(posedge slow_clk);write_instruction(32'h00070793); 
    @(posedge slow_clk);write_instruction(32'h00479793); 
    @(posedge slow_clk);write_instruction(32'h00e787b3); 
    @(posedge slow_clk);write_instruction(32'hfaf42423); 
    @(posedge slow_clk);write_instruction(32'hfdc42703); 
    @(posedge slow_clk);write_instruction(32'hfa842783); 
    @(posedge slow_clk);write_instruction(32'h00f707b3); 
    @(posedge slow_clk);write_instruction(32'hfcf42e23); 
    @(posedge slow_clk);write_instruction(32'hfa842703); 
    @(posedge slow_clk);write_instruction(32'h06600793); 
    @(posedge slow_clk);write_instruction(32'heae7d8e3); 
    @(posedge slow_clk);write_instruction(32'h00100793); 
    @(posedge slow_clk);write_instruction(32'hfef42623); 
    @(posedge slow_clk);write_instruction(32'h00100793); 
    @(posedge slow_clk);write_instruction(32'hfef42423); 
    @(posedge slow_clk);write_instruction(32'hfc042823); 
    @(posedge slow_clk);write_instruction(32'hfec42783); 
    @(posedge slow_clk);write_instruction(32'h00379793); 
    @(posedge slow_clk);write_instruction(32'hfcf42223); 
    @(posedge slow_clk);write_instruction(32'hfe842783); 
    @(posedge slow_clk);write_instruction(32'h00479793); 
    @(posedge slow_clk);write_instruction(32'hfcf42023); 
    @(posedge slow_clk);write_instruction(32'hfd042783); 
    @(posedge slow_clk);write_instruction(32'h00279793); 
    @(posedge slow_clk);write_instruction(32'hfcf42423); 
    @(posedge slow_clk);write_instruction(32'hfc442783); 
    @(posedge slow_clk);write_instruction(32'hfc042703); 
    @(posedge slow_clk);write_instruction(32'hfc842683); 
    @(posedge slow_clk);write_instruction(32'hfe3f7f13); 
    @(posedge slow_clk);write_instruction(32'h00ff6f33); 
    @(posedge slow_clk);write_instruction(32'h00ef6f33); 
    @(posedge slow_clk);write_instruction(32'h00df6f33); 
    @(posedge slow_clk);write_instruction(32'he59ff06f); 
    @(posedge slow_clk);write_instruction(32'hfc042823); 
    @(posedge slow_clk);write_instruction(32'hfd042783); 
    @(posedge slow_clk);write_instruction(32'h00279793); 
    @(posedge slow_clk);write_instruction(32'hfcf42423); 
    @(posedge slow_clk);write_instruction(32'hfc842783); 
    @(posedge slow_clk);write_instruction(32'hffbf7f13); 
    @(posedge slow_clk);write_instruction(32'h00ff6f33); 
    @(posedge slow_clk);write_instruction(32'hfe042783); 
    @(posedge slow_clk);write_instruction(32'h00178793); 
    @(posedge slow_clk);write_instruction(32'hfef42023); 
    @(posedge slow_clk);write_instruction(32'he2dff06f); 
    @(posedge slow_clk);write_instruction(32'hffffffff); 
    @(posedge slow_clk);write_instruction(32'hffffffff); 
    */
     $display("Test Results:");
     $display("    PASSES: %d", passes);
     $display("    FAILS : %d", fails);
    #100000
    $display("Finish simulation at time %d", $time);
    $finish;
end

 wrapper dut (
.clk        (clk          ), // Top level system clock input.
.resetn       (resetn       ), // Asynchronous active low reset.
.uart_rxd     (uart_rxd     ), // UART Recieve pin.
.uart_rx_en   (uart_rx_en   ), // Recieve enable
.uart_rx_break(uart_rx_break), // Did we get a BREAK message?
.uart_rx_valid(uart_rx_valid), // Valid data recieved and available.
.uart_rx_data (uart_rx_data ), 
.input_gpio_pins(input_wires), 
.output_gpio_pins(output_wires), 
.write_done(write_done)
); 



endmodule
