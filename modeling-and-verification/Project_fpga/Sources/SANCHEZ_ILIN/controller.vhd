library ieee;
use ieee.std_logic_1164.all;

entity Controller is
	port(Reset, Clk, Door_open, Half_Power, Full_Power, Start, Time_Set, s30, s60, s120, Timeout: in std_logic; 
	Start_Count, Stop_Count, Full, Half, In_Light, Finished: out std_logic);
end Controller;

architecture arch1 of Controller is
type States is (Idle, Full_Power_On, Half_Power_On, Set_Time, Operation_enabled, Operation_Disabled, Operating, Complete);
signal state, next_state: States;

begin
process(state, Door_open, Half_Power, Full_Power, Start, Time_Set, s30, s60, s120, Timeout) -- next state process
begin
	case state is 
			when idle => 
				if Full_power = '1' then
					next_state <= Full_power_on;	-- if full-power chosen, go to full_power state
				elsif Half_power = '1' then
					next_state <= Half_power_on;	-- else if half-power chosen, go to half_power state
				else
					next_state <= Idle;		-- else stay in idle
				end if;
			when Full_Power_On => 
				if Half_Power = '1' then
					next_state <= Half_Power_On;	-- if half-power chosen, switch to half_power state
				elsif s30 = '1' or s30 = '1' or s120 = '1' then
					next_state <= Set_Time;		-- else if one of the time button pressed
				else
					next_state <= Full_Power_On;
				end if;
			when Half_Power_On => 
				if Full_Power = '1' then
					next_state <= Full_Power_On;
				elsif s30 = '1' or s30 = '1' or s120 = '1' then
					next_state <= Set_Time;
				else
					next_state <= Half_Power_On;
				end if;
			when Set_time => 
				if Time_Set='1' and Door_Open='1' then -- if time set button pressed with opened door we disable the operation
					next_state <= Operation_Disabled;
				elsif Time_Set='1' and Door_Open='0' then -- if time set button pressed with opened door we enable the operation
					next_state <= Operation_Enabled;
				else
					next_state <= Set_Time;
				end if;
			when Operation_enabled =>
				if Door_Open='1' then -- disable on the opened door
					next_state <= Operation_Disabled;
				elsif Start='1' then -- if door is not opened and start pressed we cqn start
					next_state <= Operating;
				else
					next_state <= Operation_enabled;
		 		end if;
			when Operation_disabled => 
				if Door_Open = '0' then -- enable operation on door closing
					next_state <= Operation_Enabled;
				else
					next_state <= Operation_Disabled;
				end if;
			when Operating => 
				if timeout='1' then
					next_state <= Complete;				
				elsif Door_open = '1' then -- disable on the opened door					
					next_state <= Operation_Disabled;
				else
					next_state <= Operating;
				end if;
			when Complete => 
				if Door_Open = '1' then
					next_state <= Idle;
				else
					next_state <= Complete;
				end if;
		end case;
end process;

process(reset, clk)	-- clock process
begin
	if (reset ='1') then 
		state <= Idle;
	elsif (clk'event and clk ='1') then
		state <= next_state;
	end if;
end process;

process(state, Door_open, Half_Power, Full_Power, Start, Time_Set, s30, s60, s120, Timeout) -- output function process - probably not correct, not tested yet
begin
	if Full_Power = '1' then
			Full <= '1';
			Half <= '0';
		elsif Half_Power = '1' then
			Full <= '0';
			Half <= '1';
		end if;
	if (state = Idle) or (state = Full_Power_On) or (state = Half_power_On) or (state = set_time) then 
		Start_Count <= '0';
		Stop_Count <= '0';
		In_Light <= '0';
		Finished <= '0';
	elsif (state = operation_enabled) and door_open = '0' and start = '0' then
		start_count <= '1';
	elsif state = Operating and door_open = '1' then
		stop_count <= '1';
	end if;

end process;
end arch1;


