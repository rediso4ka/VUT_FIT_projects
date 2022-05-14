-- uart.vhd: UART controller - receiving part
-- Author(s): xshevc01
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

-------------------------------------------------
entity UART_RX is
port(	
 	DIN: 	    in std_logic;
  CLK: 	    in std_logic;
	RST: 	    in std_logic;
	DOUT: 	    out std_logic_vector(7 downto 0);
	DOUT_VLD: 	out std_logic
);
end UART_RX;  

-------------------------------------------------
architecture behavioral of UART_RX is
signal CNT_BITS : std_logic_vector(3 downto 0);
signal CNT_CLK : std_logic_vector(4 downto 0);
signal CNT_CLK_EN : std_logic;
signal RECEIVE_EN : std_logic;
signal DATA_VLD : std_logic;
begin
    FSM: entity work.UART_FSM(behavioral)
    port map (
        DIN => DIN,
        CLK => CLK,
        RST => RST,
        CNT_BITS => CNT_BITS,
        CNT_CLK => CNT_CLK,
        CNT_CLK_EN => CNT_CLK_EN,
        RECEIVE_EN => RECEIVE_EN,
        DATA_VLD => DATA_VLD
    );
    DOUT_VLD <= DATA_VLD;
    process (CLK) begin
      if rising_edge(CLK) then
        if RST = '1' then
          CNT_CLK <= "00000";
          CNT_BITS <= "0000";
        else
          if CNT_CLK_EN = '1' then
            CNT_CLK <= CNT_CLK + 1;
          else
            CNT_CLK <= "00000";
          end if;
          if RECEIVE_EN = '0' then
            CNT_BITS <= "0000";
          end if;
          if CNT_BITS < "1000" and RECEIVE_EN = '1' and CNT_CLK(4) = '1' then
            DOUT(conv_integer(CNT_BITS)) <= DIN;--place data from DIN to corresponding index of DOUT
            CNT_BITS <= CNT_BITS + 1;
            CNT_CLK <= "00001";
          end if;
        end if;
      end if;
    end process;
end behavioral;
