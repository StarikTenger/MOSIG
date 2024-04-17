
-- 
-- Definition of  Oven_ctrl
-- 
--      Wed Apr 10 08:45:23 2024
--      
--      LeonardoSpectrum Level 3, 2017a.2
-- 

library IEEE;
use IEEE.STD_LOGIC_1164.all;
library c35_CORELIB;
use c35_CORELIB.vcomponents.all;

entity Oven_ctrl is
   port (
      reset : IN std_logic ;
      clk : IN std_logic ;
      Half_power : IN std_logic ;
      Full_power : IN std_logic ;
      Start : IN std_logic ;
      s30 : IN std_logic ;
      s60 : IN std_logic ;
      s120 : IN std_logic ;
      Time_set : IN std_logic ;
      Door_open : IN std_logic ;
      Timeout : IN std_logic ;
      Full : OUT std_logic ;
      Half : OUT std_logic ;
      In_light : OUT std_logic ;
      Finished : OUT std_logic ;
      Start_count : OUT std_logic ;
      Stop_count : OUT std_logic) ;
end Oven_ctrl ;

architecture Behavioral_onehot of Oven_ctrl is
   procedure DFFPC (
      constant data   : in std_logic;
      constant preset : in std_logic;
      constant clear  : in std_logic;
      signal clk      : in std_logic;
      signal q        : out std_logic)
   is begin
       if (preset = '1') then
           q <= '1' ;
       elsif (clear = '1') then
           q <= '0' ;
       elsif (clk'event and clk'last_value = '0' and clk = '1') then
           q <= data and data ;    -- takes care of q<='X' if data='Z'
       end if ;
       if ((clear/='1' or preset/='1') and clk/='0' and clk/='1') then
           q <= 'X' ;
       end if ;
   end DFFPC ;
   component select_8_8
      port (
         a : IN std_logic_vector (7 DOWNTO 0) ;
         b : IN std_logic_vector (7 DOWNTO 0) ;
         d : OUT std_logic) ;
   end component ;
   component select_3_3
      port (
         a : IN std_logic_vector (2 DOWNTO 0) ;
         b : IN std_logic_vector (2 DOWNTO 0) ;
         d : OUT std_logic) ;
   end component ;
   signal current_state_7, current_state_6, current_state_5, current_state_4, 
      current_state_3, current_state_2, current_state_1, current_state_0, 
      next_state_7, next_state_6, next_state_5, next_state_4, next_state_3, 
      next_state_2, next_state_1, next_state_0, PWR, GND0, nx84, nx122, 
      nx134, nx136, nx162, NOT_Door_open, nx198, nx200, nx228, nx274, nx322, 
      nx324, Start_count_EXMPLR, nx328, nx330, nx332, Stop_count_EXMPLR, 
      nx340, nx342, nx344, nx346, nx352, nx354, nx356, nx358, nx364, nx366, 
      nx387, nx402, nx418, nx427, nx428, nx432, nx447, nx461, NOT_nx387, 
      NOT_nx402, NOT_nx418, NOT_nx432, NOT_nx447, NOT_nx461, NOT_Time_set, 
      NOT_Full_power, NOT_Half_power: std_logic ;

