module bar_ram
   #(
    parameter ADDR_WIDTH = 10,  // number of address bits
              DATA_WIDTH = 10    // color depth
   )
   (
    input  logic clk,
    input  logic we,
    input  logic [ADDR_WIDTH-1:0] addr_r,
    input  logic [ADDR_WIDTH-1:0] addr_w,
    input  logic [DATA_WIDTH-1:0] din,
    output logic [DATA_WIDTH-1:0] dout
   );

   // signal declaration
   logic [DATA_WIDTH-1:0] ram [0:2**ADDR_WIDTH-1];
   logic [DATA_WIDTH-1:0] data_reg;
   
   // bar_bitmap.txt specifies the initial values of ram 
   initial 
      $readmemb("C:/Users/DanKho/Documents/School/Vivado_Projects/ECE4305/M15_M18_Complete_System/M15_M18_Complete_System.srcs/sources_1/imports/HDL/txt_files/bar_bitmap.txt", ram);
   // body
   always_ff @(posedge clk)
   begin
      if (we)
         ram[addr_w] <= din;
      data_reg <= ram[addr_r];
   end
   assign dout = data_reg;
endmodule   

