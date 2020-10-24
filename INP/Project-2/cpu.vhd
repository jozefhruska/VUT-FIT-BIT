-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2018 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Jozef Hruška (xhrusk25)
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

   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti

   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='1') / zapis do pameti (DATA_RDWR='0')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti

   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice

   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	-- PC
	signal PC_INC, PC_DEC : std_logic;
	signal PC_DATA        : std_logic_vector(11 downto 0);

	-- CNT
	signal CNT_INC, CNT_DEC : std_logic;
	signal CNT_DATA         : std_logic_vector(8 downto 0);

	-- PTR
	signal PTR_INC, PTR_DEC : std_logic;
	signal PTR_DATA         : std_logic_vector(9 downto 0);

	-- MX
	signal MX_SEL : std_logic_vector(1 downto 0);

	-- Instruction types
	type INSTR_TYPE is (
		INSTR_PTR_INC,      -- '>' -> [0x3E] pointer increment
		INSTR_PTR_DEC,      -- '<' -> [0x3C] pointer decrement
		INSTR_VAL_INC,      -- '+' -> [0x2B] pointer value increment
		INSTR_VAL_DEC,      -- '-' -> [0x2B] pointer value decrement
		INSTR_WHILE_START,  -- '[' -> [0x5B] if pointer value is zero, jump to next ']'
		INSTR_WHILE_END,    -- ']' -> [0x5D] if pointer value is not zero, jump back to '['
		INSTR_PRINT,        -- '.' -> [0x2E] print pointer value
		INSTR_STORE,        -- '.' -> [0x2C] load & store value
		INSTR_COMMENT,      -- '#' -> [0x23] all chars after this and next '#' are ignored (block comment)

		INSTR_WRITE_0,      -- '0' -> [0x30] write '0' to pointer value
		INSTR_WRITE_1,      -- '1' -> [0x31] write '1' to pointer value
		INSTR_WRITE_2,      -- '2' -> [0x32] write '2' to pointer value
		INSTR_WRITE_3,      -- '3' -> [0x33] write '3' to pointer value
		INSTR_WRITE_4,      -- '4' -> [0x34] write '4' to pointer value
		INSTR_WRITE_5,      -- '5' -> [0x35] write '5' to pointer value
		INSTR_WRITE_6,      -- '6' -> [0x36] write '6' to pointer value
		INSTR_WRITE_7,      -- '7' -> [0x37] write '7' to pointer value
		INSTR_WRITE_8,      -- '8' -> [0x38] write '8' to pointer value
		INSTR_WRITE_9,      -- '9' -> [0x39] write '9' to pointer value

		INSTR_WRITE_A,      -- 'A' -> [0x41] write 'A' to pointer value
		INSTR_WRITE_B,      -- 'B' -> [0x42] write 'B' to pointer value
		INSTR_WRITE_C,      -- 'C' -> [0x43] write 'C' to pointer value
		INSTR_WRITE_D,      -- 'D' -> [0x44] write 'D' to pointer value
		INSTR_WRITE_E,      -- 'E' -> [0x45] write 'E' to pointer value
		INSTR_WRITE_F,      -- 'F' -> [0x46] write 'F' to pointer value

		INSTR_HALT,         -- 'null' -> [0x00] stop program
		INSTR_OTHER         -- All other instructions
	);

	-- Currently running instruction
	signal INSTR_CURRENT : INSTR_TYPE;

	-- FSM states
	type FSM_STATE is (
		STATE_IDLE,
		STATE_DECODE,
		STATE_PTR_INC,
		STATE_PTR_DEC,
		STATE_VAL_INC_READ,
		STATE_VAL_INC_WRITE,
		STATE_VAL_DEC_READ,
		STATE_VAL_DEC_WRITE,
		STATE_WHILE_START_1,
		STATE_WHILE_START_2,
		STATE_WHILE_START_3,
		STATE_WHILE_START_4,
		STATE_WHILE_END_1,
		STATE_WHILE_END_2,
		STATE_WHILE_END_3,
		STATE_WHILE_END_4,
		STATE_WHILE_END_5,
		STATE_PRINT_START,
		STATE_PRINT_END,
		STATE_STORE,
		STATE_COMMENT_1,
		STATE_COMMENT_2,
		STATE_COMMENT_3,
		STATE_WRITE,
		STATE_HALT,
		STATE_OTHER
	);

	-- State signals
	signal CURRENT_STATE, NEXT_STATE : FSM_STATE;

	-- Temporary data
	signal TEMP_DATA : std_logic_vector(7 downto 0);

