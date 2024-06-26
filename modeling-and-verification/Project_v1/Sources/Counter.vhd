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
       	  if s30 = '1' then
          		nextc <= 30;
          elsif s60 = '1' then
          		nextc <= 60;
          elsif s120 = '1' then
          		nextc <= 120;
          elsif start='1' then 
                nextstate <= counting;
                if c > 0 then nextc <= c - 1; end if;
          else nextstate <= idle;
               nextc <= c;
          end if;
       when counting =>
          if stop = '1' then 
               nextstate <= stopped;
               nextc <= c;
          elsif c = 0 then 
               nextstate <= stopped;
               nextc <= 0;
          else nextstate <= counting;
               if c > 0 then nextc <= c - 1; end if;
          end if;
       when stopped =>
          if start='1' then 
               nextstate <= counting;
               if c > 0 then nextc <= c - 1; end if;
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

  aboveth <= '1' when state = stopped else '0';
end impl;
