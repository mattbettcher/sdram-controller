// Created by fizzim.pl version 5.20 on 2018:03:10 at 19:39:41 (www.fizzim.com)

module sdram_controller (
  output logic [10:0] addr,
  output logic [1:0] ba,
  output logic cas_n,
  output logic cke,
  output logic cs_n,
  output logic dqmh,
  output logic dqml,
  output logic [20:0] init_timer,
  output logic ras_n,
  output logic we_n,
  input logic clk,
  input logic rst_n
);

  // state bits
  enum logic [4:0] {
    Init = 5'b00000, // we_n=0 ras_n=0 cs_n=0 cke=0 cas_n=0 
    Nop  = 5'b11001  // we_n=1 ras_n=1 cs_n=0 cke=0 cas_n=1
  } state, nextstate;


  // comb always block
  always_comb begin
    // Warning I2: Neither implied_loopback nor default_state_is_x attribute is set on state machine - defaulting to implied_loopback to avoid latches being inferred 
    nextstate = state; // default to hold value because implied_loopback is set
    case (state)
      Init: begin
        if (init_timer > 20'h5) begin
          nextstate = Nop;
        end
        else begin
          nextstate = Init;
        end
      end
      Nop : begin
        begin
          nextstate = Nop;
        end
      end
    endcase
  end

  // Assign reg'd outputs to state bits
  assign cas_n = state[0];
  assign cke = state[1];
  assign cs_n = state[2];
  assign ras_n = state[3];
  assign we_n = state[4];

  // sequential always block
  always_ff @(posedge clk) begin
    if (!rst_n)
      state <= Init;
    else
      state <= nextstate;
  end

  // datapath sequential always block
  always_ff @(posedge clk) begin
    if (!rst_n) begin
      // Warning R18: No reset value set for datapath output addr[10:0].   Assigning a reset value of 11'h0 based on value in reset state Init 
      addr[10:0] <= 11'h0;
      // Warning R18: No reset value set for datapath output ba[1:0].   Assigning a reset value of 0 based on value in reset state Init 
      ba[1:0] <= 0;
      // Warning R18: No reset value set for datapath output dqmh.   Assigning a reset value of 1 based on value in reset state Init 
      dqmh <= 1;
      // Warning R18: No reset value set for datapath output dqml.   Assigning a reset value of 1 based on value in reset state Init 
      dqml <= 1;
      // Warning R18: No reset value set for datapath output init_timer[20:0].   Assigning a reset value of init_timer + 1 based on value in reset state Init 
      init_timer[20:0] <= init_timer + 1;
    end
    else begin
      addr[10:0] <= 0; // default
      ba[1:0] <= 0; // default
      dqmh <= 0; // default
      dqml <= 0; // default
      init_timer[20:0] <= 0; // default
      case (nextstate)
        Init: begin
          addr[10:0] <= 11'h0;
          dqmh <= 1;
          dqml <= 1;
          init_timer[20:0] <= init_timer + 1;
        end
      endcase
    end
  end
endmodule