begin
	-- PC PROCESS
	PC_PROCESS : process(RESET, CLK)
	begin
		if (RESET = '1') then PC_DATA <= (others => '0');
		elsif rising_edge(CLK) then
			if (PC_INC = '1') then PC_DATA <= PC_DATA + 1;
			elsif (PC_DEC = '1') then PC_DATA <= PC_DATA - 1;
			end if;
		end if;
		CODE_ADDR <= PC_DATA;
	end process PC_PROCESS;

	-- CNT PROCESS
	CNT_PROCESS : process(RESET, CLK)
	begin
		if (RESET = '1') then CNT_DATA <= (others => '0');
		elsif rising_edge(CLK) then
			if (CNT_INC = '1') then CNT_DATA <= CNT_DATA + 1;
			elsif (CNT_DEC = '1') then CNT_DATA <= CNT_DATA - 1;
			end if;
		end if;
		DATA_ADDR <= PTR_DATA;
	end process CNT_PROCESS;

	-- PTR PROCESS
	PTR_PROCESS : process(RESET, CLK)
	begin
		if (RESET = '1') then PTR_DATA <= (others => '0');
		elsif rising_edge(CLK) then
			if (PTR_INC = '1') then PTR_DATA <= PTR_DATA + 1;
			elsif (PTR_DEC = '1') then PTR_DATA <= PTR_DATA - 1;
			end if;
		end if;
	end process PTR_PROCESS;

	-- INSTRUCTION DECODER PROCESS
	INSTR_DECODER_PROCESS : process(CODE_DATA)
	begin
		case CODE_DATA is
			when X"3E" => INSTR_CURRENT <= INSTR_PTR_INC;
			when X"3C" => INSTR_CURRENT <= INSTR_PTR_DEC;
			when X"2B" => INSTR_CURRENT <= INSTR_VAL_INC;
			when X"2D" => INSTR_CURRENT <= INSTR_VAL_DEC;
			when X"5B" => INSTR_CURRENT <= INSTR_WHILE_START;
			when X"5D" => INSTR_CURRENT <= INSTR_WHILE_END;
			when X"2E" => INSTR_CURRENT <= INSTR_PRINT;
			when X"2C" => INSTR_CURRENT <= INSTR_STORE;
			when X"23" => INSTR_CURRENT <= INSTR_COMMENT;

			when X"30" => INSTR_CURRENT <= INSTR_WRITE_0;
			when X"31" => INSTR_CURRENT <= INSTR_WRITE_1;
			when X"32" => INSTR_CURRENT <= INSTR_WRITE_2;
			when X"33" => INSTR_CURRENT <= INSTR_WRITE_3;
			when X"34" => INSTR_CURRENT <= INSTR_WRITE_4;
			when X"35" => INSTR_CURRENT <= INSTR_WRITE_5;
			when X"36" => INSTR_CURRENT <= INSTR_WRITE_6;
			when X"37" => INSTR_CURRENT <= INSTR_WRITE_7;
			when X"38" => INSTR_CURRENT <= INSTR_WRITE_8;
			when X"39" => INSTR_CURRENT <= INSTR_WRITE_9;

			when X"41" => INSTR_CURRENT <= INSTR_WRITE_A;
			when X"42" => INSTR_CURRENT <= INSTR_WRITE_B;
			when X"43" => INSTR_CURRENT <= INSTR_WRITE_C;
			when X"44" => INSTR_CURRENT <= INSTR_WRITE_D;
			when X"45" => INSTR_CURRENT <= INSTR_WRITE_E;
			when X"46" => INSTR_CURRENT <= INSTR_WRITE_F;

			when X"00" => INSTR_CURRENT <= INSTR_HALT;
			when others => INSTR_CURRENT <= INSTR_OTHER;
		end case;
	end process INSTR_DECODER_PROCESS;

 -- MX SEL
 MX_SEL_PROCESS : process(DATA_RDATA, IN_DATA, MX_SEL)
	begin
		case MX_SEL is
			when "11" => DATA_WDATA <= TEMP_DATA;
			when "10" => DATA_WDATA <= DATA_RDATA - 1;
			when "01" => DATA_WDATA <= DATA_RDATA + 1;
			when "00" => DATA_WDATA <= IN_DATA;
			when others =>
		end case;
	end process MX_SEL_PROCESS;

	-- FSM CURRENT STATE PROCESS
	FSM_CURRENT_STATE_PROCESS : process(RESET, CLK)
	begin
		if (RESET = '1') then CURRENT_STATE <= STATE_IDLE;
		elsif rising_edge(CLK) and EN = '1' then
			CURRENT_STATE <= NEXT_STATE;
		end if;
	end process FSM_CURRENT_STATE_PROCESS;

	-- FSM NEXT STATE PROCESS
	FSM_NEXT_STATE_PROCESS : process(RESET, CLK, EN, IN_VLD, IN_DATA, DATA_RDATA, OUT_BUSY, CURRENT_STATE, CNT_DATA, INSTR_CURRENT, MX_SEL)
	begin

		NEXT_STATE   <= STATE_IDLE;
		CODE_EN      <= '1';
		DATA_EN      <= '0';
		DATA_RDWR    <= '0';
		IN_REQ       <= '0';
		OUT_WE       <= '0';
		PC_INC       <= '0';
		PC_DEC       <= '0';
		CNT_INC      <= '0';
		CNT_DEC      <= '0';
		PTR_INC      <= '0';
		PTR_DEC      <= '0';
		MX_SEL       <= "00";

		case CURRENT_STATE is

			-- STATE_IDLE
			when STATE_IDLE =>
				CODE_EN <= '1';
				NEXT_STATE <= STATE_DECODE;

			-- STATE_DECODE
			when STATE_DECODE =>
				case INSTR_CURRENT is
					when INSTR_PTR_INC => NEXT_STATE <= STATE_PTR_INC;
					when INSTR_PTR_DEC => NEXT_STATE <= STATE_PTR_DEC;
					when INSTR_VAL_INC => NEXT_STATE <= STATE_VAL_INC_READ;
					when INSTR_VAL_DEC => NEXT_STATE <= STATE_VAL_DEC_READ;
					when INSTR_WHILE_START => NEXT_STATE <= STATE_WHILE_START_1;
					when INSTR_WHILE_END => NEXT_STATE <= STATE_WHILE_END_1;
					when INSTR_PRINT => NEXT_STATE <= STATE_PRINT_START;
					when INSTR_STORE => NEXT_STATE <= STATE_STORE;
					when INSTR_COMMENT => NEXT_STATE <= STATE_COMMENT_1;

					when INSTR_WRITE_0 | INSTR_WRITE_1 | INSTR_WRITE_2 | INSTR_WRITE_3 |
					     INSTR_WRITE_4 | INSTR_WRITE_5 | INSTR_WRITE_6 | INSTR_WRITE_7 |
							 INSTR_WRITE_8 | INSTR_WRITE_9 | INSTR_WRITE_A | INSTR_WRITE_B |
							 INSTR_WRITE_C | INSTR_WRITE_D | INSTR_WRITE_E | INSTR_WRITE_F =>
						NEXT_STATE <= STATE_WRITE;

					when INSTR_HALT => NEXT_STATE <= STATE_HALT;
					when others => NEXT_STATE <= STATE_OTHER;
				end case;

			-- STATE_PTR_INC
			when STATE_PTR_INC =>
				NEXT_STATE <= STATE_IDLE;
				PTR_INC <= '1';
				PC_INC <= '1';

			-- STATE_PTR_DEC
			when STATE_PTR_DEC =>
				NEXT_STATE <= STATE_IDLE;
				PTR_DEC <= '1';
				PC_INC <= '1';

			-- STATE_VAL_INC_READ
			when STATE_VAL_INC_READ =>
				NEXT_STATE <= STATE_VAL_INC_WRITE;
				DATA_EN <= '1';
				DATA_RDWR <= '1';

			-- STATE_VAL_INC_WRITE
			when STATE_VAL_INC_WRITE =>
				NEXT_STATE <= STATE_IDLE;
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				PC_INC <= '1';
				MX_SEL <= "01";

			-- STATE_VAL_DEC_READ
			when STATE_VAL_DEC_READ =>
				NEXT_STATE <= STATE_VAL_DEC_WRITE;
				DATA_EN <= '1';
				DATA_RDWR <= '1';

			-- STATE_VAL_DEC_WRITE
			when STATE_VAL_DEC_WRITE =>
				NEXT_STATE <= STATE_IDLE;
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				PC_INC <= '1';
				MX_SEL <= "10";

			-- STATE_WHILE_START_1
			when STATE_WHILE_START_1 =>
				NEXT_STATE <= STATE_WHILE_START_2;
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				PC_INC <= '1';

			-- STATE_WHILE_START_2
			when STATE_WHILE_START_2 =>
				if DATA_RDATA = "00000000" then
					NEXT_STATE <= STATE_WHILE_START_3;
					CNT_INC <= '1';
				else
					NEXT_STATE <= STATE_IDLE;
				end if;

			-- STATE_WHILE_START_3
			when STATE_WHILE_START_3 =>
				if CNT_DATA = "00000000" then
					NEXT_STATE <= STATE_IDLE;
				else
					NEXT_STATE <= STATE_WHILE_START_4;
					CODE_EN <= '1';
				end if;

			-- STATE_WHILE_START_4
			when STATE_WHILE_START_4 =>
				if CODE_DATA = X"5D" then
					CNT_DEC <= '1';
				elsif CODE_DATA = X"5B" then
					CNT_INC <= '1';
				end if;
				NEXT_STATE <= STATE_WHILE_START_3;
				PC_INC <= '1';

			-- STATE_WHILE_END_1
			when STATE_WHILE_END_1 =>
				NEXT_STATE <= STATE_WHILE_END_2;
				DATA_RDWR <= '1';
				DATA_EN <= '1';

			-- STATE_WHILE_END_2
			when STATE_WHILE_END_2 =>
				if DATA_RDATA = "00000000" then
					NEXT_STATE <= STATE_IDLE;
					PC_INC <= '1';
				else
					NEXT_STATE <= STATE_WHILE_END_3;
					CNT_INC <= '1';
					PC_DEC <= '1';
				end if;

			-- STATE_WHILE_END_3
			when STATE_WHILE_END_3 =>
				if CNT_DATA = "00000000" then
					NEXT_STATE <= STATE_IDLE;
				else
					NEXT_STATE <= STATE_WHILE_END_4;
					CODE_EN <= '1';
				end if;

			-- STATE_WHILE_END_4
			when STATE_WHILE_END_4 =>
				if CODE_DATA = X"5B" then
					CNT_DEC <= '1';
				elsif CODE_DATA = X"5D" then
					CNT_INC <= '1';
				end if;
				NEXT_STATE <= STATE_WHILE_END_5;

			-- STATE_WHILE_END_5
			when STATE_WHILE_END_5 =>
				if CNT_DATA = "00000000" then
					PC_INC <= '1';
				else
					PC_DEC <= '1';
				end if;
				NEXT_STATE <= STATE_WHILE_END_3;

			-- STATE_PRINT_START
			when STATE_PRINT_START =>
				if OUT_BUSY = '1' then
					NEXT_STATE <= STATE_PRINT_START;
				else
					DATA_EN <= '1';
					DATA_RDWR <= '1';
					NEXT_STATE <= STATE_PRINT_END;
				end if;

			-- STATE_STORE
			when STATE_STORE =>
				IN_REQ <= '1';
				if IN_VLD = '1' then
					NEXT_STATE <= STATE_IDLE;
					DATA_RDWR <= '0';
					DATA_EN <= '1';
					PC_INC <= '1';
					MX_SEL <= "00";
				else NEXT_STATE <= STATE_STORE;
				end if;

			-- STATE_PRINT_END
			when STATE_PRINT_END =>
				NEXT_STATE <= STATE_IDLE;
				OUT_WE <= '1';
				OUT_DATA <= DATA_RDATA;
				PC_INC <= '1';

			-- STATE_COMMENT_1
			when STATE_COMMENT_1 =>
				NEXT_STATE <= STATE_COMMENT_2;
				PC_INC <= '1';

			-- STATE_COMMENT_2
			when STATE_COMMENT_2 =>
				NEXT_STATE <= STATE_COMMENT_3;
				CODE_EN <= '1';

			-- STATE_COMMENT_3
			when STATE_COMMENT_3 =>
				if CODE_DATA = X"23" then
					NEXT_STATE <= STATE_IDLE;
					PC_INC <= '1';
				else NEXT_STATE <= STATE_COMMENT_1;
				end if;

			-- STATE_WRITE
			when STATE_WRITE =>
				NEXT_STATE <= STATE_IDLE;
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				PC_INC <= '1';
				MX_SEL <= "11";
				TEMP_DATA <= CODE_DATA(3 downto 0) & "0000";

			-- STATE_HALT
			when STATE_HALT =>
				NEXT_STATE <= STATE_HALT;

			-- STATE_OTHER
			when STATE_OTHER =>
				NEXT_STATE <= STATE_IDLE;
				PC_INC <= '1';

		end case;

	end process FSM_NEXT_STATE_PROCESS;

end behavioral;