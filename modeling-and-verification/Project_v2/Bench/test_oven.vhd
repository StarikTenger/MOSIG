library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Testbench is
end Testbench;

architecture Behavioral of Testbench is
    constant CLK_PERIOD : time := 10 ns;  -- Clock period
    signal reset, clk, 
    Half_power, Full_power,
    Start, 
    s30, s60, s120,
	Time_set, Door_open, 
    Timeout 
    : std_logic;
    
	signal Full, Half, In_light, 
    Finished,

	Full1, Half1, In_light1, 
	    Finished1
    : std_logic;

    component Oven is 
    	Port (
          reset, clk, 
	  Half_power, Full_power, 
	  Start, s30, s60, s120,
	  Time_set, Door_open
	  : in std_logic;
	  
	  Full, Half, In_light, Finished
	  : out std_logic
        );
	end component;

	component Oven1 is 
    	Port (
          reset, clk, 
	  Half_power, Full_power, 
	  Start, s30, s60, s120,
	  Time_set, Door_open
	  : in std_logic;
	  
	  Full, Half, In_light, Finished
	  : out std_logic
        );
	end component;
begin
    -- Instantiate the unit under test (UUT)
    uut: Oven
        port map (
            clk => clk,
            reset => reset,
            Half_power => Half_power,
            Full_power => Full_power,
            Start => Start,
            s30 => s30,
            s60 => s60,
            s120 => s120,
            Time_set => Time_set,
            Door_open => Door_open,
	    full => full,
		half => half,
		In_light => In_light,
 		Finished => Finished
        );

    uut1: Oven1
        port map (
            clk => clk,
            reset => reset,
            Half_power => Half_power,
            Full_power => Full_power,
            Start => Start,
            s30 => s30,
            s60 => s60,
            s120 => s120,
            Time_set => Time_set,
            Door_open => Door_open,
	    full => full1,
		half => half1,
		In_light => In_light1,
 		Finished => Finished1
        );

    -- Clock process
    clk_process: process
    begin
        while now < 5000 ns loop
            clk <= '0';
            wait for CLK_PERIOD / 2;
            clk <= '1';
            wait for CLK_PERIOD / 2;
        end loop;
        wait;
    end process;

    -- Stimulus process
    stimulus_process: process
    begin
        -- Reset sequence
	reset <= '0';
	wait for CLK_PERIOD;
        reset <= '1';
        wait for CLK_PERIOD;
        reset <= '0';
        wait for CLK_PERIOD / 4;

        -- Test sequence
        Half_power <= '0';
	Full_power <= '0';
	Start <= '0';
	s30 <= '1'; 
	s60 <= '0';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';
        wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '1';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';
	wait for CLK_PERIOD;
        Half_power <= '1';
	Full_power <= '0';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';
	wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '1';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';
        wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';
        wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '1'; 
	Door_open <= '0';
	wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '1';
	wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';
        wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '1';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';
	wait for CLK_PERIOD * 3;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '1';
	wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';
	wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '1';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';
	wait for CLK_PERIOD * 64;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '1';
wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';
wait for CLK_PERIOD;
        Half_power <= '1';
	Full_power <= '0';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';
wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '0';
	s30 <= '0'; 
	s60 <= '1';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '1';
wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '0';
	s30 <= '1'; 
	s60 <= '0';
	s120 <= '0';
	Time_set <= '1'; 
	Door_open <= '1';
wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '0';
	s30 <= '1'; 
	s60 <= '0';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';
wait for CLK_PERIOD;
        Half_power <= '0';
	Full_power <= '0';
	Start <= '1';
	s30 <= '1'; 
	s60 <= '0';
	s120 <= '0';
	Time_set <= '0'; 
	Door_open <= '0';

        -- End simulation
        wait;
    end process;

end Behavioral;

library LIB_CONTROLLER;
library LIB_CONTROLLER_BENCH;

configuration config_onehot of LIB_CONTROLLER_BENCH.Testbench is 
    for Behavioral 
       --for uut:Oven use entity LIB_CONTROLLER.Oven(Oven_Arch); end for;
       for uut:Oven use configuration LIB_CONTROLLER.config1; end for;
	for uut1:Oven1 use configuration LIB_CONTROLLER.config_onehot;
       end for;
    end for; 
end config_onehot;

library LIB_CONTROLLER;
library LIB_CONTROLLER_BENCH;

configuration config_binary of LIB_CONTROLLER_BENCH.Testbench is 
    for Behavioral 
       --for uut:Oven use entity LIB_CONTROLLER.Oven(Oven_Arch); end for;
       for uut:Oven use configuration LIB_CONTROLLER.config1; end for;
	for uut1:Oven1 use configuration LIB_CONTROLLER.config_binary;
       end for;
    end for; 
end config_binary;

library LIB_CONTROLLER;
library LIB_CONTROLLER_BENCH;

configuration config_gray of LIB_CONTROLLER_BENCH.Testbench is 
    for Behavioral 
       --for uut:Oven use entity LIB_CONTROLLER.Oven(Oven_Arch); end for;
       for uut:Oven use configuration LIB_CONTROLLER.config1; end for;
	for uut1:Oven1 use configuration LIB_CONTROLLER.config_gray;
       end for;
    end for; 
end config_gray;




