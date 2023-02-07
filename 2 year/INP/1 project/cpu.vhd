-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2022 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): xshevc01
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is
        -- __CNT__
    signal cnt_inc: std_logic;
    signal cnt_dec: std_logic;
    signal cnt_rst: std_logic;
    signal cnt_reg: std_logic_vector(12 downto 0);

        -- __PC__
    signal pc_inc: std_logic;
    signal pc_dec: std_logic;
    signal pc_rst: std_logic;
    signal pc_reg: std_logic_vector(12 downto 0);

        -- __PTR__
    signal ptr_inc: std_logic;
    signal ptr_dec: std_logic;
    signal ptr_rst: std_logic;
    signal ptr_reg: std_logic_vector(12 downto 0);

        -- __MX1__
    signal mx1_sel: std_logic;
    signal mx1_out: std_logic_vector(12 downto 0);

        -- __MX2__
    signal mx2_sel: std_logic_vector(1 downto 0);
    signal mx2_out: std_logic_vector(7 downto 0);

    type STATE_TYPE is (
        IDLE,
        WAIT_A_SEC,
		FETCH,
		DECODE,
		INC_PTR,
		DEC_PTR,
		INC_VAL_START, INC_VAL_TO_DATA, INC_VAL_INC, INC_VAL_END,
		DEC_VAL_START, DEC_VAL_TO_DATA, DEC_VAL_DEC, DEC_VAL_END,
		WHILE_START, WHILE_GET_DATA, WHILE_CHECK_DATA, WHILE_CHECK_CNT, WHILE_FIND_BRACKET,
		END_WHILE_START, END_WHILE_GET_DATA, END_WHILE_CHECK_DATA, END_WHILE_WAIT_DATA, END_WHILE_CHECK_CNT, END_WHILE_CHECK_FOUND, END_WHILE_FIND_BRACKET, 
        DOWHILE_START,
        END_DOWHILE_START, END_DOWHILE_GET_DATA, END_DOWHILE_CHECK_DATA, END_DOWHILE_WAIT_DATA, END_DOWHILE_CHECK_CNT, END_DOWHILE_CHECK_FOUND, END_DOWHILE_FIND_BRACKET,
		PUTCHAR_START, PUTCHAR_IN, PUTCHAR_END,
		GETCHAR_START, GETCHAR_IN, GETCHAR_END,
		S_RETURN,
		S_OTHERS
    );
    
    signal fsm_state : STATE_TYPE := IDLE;
    signal fsm_next_state : STATE_TYPE;

