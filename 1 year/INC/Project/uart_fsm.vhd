-- uart_fsm.vhd: UART controller - finite state machine
-- Author(s): xshevc01
--
library ieee;
use ieee.std_logic_1164.all;

-------------------------------------------------
entity UART_FSM is
port(
   DIN : in std_logic;
   CLK : in std_logic;
   RST : in std_logic;
   CNT_BITS : in std_logic_vector(3 downto 0);--max needed value is 8
   CNT_CLK : in std_logic_vector(4 downto 0);--max needed value is 24
   CNT_CLK_EN : out std_logic;
   RECEIVE_EN : out std_logic;
   DATA_VLD : out std_logic
   );
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is
type STATE_TYPE is (WAIT_START, WAIT_LSB, RECEIVE_BITS, WAIT_STOP, VALIDATION);
signal state : STATE_TYPE := WAIT_START;
begin
  CNT_CLK_EN <= '1' when state = WAIT_LSB or state = RECEIVE_BITS or state = WAIT_STOP else '0';
  RECEIVE_EN <= '1' when state = RECEIVE_BITS else '0';
  DATA_VLD <= '1' when state = VALIDATION else '0';
  process (CLK) begin
    if rising_edge(CLK) then
      if RST = '1' then
        state <= WAIT_START;
      else
        case state is
        when WAIT_START => if DIN = '0' then
                            state <= WAIT_LSB;
                           end if;
        when WAIT_LSB => if CNT_CLK = "11000" then
                            state <= RECEIVE_BITS;
                         end if;
        when RECEIVE_BITS => if CNT_BITS = "1000" then
                                state <= WAIT_STOP;
                             end if;
        when WAIT_STOP => if CNT_CLK = "10000" then-- we do not have to check STOP bit = 1
                            state <= VALIDATION;--    because in the task it is always set
                          end if;--                   so we just wait 16 clocks
        when VALIDATION => state <= WAIT_START; 
        when others         => null;
        end case;
      end if;
    end if;
  end process;
end behavioral;
