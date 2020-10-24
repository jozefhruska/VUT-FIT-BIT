-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST1, TEST2, TEST3, TEST4, TEST5, TEST6, TEST_CROSS, TESTA8, TESTA9, TESTA_END, TESTB8, TESTB9, TESTB10, TESTB_END, STATE_WRONG, PRINT_MESSAGE_OK, PRINT_MESSAGE_WRONG, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
    when STATE_WRONG =>
        next_state <= STATE_WRONG;
        if (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
    when PRINT_MESSAGE_OK =>
        next_state <= PRINT_MESSAGE_OK;
        if (CNT_OF = '1') then
            next_state <= FINISH;
        end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
    when PRINT_MESSAGE_WRONG =>
        next_state <= PRINT_MESSAGE_WRONG;
        if (CNT_OF = '1') then
            next_state <= FINISH;
        end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
    when FINISH =>
        next_state <= FINISH;
        if (KEY(15) = '1') then
            next_state <= TEST1;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
    -- KEY TESTS
    -- - - - - - - - - - - - - - - - - - - - - - -
    when TEST1 =>
        next_state <= TEST1;
        if (KEY(8) = '1') then
            next_state <= TEST2;
        elsif (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2 =>
        next_state <= TEST2;
        if (KEY(4) = '1') then
            next_state <= TEST3;
        elsif (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
    when TEST3 =>
        next_state <= TEST3;
        if (KEY(3) = '1') then
            next_state <= TEST4;
        elsif (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
    when TEST4 =>
        next_state <= TEST4;
        if (KEY(6) = '1') then
            next_state <= TEST5;
        elsif (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
    when TEST5 =>
        next_state <= TEST5;
        if (KEY(7) = '1') then
            next_state <= TEST6;
        elsif (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
    when TEST6 =>
        next_state <= TEST6;
        if (KEY(6) = '1') then
            next_state <= TEST_CROSS;
        elsif (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
    -- CROSSROAD (TEST A / TEST B)
    -- - - - - - - - - - - - - - - - - - - - - - -
    when TEST_CROSS =>
        next_state <= TEST_CROSS;
        if (KEY(9) = '1') then
            next_state <= TESTA8;
        elsif (KEY(8) = '1') then
            next_state <= TESTB8;
        elsif (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
    -- TEST A
    -- - - - - - - - - - - - - - - - - - - - - - -
    when TESTA8 =>
        next_state <= TESTA8;
        if (KEY(0) = '1') then
            next_state <= TESTA9;
        elsif (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - -
    when TESTA9 =>
        next_state <= TESTA9;
        if (KEY(6) = '1') then
            next_state <= TESTA_END;
        elsif (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - -
    when TESTA_END =>
        next_state <= TESTA_END;
        if (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_OK;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
    -- TEST B
    -- - - - - - - - - - - - - - - - - - - - - - -
    when TESTB8 =>
        next_state <= TESTB8;
        if (KEY(4) = '1') then
            next_state <= TESTB9;
        elsif (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - -
    when TESTB9 =>
        next_state <= TESTB9;
        if (KEY(3) = '1') then
            next_state <= TESTB10;
        elsif (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - -
    when TESTB10 =>
        next_state <= TESTB10;
        if (KEY(2) = '1') then
            next_state <= TESTB_END;
        elsif (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_WRONG;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - -
    when TESTB_END =>
        next_state <= TESTB_END;
        if (KEY(15) = '1') then
            next_state <= PRINT_MESSAGE_OK;
        elsif (KEY(14 downto 0) /= "000000000000000") then
            next_state <= STATE_WRONG;
        end if;
    -- - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_OK =>
        FSM_CNT_CE     <= '1';
        FSM_MX_LCD     <= '1';
        FSM_LCD_WR     <= '1';
        FSM_MX_MEM     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_WRONG =>
        FSM_CNT_CE     <= '1';
        FSM_MX_LCD     <= '1';
        FSM_LCD_WR     <= '1';
        FSM_MX_MEM     <= '0';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
        if (KEY(15) = '1') then
            FSM_LCD_CLR    <= '1';
        end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
        if (KEY(14 downto 0) /= "000000000000000") then
            FSM_LCD_WR     <= '1';
        end if;
        if (KEY(15) = '1') then
            FSM_LCD_CLR    <= '1';
        end if;
   end case;
end process output_logic;

end architecture behavioral;