begin

    cnt: process (CLK, RESET, cnt_inc, cnt_dec)
    begin
        if RESET = '1' then
            cnt_reg <= (others => '0');
        elsif rising_edge(CLK) then
            if cnt_inc = '1' then
                cnt_reg <= cnt_reg + 1;
            elsif cnt_dec = '1' then
                cnt_reg <= cnt_reg - 1;
            elsif cnt_rst = '1' then
                cnt_reg <= (others => '0'); 
            end if;
        end if;
    end process cnt;

    pc: process (CLK, RESET, pc_inc, pc_dec)
    begin
        if RESET = '1' then
            pc_reg <= (others => '0');
        elsif rising_edge(CLK) then
            if pc_inc = '1' then
                pc_reg <= pc_reg + 1;
            elsif pc_dec = '1' then
                pc_reg <= pc_reg - 1;
            elsif pc_rst = '1' then
                pc_reg <= (others => '0');
            end if;
        end if;
    end process pc;

    ptr: process (CLK, RESET, ptr_inc, ptr_dec)
    begin
        if RESET = '1' then
            ptr_reg <= (12 => '1', others => '0');
        elsif rising_edge(CLK) then
            if ptr_inc = '1' then
                ptr_reg <= ptr_reg + 1;
            elsif ptr_dec = '1' then
                ptr_reg <= ptr_reg - 1;
            elsif ptr_rst = '1' then
                ptr_reg <= (12 => '1', others => '0');
            end if;
        end if;
    end process ptr;

    mx1: process (CLK, RESET, mx1_sel, pc_reg, ptr_reg)
    begin
        if RESET = '1' then
            mx1_out <= (12 => '1', others => '0');
        elsif rising_edge(CLK) then
            case mx1_sel is
                when '0'    => mx1_out <= pc_reg;
                when '1'    => mx1_out <= ptr_reg;
                when others => mx1_out <= (12 => '1', others => '0');
            end case;
        end if;
    end process mx1;

    mx2: process (CLK, RESET, mx2_sel, IN_DATA, DATA_RDATA)
    begin
        if RESET = '1' then
            mx2_out <= (others => '0');
        elsif rising_edge(CLK) then
            case mx2_sel is
                when "00"   => mx2_out <= IN_DATA;
                when "01"   => mx2_out <= DATA_RDATA - 1;
                when "10"   => mx2_out <= DATA_RDATA + 1;
                when others => mx2_out <= (others => '0');
            end case;
        end if;
    end process mx2;

    DATA_ADDR <= mx1_out;
    DATA_WDATA <= mx2_out;
    OUT_DATA <= DATA_RDATA;
    
    fsm_state_proc: process (CLK, RESET, EN)
    begin
        if RESET = '1' then
            fsm_state <= IDLE;
        elsif rising_edge(CLK) and EN = '1' then
            fsm_state <= fsm_next_state;
        end if;
    end process fsm_state_proc;

    fsm: process (RESET, EN, DATA_RDATA, IN_DATA, fsm_state, OUT_BUSY, IN_VLD)
    begin
        -- __PC__
        pc_inc <= '0';
        pc_dec <= '0';
        pc_rst <= '0';
        -- __PTR__
        ptr_inc <= '0';
        ptr_dec <= '0';
        ptr_rst <= '0';
        -- __CNT__
        cnt_inc <= '0';
        cnt_dec <= '0';
        cnt_rst <= '0';
        -- __IN-OUT__
        IN_REQ <= '0';
        OUT_WE <= '0';
        -- __MX1__
        mx1_sel <= '0';
        -- __MX2__
        mx2_sel <= "11";
        -- __DATA__
        DATA_RDWR <= '0';
        DATA_EN <= '0';
        

        case fsm_state is
            when IDLE =>
                cnt_rst <= '1';
                pc_rst <= '1';
                ptr_rst <= '1';

                fsm_next_state <= FETCH;

            when WAIT_A_SEC =>

                fsm_next_state <= FETCH;

            when FETCH =>
                DATA_EN <= '1';

                fsm_next_state <= DECODE;

            when DECODE =>
                case DATA_RDATA is
                    when X"3E" =>
                        fsm_next_state <= INC_PTR;
                    when X"3C" =>
                        fsm_next_state <= DEC_PTR;
                    when X"2B" =>
                        fsm_next_state <= INC_VAL_START;
                    when X"2D" =>
                        fsm_next_state <= DEC_VAL_START;
                    when X"5B" =>
                        fsm_next_state <= WHILE_START;
                    when X"5D" =>
                        fsm_next_state <= END_WHILE_START;
                    when X"28" =>
                       fsm_next_state <= DOWHILE_START;
                    when X"29" =>
                       fsm_next_state <= END_DOWHILE_START;
                    when X"2E" =>
                        fsm_next_state <= PUTCHAR_START;
                    when X"2C" =>
                        fsm_next_state <= GETCHAR_START;
                    when X"00" =>
                        fsm_next_state <= S_RETURN;
                    when others =>
                        fsm_next_state <= S_OTHERS;
                end case;
            
            --- inkrementace hodnoty ukazatele
            when INC_PTR =>
                ptr_inc <= '1';
                pc_inc <= '1';

                fsm_next_state <= WAIT_A_SEC;

            --- dekrementace hodnoty ukazatele
            when DEC_PTR =>
                ptr_dec <= '1';
                pc_inc <= '1';

                fsm_next_state <= WAIT_A_SEC;
            
            --- incrementace hodnoty aktualni bunky
            when INC_VAL_START =>
                mx1_sel <= '1';

                fsm_next_state <= INC_VAL_TO_DATA;
                    
            when INC_VAL_TO_DATA =>
                DATA_EN <= '1';
                mx1_sel <= '1';

                fsm_next_state <= INC_VAL_INC;

            when INC_VAL_INC =>
                mx2_sel <= "10";
                mx1_sel <= '1';
                pc_inc <= '1';

                fsm_next_state <= INC_VAL_END;

            when INC_VAL_END =>
                DATA_EN <= '1';
                DATA_RDWR <= '1';

                fsm_next_state <= FETCH;

            --- decrementace hodnoty aktualni bunky
            when DEC_VAL_START =>
                mx1_sel <= '1';

                fsm_next_state <= DEC_VAL_TO_DATA;
                    
            when DEC_VAL_TO_DATA =>
                DATA_EN <= '1';
                mx1_sel <= '1';

                fsm_next_state <= DEC_VAL_DEC;

            when DEC_VAL_DEC =>
                mx2_sel <= "01";
                mx1_sel <= '1';
                pc_inc <= '1';

                fsm_next_state <= DEC_VAL_END;

            when DEC_VAL_END =>
                DATA_EN <= '1';
                DATA_RDWR <= '1';

                fsm_next_state <= FETCH;

            --- vytiskni hodnotu aktualni bunky
            when PUTCHAR_START =>
                mx1_sel <= '1';

                fsm_next_state <= PUTCHAR_IN;
            
            when PUTCHAR_IN =>
                DATA_EN <= '1';
                DATA_RDWR <= '0';
                mx1_sel <= '1';

                fsm_next_state <= PUTCHAR_END;

            when PUTCHAR_END =>
                if OUT_BUSY = '1' then
                    mx1_sel <= '1';
                    DATA_EN <= '1';
                    DATA_RDWR <= '0';

                    fsm_next_state <= PUTCHAR_END;
                else
                    mx1_sel <= '0';
                    OUT_WE <= '1';
                    pc_inc <= '1';
                    
                    fsm_next_state <= WAIT_A_SEC;
                end if;

            --- nacti hodnotu a uloz ji do aktualnı bunky
            when GETCHAR_START =>
                mx1_sel <= '1';
                if IN_VLD /= '1' then
                    IN_REQ <= '1';
                    
                    fsm_next_state <= GETCHAR_START;
                else
                    DATA_EN <= '1';
                    mx2_sel <= "00";
                    
                    fsm_next_state <= GETCHAR_IN;
                end if;

            when GETCHAR_IN =>
                DATA_EN <= '1';
                mx1_sel <= '1';
                DATA_RDWR <= '1';
                pc_inc <= '1';
                
                fsm_next_state <= GETCHAR_END;

            when GETCHAR_END =>
            
                fsm_next_state <= FETCH;
                
            --- while start
            when WHILE_START =>
                mx1_sel <= '1';

                fsm_next_state <= WHILE_GET_DATA;

            when WHILE_GET_DATA =>
                pc_inc <= '1';
                DATA_EN <= '1';

                fsm_next_state <= WHILE_CHECK_DATA;

            when WHILE_CHECK_DATA =>
                if DATA_RDATA /= "00000000" then
                
                    fsm_next_state <= FETCH;
                else
                    DATA_EN <= '1';
                    cnt_inc <= '1';

                    fsm_next_state <= WHILE_CHECK_CNT;
                end if;

            when WHILE_CHECK_CNT =>
                if cnt_reg = "0000000000000" then

                    fsm_next_state <= FETCH;
                else
                    if DATA_RDATA = X"5B" then
                        cnt_inc <= '1';
                    elsif DATA_RDATA = X"5D" then
                        cnt_dec <= '1';
                    end if;
                    pc_inc <= '1';

                    fsm_next_state <= WHILE_FIND_BRACKET;
                end if;

            when WHILE_FIND_BRACKET =>
                DATA_EN <= '1';

                fsm_next_state <= WHILE_CHECK_CNT;

            --- while end
            when END_WHILE_START =>
                mx1_sel <= '1';

                fsm_next_state <= END_WHILE_GET_DATA;

            when END_WHILE_GET_DATA =>
                DATA_EN <= '1';

                fsm_next_state <= END_WHILE_CHECK_DATA;

            when END_WHILE_CHECK_DATA =>
                if DATA_RDATA = (DATA_RDATA'range => '0') then
                    pc_inc <= '1';

                    fsm_next_state <= WAIT_A_SEC;
                else
                    cnt_inc <= '1';
                    pc_dec <= '1';

                    fsm_next_state <= END_WHILE_CHECK_CNT;
                end if;
            
            when END_WHILE_CHECK_CNT =>
                if cnt_reg = (cnt_reg'range => '0') then

                    fsm_next_state <= FETCH;
                else 
                    if DATA_RDATA = X"5D" then
                        cnt_inc <= '1';
                    elsif DATA_RDATA = X"5B" then
                        cnt_dec <= '1';
                    end if;

                    fsm_next_state <= END_WHILE_CHECK_FOUND;
                end if;

            when END_WHILE_CHECK_FOUND =>
                if cnt_reg = (cnt_reg'range => '0') then
                    pc_inc <= '1';
                else
                    pc_dec <= '1';
                end if;

                fsm_next_state <= END_WHILE_FIND_BRACKET;
            
            when END_WHILE_FIND_BRACKET =>
                DATA_EN <= '1';

                fsm_next_state <= END_WHILE_CHECK_CNT;


            --- do while start
            when DOWHILE_START =>
                pc_inc <= '1';

                fsm_next_state <= WAIT_A_SEC;


            --- do while end
            when END_DOWHILE_START =>
                mx1_sel <= '1';

                fsm_next_state <= END_DOWHILE_GET_DATA;

            when END_DOWHILE_GET_DATA =>
                DATA_EN <= '1';

                fsm_next_state <= END_DOWHILE_CHECK_DATA;

            when END_DOWHILE_CHECK_DATA =>
                if DATA_RDATA = (DATA_RDATA'range => '0') then
                    pc_inc <= '1';

                    fsm_next_state <= WAIT_A_SEC;
                else
                    cnt_inc <= '1';
                    pc_dec <= '1';

                    fsm_next_state <= END_DOWHILE_CHECK_CNT;
                end if;
            
            when END_DOWHILE_CHECK_CNT =>
                if cnt_reg = (cnt_reg'range => '0') then

                    fsm_next_state <= FETCH;
                else 
                    if DATA_RDATA = X"29" then
                        cnt_inc <= '1';
                    elsif DATA_RDATA = X"28" then
                        cnt_dec <= '1';
                    end if;

                    fsm_next_state <= END_DOWHILE_CHECK_FOUND;
                end if;

            when END_DOWHILE_CHECK_FOUND =>
                if cnt_reg = (cnt_reg'range => '0') then
                    pc_inc <= '1';
                else
                    pc_dec <= '1';
                end if;

                fsm_next_state <= END_DOWHILE_FIND_BRACKET;
            
            when END_DOWHILE_FIND_BRACKET =>
                DATA_EN <= '1';

                fsm_next_state <= END_DOWHILE_CHECK_CNT;


            when others =>
                pc_inc <= '1';

                fsm_next_state <= WAIT_A_SEC;
                



        end case;
    end process fsm;

end behavioral;

