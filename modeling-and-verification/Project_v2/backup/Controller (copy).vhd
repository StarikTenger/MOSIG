
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Oven_ctrl is
port(
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
    : out std_logic);
end Oven_ctrl;



architecture Behavioral of Oven_ctrl is
    type state_type is (
    	idle, full_power_on, half_power_on, set_time, 
        operation_enabled, operation_disabled,
        operating, complete);
    signal current_state, next_state: state_type;
begin
    process (clk, reset)
    begin
        if reset = '1' then
            current_state <= idle;
        elsif rising_edge(clk) then
            current_state <= next_state;
        end if;
    end process;

    process (current_state, Door_open, Half_Power, Full_Power, Start, Time_Set, s30, s60, s120, Timeout)
    begin
        case current_state is
            when idle =>
                if full_power = '1' then
                    next_state <= full_power_on;
                elsif half_power = '1' then
                	next_state <= half_power_on;
                else
                    next_state <= idle;
                end if;
             when full_power_on =>
                if half_power = '1' then
                    next_state <= half_power_on;
                elsif s30 = '1' or s60 = '1' or s120 = '1' then
                	next_state <= set_time;
                else
                    next_state <= full_power_on;
                end if;
             when half_power_on =>
                if full_power = '1' then
                    next_state <= full_power_on;
                elsif s30 = '1' or s60 = '1' or s120 = '1' then
                	next_state <= set_time;
                else
                    next_state <= half_power_on;
                end if;
             when set_time =>
             	if time_set = '1' and door_open = '0' then
                	next_state <= operation_enabled;
                elsif time_set = '1' and door_open = '1' then
                	next_state <= operation_disabled;
                else
                	next_state <= set_time;
                end if;
             when operation_enabled =>
             	if door_open = '1' then
                	next_state <= operation_disabled;
                elsif start = '1' then
             		next_state <= operating;
                else
                	next_state <= operation_enabled;
                end if;
             when operation_disabled =>
             	if door_open = '0' then
                	next_state <= operation_enabled;
                else
                	next_state <= operation_disabled;
                end if;
			 when operating =>
             	if door_open = '1' then
                	next_state <= operation_disabled;
                elsif timeout = '1' then
             		next_state <= complete;
                else
                	next_state <= operating;
                end if;
             when complete =>
             	if door_open = '1' then
                	next_state <= idle;
                else
                	next_state <= complete;
                end if;
                    
                 
        end case;
    end process;

    process(current_state, Door_open, Half_Power, Full_Power, Start, Time_Set, Timeout) -- output function process - probably not correct, not tested yet
    begin
	 if (current_state = Operation_enabled and Start = '1')
		 then Start_count <= '1';
		 else Start_count <= '0';
	 end if;

	 if (current_state = Operating and Door_open = '1')
	 then Stop_count <= '1';
	 else Stop_count <= '0';
	 end if;

	 if ((current_state = Set_time and Time_set = '1' and Door_open = '1') 
	 OR (current_state = operation_enabled and Start = '1')
	 OR (current_state = operation_enabled and Door_open = '1')
	 OR (current_state = operating and Door_open = '1'))

	 then In_light <= '1';
	 else In_light <= '0';
	 end if;

	 if ((current_state = idle and Full_power = '1')
	 OR (current_state = Half_power_on and Full_power = '1')
	 OR (current_state = Full_power_on and Full_power = '1'))
	 then Full <= '1';
	 else Full <= '0';
	 end if;

	 if ((current_state = idle and Half_power = '1')
	 OR (current_state = Half_power_on and Half_power = '1')
	 OR (current_state = Full_power_on and Half_power = '1'))
	 then Half <= '1';
	 else Half <= '0';
	 end if;

	 if((current_state = operating and Timeout = '1')
	 OR (current_state = complete and Door_open = '0'))
	 then Finished <= '1';
	 else Finished <= '0';
	 end if;


    end process;


	--when if current_state = operation_enabled and Start ='1';
	--Stop_count <= '1  when current_state = Operating and Door_open = '1';
	--Inligh <= '1' when (current_state = Set_time and Time_set = '1' and Door_open = '1')
	--	OR (current_state = operation_enabled and start = '1')
	--	OR (current_state = operation_enabled and Door_open = '1' 

end Behavioral;
