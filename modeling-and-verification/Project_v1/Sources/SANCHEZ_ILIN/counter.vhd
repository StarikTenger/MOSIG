library ieee;
use ieee.std_logic_1164.all;

entity Oven_count is port(reset, clk, start, stop, s30, s60, s120: in std_logic; aboveth: out std_logic);
end Oven_count;

architecture impl of Oven_count is
type States is (idle, counting, stopped);
signal state, nextstate: States;
signal c, nextc: natural;
begin
  process (state, c, start, stop, s30, s60, s120)
  begin
    case state is
       when idle =>
          if start='1' then 
                nextstate <= counting;
                nextc <= c+1;
          else nextstate <= idle;
               nextc <= 0;
          end if;
       when counting =>
          if stop = '1' then 
               nextstate <= stopped;
               nextc <= c;
          elsif ((s30='1' and c >= 30) or (s60='1' and c >= 60) or 
                 (s120='1' and c >= 120)) then 
               nextstate <= idle;
               nextc <= 0;
		aboveth <= '1';
          else nextstate <= counting;
               nextc <= c+1;
          end if;
       when stopped =>
          if start='1' then 
               nextstate <= counting;
               nextc <= c+1;
          else nextstate <= stopped;
               nextc <= c;
          end if;
    end case;
  end process; 

  process (reset, clk)
  begin
    if (reset = '1') then state <= idle;
                          c <= 0;
    elsif (clk'event and clk='1') then
        state <= nextstate;
        c <= nextc;
    end if;
  end process;

  aboveth <= '0';
end impl;
