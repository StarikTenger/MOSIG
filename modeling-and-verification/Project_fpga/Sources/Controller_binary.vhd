
-- 
-- Definition of  Oven_ctrl
-- 
--      Wed Apr 10 08:41:12 2024
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

architecture Behavioral_binary of Oven_ctrl is
   signal current_state_1, current_state_2, nx2, current_state_0, nx8, nx445, 
      nx446, nx34, nx44, nx50, nx447, nx60, nx74, nx92, nx122, nx144, nx459, 
      nx462, nx464, nx468, nx470, nx472, nx474, nx476, nx478, nx481, nx484, 
      nx486, nx488, nx491, nx493, nx495, nx497, nx500, nx505, nx508, nx511, 
      nx514, nx516, nx519, nx522, nx524, nx526, nx528, nx530, nx534: 
   std_logic ;

begin
   ix165 : OAI221 port map ( Q=>Finished, A=>nx447, B=>nx462, C=>nx446, D=>
      nx511);
   ix85 : NAND21 port map ( Q=>nx447, A=>current_state_0, B=>current_state_1
   );
   ix75 : OAI211 port map ( Q=>nx74, A=>Half_power, B=>nx8, C=>nx493);
   ix9 : NAND31 port map ( Q=>nx8, A=>nx459, B=>nx468, C=>current_state_0);
   ix145 : OAI2111 port map ( Q=>nx144, A=>nx462, B=>nx478, C=>nx481, D=>
      nx488);
   ix463 : NAND21 port map ( Q=>nx462, A=>nx464, B=>current_state_2);
   ix465 : CLKIN1 port map ( Q=>nx464, A=>Door_open);
   ix99 : OAI2111 port map ( Q=>nx445, A=>nx468, B=>nx472, C=>nx476, D=>
      nx462);
   reg_current_state_2 : DFC1 port map ( Q=>current_state_2, QN=>nx468, C=>
      clk, D=>nx445, RN=>nx470);
   ix471 : CLKIN1 port map ( Q=>nx470, A=>reset);
   ix473 : NOR21 port map ( Q=>nx472, A=>nx474, B=>nx459);
   reg_current_state_0 : DFC1 port map ( Q=>current_state_0, QN=>nx474, C=>
      clk, D=>nx74, RN=>nx470);
   ix477 : NAND31 port map ( Q=>nx476, A=>nx472, B=>Time_set, C=>nx468);
   ix479 : AOI211 port map ( Q=>nx478, A=>Start, B=>nx474, C=>
      current_state_1);
   reg_current_state_1 : DFC1 port map ( Q=>current_state_1, QN=>nx459, C=>
      clk, D=>nx144, RN=>nx470);
   ix482 : OAI211 port map ( Q=>nx481, A=>nx44, B=>Half_power, C=>nx486);
   ix45 : CLKIN1 port map ( Q=>nx44, A=>nx484);
   ix485 : NOR31 port map ( Q=>nx484, A=>s60, B=>s120, C=>s30);
   ix487 : NOR31 port map ( Q=>nx486, A=>current_state_1, B=>current_state_2, 
      C=>nx474);
   ix489 : NAND21 port map ( Q=>nx488, A=>nx468, B=>nx122);
   ix123 : OAI221 port map ( Q=>nx122, A=>Full_power, B=>nx491, C=>Time_set, 
      D=>nx447);
   ix492 : OAI211 port map ( Q=>nx491, A=>Half_power, B=>current_state_1, C
      =>nx474);
   ix494 : MUX22 port map ( Q=>nx493, A=>nx495, B=>nx500, S=>nx505);
   ix496 : AOI2111 port map ( Q=>nx495, A=>Timeout, B=>nx497, C=>nx60, D=>
      Door_open);
   ix498 : NOR21 port map ( Q=>nx497, A=>current_state_0, B=>nx459);
   ix61 : NOR40 port map ( Q=>nx60, A=>nx474, B=>nx459, C=>Door_open, D=>
      nx468);
   ix501 : AOI211 port map ( Q=>nx500, A=>Full_power, B=>nx474, C=>nx50);
   ix51 : OAI221 port map ( Q=>nx50, A=>current_state_2, B=>nx34, C=>nx484, 
      D=>nx446);
   ix35 : AOI211 port map ( Q=>nx34, A=>current_state_0, B=>current_state_1, 
      C=>current_state_2);
   ix153 : NAND21 port map ( Q=>nx446, A=>nx474, B=>current_state_1);
   ix506 : AOI2111 port map ( Q=>nx505, A=>current_state_2, B=>nx447, C=>
      nx92, D=>nx2);
   ix93 : NOR40 port map ( Q=>nx92, A=>nx474, B=>nx459, C=>nx508, D=>
      current_state_2);
   ix509 : CLKIN1 port map ( Q=>nx508, A=>Time_set);
   ix3 : NOR21 port map ( Q=>nx2, A=>Door_open, B=>nx468);
   ix512 : NAND21 port map ( Q=>nx511, A=>Timeout, B=>current_state_2);
   ix167 : NOR21 port map ( Q=>Half, A=>nx514, B=>nx516);
   ix515 : CLKIN1 port map ( Q=>nx514, A=>Half_power);
   ix517 : OAI211 port map ( Q=>nx516, A=>nx474, B=>nx459, C=>nx468);
   ix169 : NOR21 port map ( Q=>Full, A=>nx519, B=>nx516);
   ix520 : CLKIN1 port map ( Q=>nx519, A=>Full_power);
   ix195 : OAI2111 port map ( Q=>In_light, A=>nx522, B=>nx526, C=>nx528, D=>
      nx530);
   ix523 : AOI211 port map ( Q=>nx522, A=>current_state_1, B=>
      current_state_0, C=>nx524);
   ix525 : NOR21 port map ( Q=>nx524, A=>current_state_0, B=>current_state_1
   );
   ix527 : NAND21 port map ( Q=>nx526, A=>Door_open, B=>nx445);
   ix529 : NAND31 port map ( Q=>nx528, A=>nx497, B=>Door_open, C=>
      current_state_2);
   ix531 : NAND31 port map ( Q=>nx530, A=>nx524, B=>Start, C=>
      current_state_2);
   ix181 : NOR31 port map ( Q=>Stop_count, A=>nx446, B=>nx464, C=>nx468);
   ix175 : NOR40 port map ( Q=>Start_count, A=>current_state_0, B=>
      current_state_1, C=>nx534, D=>nx468);
   ix535 : CLKIN1 port map ( Q=>nx534, A=>Start);
end Behavioral_binary ;