begin
   Start_count <= Start_count_EXMPLR ;
   Stop_count <= Stop_count_EXMPLR ;
   PWR <= '1' ;
   GND0 <= '0' ;
   DFFPC (next_state_7,GND0,reset,clk,current_state_7) ;
   DFFPC (next_state_6,GND0,reset,clk,current_state_6) ;
   DFFPC (next_state_5,GND0,reset,clk,current_state_5) ;
   DFFPC (next_state_4,GND0,reset,clk,current_state_4) ;
   DFFPC (next_state_3,GND0,reset,clk,current_state_3) ;
   DFFPC (next_state_2,GND0,reset,clk,current_state_2) ;
   DFFPC (next_state_1,GND0,reset,clk,current_state_1) ;
   DFFPC (next_state_0,reset,GND0,clk,current_state_0) ;
   nx134 <= s30 OR s60 ;
   nx136 <= nx134 OR s120 ;
   NOT_Door_open <= NOT Door_open ;
   nx427 <= Time_set AND NOT_Door_open ;
   nx428 <= Time_set AND Door_open ;
   modgen_select_0 : select_8_8 port map ( a(7)=>current_state_0, a(6)=>
      current_state_1, a(5)=>current_state_2, a(4)=>current_state_3, a(3)=>
      current_state_4, a(2)=>current_state_5, a(1)=>current_state_6, a(0)=>
      current_state_7, b(7)=>GND0, b(6)=>GND0, b(5)=>GND0, b(4)=>GND0, b(3)
      =>GND0, b(2)=>GND0, b(1)=>nx274, b(0)=>NOT_Door_open, d=>next_state_7
   );
   modgen_select_1 : select_8_8 port map ( a(7)=>current_state_0, a(6)=>
      current_state_1, a(5)=>current_state_2, a(4)=>current_state_3, a(3)=>
      current_state_4, a(2)=>current_state_5, a(1)=>current_state_6, a(0)=>
      current_state_7, b(7)=>GND0, b(6)=>GND0, b(5)=>GND0, b(4)=>GND0, b(3)
      =>nx228, b(2)=>GND0, b(1)=>NOT_nx461, b(0)=>GND0, d=>next_state_6);
   modgen_select_2 : select_8_8 port map ( a(7)=>current_state_0, a(6)=>
      current_state_1, a(5)=>current_state_2, a(4)=>current_state_3, a(3)=>
      current_state_4, a(2)=>current_state_5, a(1)=>current_state_6, a(0)=>
      current_state_7, b(7)=>GND0, b(6)=>GND0, b(5)=>GND0, b(4)=>nx198, b(3)
      =>Door_open, b(2)=>Door_open, b(1)=>Door_open, b(0)=>GND0, d=>
      next_state_5);
   modgen_select_3 : select_8_8 port map ( a(7)=>current_state_0, a(6)=>
      current_state_1, a(5)=>current_state_2, a(4)=>current_state_3, a(3)=>
      current_state_4, a(2)=>current_state_5, a(1)=>current_state_6, a(0)=>
      current_state_7, b(7)=>GND0, b(6)=>GND0, b(5)=>GND0, b(4)=>nx200, b(3)
      =>NOT_nx447, b(2)=>NOT_Door_open, b(1)=>GND0, b(0)=>GND0, d=>
      next_state_4);
   modgen_select_4 : select_8_8 port map ( a(7)=>current_state_0, a(6)=>
      current_state_1, a(5)=>current_state_2, a(4)=>current_state_3, a(3)=>
      current_state_4, a(2)=>current_state_5, a(1)=>current_state_6, a(0)=>
      current_state_7, b(7)=>GND0, b(6)=>nx122, b(5)=>nx162, b(4)=>NOT_nx432, 
      b(3)=>GND0, b(2)=>GND0, b(1)=>GND0, b(0)=>GND0, d=>next_state_3);
   modgen_select_5 : select_8_8 port map ( a(7)=>current_state_0, a(6)=>
      current_state_1, a(5)=>current_state_2, a(4)=>current_state_3, a(3)=>
      current_state_4, a(2)=>current_state_5, a(1)=>current_state_6, a(0)=>
      current_state_7, b(7)=>nx84, b(6)=>Half_power, b(5)=>NOT_nx418, b(4)=>
      GND0, b(3)=>GND0, b(2)=>GND0, b(1)=>GND0, b(0)=>GND0, d=>next_state_2
   );
   modgen_select_6 : select_8_8 port map ( a(7)=>current_state_0, a(6)=>
      current_state_1, a(5)=>current_state_2, a(4)=>current_state_3, a(3)=>
      current_state_4, a(2)=>current_state_5, a(1)=>current_state_6, a(0)=>
      current_state_7, b(7)=>Full_power, b(6)=>NOT_nx402, b(5)=>Full_power, 
      b(4)=>GND0, b(3)=>GND0, b(2)=>GND0, b(1)=>GND0, b(0)=>GND0, d=>
      next_state_1);
   modgen_select_7 : select_8_8 port map ( a(7)=>current_state_0, a(6)=>
      current_state_1, a(5)=>current_state_2, a(4)=>current_state_3, a(3)=>
      current_state_4, a(2)=>current_state_5, a(1)=>current_state_6, a(0)=>
      current_state_7, b(7)=>NOT_nx387, b(6)=>GND0, b(5)=>GND0, b(4)=>GND0, 
      b(3)=>GND0, b(2)=>GND0, b(1)=>GND0, b(0)=>Door_open, d=>next_state_0);
   nx322 <= current_state_3 AND Time_set ;
   nx324 <= nx322 AND Door_open ;
   Start_count_EXMPLR <= current_state_4 AND Start ;
   nx328 <= nx324 OR Start_count_EXMPLR ;
   nx330 <= current_state_4 AND Door_open ;
   nx332 <= nx328 OR nx330 ;
   Stop_count_EXMPLR <= current_state_6 AND Door_open ;
   In_light <= nx332 OR Stop_count_EXMPLR ;
   nx340 <= current_state_0 AND Full_power ;
   nx342 <= current_state_2 AND Full_power ;
   nx344 <= nx340 OR nx342 ;
   nx346 <= current_state_1 AND Full_power ;
   Full <= nx344 OR nx346 ;
   nx352 <= current_state_0 AND Half_power ;
   nx354 <= current_state_2 AND Half_power ;
   nx356 <= nx352 OR nx354 ;
   nx358 <= current_state_1 AND Half_power ;
   Half <= nx356 OR nx358 ;
   nx364 <= current_state_6 AND Timeout ;
   nx366 <= current_state_7 AND NOT_Door_open ;
   Finished <= nx364 OR nx366 ;
   nx387 <= Full_power OR Half_power ;
   nx402 <= Half_power OR nx136 ;
   nx418 <= Full_power OR nx136 ;
   modgen_select_18 : select_3_3 port map ( a(2)=>nx427, a(1)=>nx428, a(0)=>
      NOT_Time_set, b(2)=>GND0, b(1)=>PWR, b(0)=>GND0, d=>nx198);
   modgen_select_19 : select_3_3 port map ( a(2)=>nx427, a(1)=>nx428, a(0)=>
      NOT_Time_set, b(2)=>PWR, b(1)=>GND0, b(0)=>GND0, d=>nx200);
   nx432 <= nx427 OR nx428 ;
   nx447 <= Door_open OR Start ;
   nx461 <= Door_open OR Timeout ;
   NOT_nx387 <= NOT nx387 ;
   NOT_nx402 <= NOT nx402 ;
   NOT_nx418 <= NOT nx418 ;
   NOT_nx432 <= NOT nx432 ;
   NOT_nx447 <= NOT nx447 ;
   NOT_nx461 <= NOT nx461 ;
   NOT_Time_set <= NOT Time_set ;
   NOT_Full_power <= NOT Full_power ;
   nx84 <= Half_power AND NOT_Full_power ;
   NOT_Half_power <= NOT Half_power ;
   nx122 <= nx136 AND NOT_Half_power ;
   nx162 <= nx136 AND NOT_Full_power ;
   nx228 <= Start AND NOT_Door_open ;
   nx274 <= Timeout AND NOT_Door_open ;
end Behavioral_onehot ;

