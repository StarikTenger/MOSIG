library ieee;
use ieee.std_logic_1164.all;

entity ex2 is
	port(clk,a,b,c: in std_logic; z: out std_logic);
end ex2;

architecture rtl of ex2 is
signal s: std_logic; 
begin
	process
	begin
		wait until clk'event and clk='1';
		s <= a or b;
	end process;
	z <= s and c;
end rtl;

architecture rtl2 of ex2 is
signal s: std_logic;
begin
process
begin
wait until clk'event and clk='1';
s <= a or b;
z <= s and c;
end process;
end rtl2;


architecture rtl3 of ex2 is
begin
process
begin
wait until clk'event and clk='1';
z <= (a or b) and c;
end process;
end rtl3;
