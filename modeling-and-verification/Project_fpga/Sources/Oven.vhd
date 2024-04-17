library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Oven is
	port(reset, clk, Half_power, Full_power, Start, s30, s60, s120,
	Time_set, Door_open: in std_logic;
	Full, Half, In_light, Finished: out std_logic;
	LEDR : out std_logic_vector(7 downto 0); -- for red leds
	LEDG : out std_logic_vector(3 downto 0); -- for green leds
	seg_val_hex0: out INTEGER range 0 to 127; -- to display the state
	seg_val_hex1: out INTEGER range 0 to 127; -- to display the counter
	seg_val_hex2: out INTEGER range 0 to 127; -- to display the counter
	seg_val_hex3: out INTEGER range 0 to 127 -- to display the counter
	);
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
	
	attribute chip_pin : string;
	attribute chip_pin of reset : signal is "R22"; -- rightmost button
	attribute chip_pin of clk : signal is "L1"; -- 50 MHz internal quartz
	-- assignment of inputs to switches (here from left to right) :
	attribute chip_pin of Half_power: signal is "M2"; -- SW 7
	attribute chip_pin of Full_power : signal is "U11"; -- SW 6
	attribute chip_pin of Start : signal is "U12"; -- SW 5
	attribute chip_pin of s30 : signal is "W12"; -- SW 4 
	attribute chip_pin of s60 : signal is "V12"; -- SW 3 
	attribute chip_pin of s120 : signal is "M22"; -- SW 2 
	attribute chip_pin of Time_set : signal is "L21"; -- SW 1 
	attribute chip_pin of Door_open : signal is "L22"; -- SW 0 
	-- 7-segment displays :
	attribute chip_pin of seg_val_hex0 : signal is "J2,J1,H2,H1,F2,F1,E2";
	-- idem for seg_val_hex1 to seg_val_hex3, to be completed (see below)
	-- assignment of outputs LEDR and LEDG to leds (here from left to right) :
	attribute chip_pin of LEDR : signal is
	"U18,Y18,V19,T18,Y19,U19,R19,R20";
	-- idem for LEDG (to be completed)
	attribute chip_pin of LEDG : signal is
		"V21,V22,U21,U22";
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

	

	LEDR(7) <= Half_power;
	LEDR(6) <= Full_power;
	LEDR(5) <= Start;
	LEDR(4) <= s30;
	LEDR(3) <= s60;
	LEDR(2) <= s120;
	LEDR(1) <= Time_set;
	LEDR(0) <= Door_open;

	LEDG(3) <= Full;
	LEDG(2) <= Half;
	LEDG(1) <= In_light;
	LEDG(0) <= Finished;
end Oven_Arch;


library LIB_CONTROLLER;

configuration config1 of LIB_CONTROLLER.Oven is 
    for Oven_arch 
       for u1:Oven_ctrl use entity LIB_CONTROLLER.Oven_ctrl(Behavioral); end for;
	for u2:Oven_count use entity LIB_CONTROLLER.Oven_count(impl);
       end for;
    end for; 
end config1;


library LIB_CONTROLLER;

configuration config_binary of LIB_CONTROLLER.Oven is 
    for Oven_arch 
       for u1:Oven_ctrl use entity LIB_CONTROLLER.Oven_ctrl(Behavioral_binary); end for;
	for u2:Oven_count use entity LIB_CONTROLLER.Oven_count(impl);
       end for;
    end for; 
end config_binary;

library LIB_CONTROLLER;

configuration config_gray of LIB_CONTROLLER.Oven is 
    for Oven_arch 
       for u1:Oven_ctrl use entity LIB_CONTROLLER.Oven_ctrl(Behavioral_gray); end for;
	for u2:Oven_count use entity LIB_CONTROLLER.Oven_count(impl);
       end for;
    end for; 
end config_gray;

library LIB_CONTROLLER;

configuration config_onehot of LIB_CONTROLLER.Oven is 
    for Oven_arch 
       for u1:Oven_ctrl use entity LIB_CONTROLLER.Oven_ctrl(Behavioral_onehot); end for;
	for u2:Oven_count use entity LIB_CONTROLLER.Oven_count(impl);
       end for;
    end for; 
end config_onehot;

