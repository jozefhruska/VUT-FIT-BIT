library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

---------------- Entity -----------------
entity ledc8x8 is
	port (
		ROW : out std_logic_vector (0 to 7);
		LED : out std_logic_vector (0 to 7);
		RESET, SMCLK : in std_logic
	);
	end ledc8x8;

---------------- Architecture -----------------
architecture main of ledc8x8 is
	signal cols	: std_logic_vector (7 downto 0) := "00000000";
	signal rows	: std_logic_vector (7 downto 0) := "00000000";
	signal timer_show : std_logic_vector (7 downto 0) := "00000000";
	signal timer_delay : std_logic_vector (20 downto 0) := "000000000000000000000";
	signal state : std_logic_vector (1 downto 0) := "00";
	signal mux : std_logic := '1';
begin

	---------------- Timer - Show -----------------
	p_timer_show : process (SMCLK, RESET, timer_show)
	begin
		if RESET = '1' then
			timer_show <= "00000000";
		elsif SMCLK'event and SMCLK = '1' then
			timer_show <= timer_show + 1;

			if timer_show = "11111111" then
				mux <= '1';
			else
				mux <= '0';
			end if;
		end if;
	end process p_timer_show;

	---------------- Timer - Delay -----------------
	p_timer_delay : process (SMCLK, RESET, timer_show)
	begin
		if RESET = '1' then
			timer_delay <= "000000000000000000000";
		elsif SMCLK'event and SMCLK = '1' then
			timer_delay <= timer_delay + 1;

			if timer_delay = "111100000000000000000" then
				timer_delay <= "000000000000000000000";
				state <= state + 1;
			end if;
		end if;
	end process p_timer_delay;

	---------------- Rotation -----------------
	rotation : process (SMCLK, RESET, rows, mux)
	begin
		if RESET = '1' then
			rows <= "10000000";
		elsif (SMCLK'event and SMCLK = '1' and mux = '1') then
			rows <= rows(0) & rows(7 downto 1);
		end if;

		ROW <= rows;
	end process rotation;

	---------------- State Switch  -----------------
	led_switch : process (state)
	begin
		if state = "00" then
			case (rows) is
				when "10000000" => cols <= "10000001";
				when "01000000" => cols <= "11111101";
				when "00100000" => cols <= "11111101";
				when "00010000" => cols <= "11111101";
				when "00001000" => cols <= "11111101";
				when "00000100" => cols <= "10111101";
				when "00000010" => cols <= "10111101";
				when "00000001" => cols <= "10000001";
				when others => cols <= "00000000";
			end case;
		elsif state = "10" then
			case (rows) is
				when "10000000" => cols <= "10111101";
				when "01000000" => cols <= "10111101";
				when "00100000" => cols <= "10111101";
				when "00010000" => cols <= "10111101";
				when "00001000" => cols <= "10000001";
				when "00000100" => cols <= "10111101";
				when "00000010" => cols <= "10111101";
				when "00000001" => cols <= "10111101";
				when others => cols <= "00000000";
			end case;
		else
			cols <= "11111111";
		end if;
	end process led_switch;

	LED <= cols;
end main;
