library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Oven is
 port(
  reset, clk, 
  Half_power, Full_power, 
  Start, s30, s60, s120,
  Time_set, Door_open
  : in std_logic;
  
  Full, Half, In_light, Finished
  : out std_logic);
end Oven;

architecture Oven_Arch of Oven is
	component Oven_ctrl is 
    	Port (
            reset, clk, 
            Half_power, Full_power, 
            Start, 
            s30, s60, s120,
            Time_set, Door_open, 
            Timeout 
            : in std_logic;

            Full, Half, In_light, 
            Finished,
            Start_count, Stop_count 
            : out std_logic
        );
    end component;
    
    
    component Oven_count is 
    	Port (
            reset, clk, start, stop, s30, s60, s120 
            : in std_logic;

            aboveth 
            : out std_logic
        );
    end component;
    
    signal start_count, stop_count, timeout : std_logic;    
begin
	u1: Oven_ctrl port map (
    	reset => reset,
        clk => clk,
    	door_open => door_open,
        half_power => half_power,
        full_power => full_power,
        start => start,
        time_set => time_set,
        s30 => s30,
        s60 => s60,
        s120 => s120,
        full => full,
        half => half,
        in_light => in_light,
        finished => finished,
        start_count => start_count,
        stop_count => stop_count,
        timeout => timeout
    );
    
    u2: Oven_count port map (
		reset => reset,
        clk => clk,
        s30 => s30,
        s60 => s60,
        s120 => s120,
        start => start_count,
        stop => stop_count,
        aboveth => timeout
    );
end Oven_Arch;


library LIB_CONTROLLER;

configuration config1 of LIB_CONTROLLER.Oven is 
    for Oven_arch 
       for u1:Oven_ctrl use entity LIB_CONTROLLER.Oven_ctrl(Behavioral); end for;
	for u2:Oven_count use entity LIB_CONTROLLER.Oven_count(impl);
       end for;
    end for; 
end config1;
