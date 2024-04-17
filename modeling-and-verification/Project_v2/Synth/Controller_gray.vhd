
-- 
-- Definition of  Oven_ctrl
-- 
--      Wed Apr 10 08:45:07 2024
--      
--      LeonardoSpectrum Level 3, 2017a.2
-- 

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity decoder_3 is
   port (
      data : IN std_logic_vector (2 DOWNTO 0) ;
      eq : OUT std_logic_vector (7 DOWNTO 0)) ;
end decoder_3 ;

architecture INTERFACE of decoder_3 is
   component eq_3u_3u
      port (
         a : IN std_logic_vector (2 DOWNTO 0) ;
         b : IN std_logic_vector (2 DOWNTO 0) ;
         d : OUT std_logic) ;
   end component ;
   signal nx3, nx5: std_logic ;

begin
   nx3 <= '0' ;
   nx5 <= '1' ;
   ix7 : eq_3u_3u port map ( a(2)=>data(2), a(1)=>data(1), a(0)=>data(0), 
      b(2)=>nx5, b(1)=>nx5, b(0)=>nx5, d=>eq(7));
   ix9 : eq_3u_3u port map ( a(2)=>data(2), a(1)=>data(1), a(0)=>data(0), 
      b(2)=>nx5, b(1)=>nx5, b(0)=>nx3, d=>eq(6));
   ix11 : eq_3u_3u port map ( a(2)=>data(2), a(1)=>data(1), a(0)=>data(0), 
      b(2)=>nx5, b(1)=>nx3, b(0)=>nx5, d=>eq(5));
   ix13 : eq_3u_3u port map ( a(2)=>data(2), a(1)=>data(1), a(0)=>data(0), 
      b(2)=>nx5, b(1)=>nx3, b(0)=>nx3, d=>eq(4));
   ix15 : eq_3u_3u port map ( a(2)=>data(2), a(1)=>data(1), a(0)=>data(0), 
      b(2)=>nx3, b(1)=>nx5, b(0)=>nx5, d=>eq(3));
   ix17 : eq_3u_3u port map ( a(2)=>data(2), a(1)=>data(1), a(0)=>data(0), 
      b(2)=>nx3, b(1)=>nx5, b(0)=>nx3, d=>eq(2));
   ix19 : eq_3u_3u port map ( a(2)=>data(2), a(1)=>data(1), a(0)=>data(0), 
      b(2)=>nx3, b(1)=>nx3, b(0)=>nx5, d=>eq(1));
   ix21 : eq_3u_3u port map ( a(2)=>data(2), a(1)=>data(1), a(0)=>data(0), 
      b(2)=>nx3, b(1)=>nx3, b(0)=>nx3, d=>eq(0));
end INTERFACE ;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

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

architecture Behavioral of Oven_ctrl is
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
   component mux_8u_8u
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
   component decoder_3
      port (
         data : IN std_logic_vector (2 DOWNTO 0) ;
         eq : OUT std_logic_vector (7 DOWNTO 0)) ;
   end component ;
   signal current_state_2, current_state_1, current_state_0, next_state_2, 
      next_state_1, next_state_0, PWR, GND0, nx35, nx58, nx65, nx67, nx79, 
      NOT_Door_open, nx100, nx111, nx133, nx155, nx156, nx158, nx160, 
      Start_count_EXMPLR, nx163, nx166, nx168, Stop_count_EXMPLR, nx178, 
      nx181, nx183, nx186, nx192, nx193, nx195, nx196, nx198, nx200, nx201, 
      nx207, nx208, nx210, nx211, nx223, nx227, nx240, nx244, nx245, nx254, 
      NOT_Time_set, NOT_Full_power, NOT_nx46, NOT_Start, NOT_Timeout: 
   std_logic ;
   
   signal DANGLING : std_logic_vector (0 downto 0 );

begin
   Start_count <= Start_count_EXMPLR ;
   Stop_count <= Stop_count_EXMPLR ;
   PWR <= '1' ;
   GND0 <= '0' ;
   DFFPC (next_state_2,GND0,reset,clk,current_state_2) ;
   DFFPC (next_state_1,GND0,reset,clk,current_state_1) ;
   DFFPC (next_state_0,GND0,reset,clk,current_state_0) ;
   nx65 <= s30 OR s60 ;
   nx67 <= nx65 OR s120 ;
   NOT_Door_open <= NOT Door_open ;
   nx244 <= Time_set AND NOT_Door_open ;
   nx245 <= Time_set AND Door_open ;
   modgen_mux_8 : mux_8u_8u port map ( a(7)=>PWR, a(6)=>PWR, a(5)=>PWR, a(4)
      =>NOT_Door_open, a(3)=>GND0, a(2)=>nx240, a(1)=>GND0, a(0)=>GND0, b(7)
      =>GND0, b(6)=>GND0, b(5)=>GND0, b(4)=>GND0, b(3)=>GND0, b(2)=>
      current_state_2, b(1)=>current_state_1, b(0)=>current_state_0, d=>
      next_state_2);
   modgen_mux_9 : mux_8u_8u port map ( a(7)=>PWR, a(6)=>nx111, a(5)=>
      Door_open, a(4)=>GND0, a(3)=>NOT_Full_power, a(2)=>PWR, a(1)=>nx227, 
      a(0)=>nx35, b(7)=>GND0, b(6)=>GND0, b(5)=>GND0, b(4)=>GND0, b(3)=>GND0, 
      b(2)=>current_state_2, b(1)=>current_state_1, b(0)=>current_state_0, d
      =>next_state_1);
   modgen_mux_10 : mux_8u_8u port map ( a(7)=>Door_open, a(6)=>nx254, a(5)=>
      nx133, a(4)=>GND0, a(3)=>nx79, a(2)=>nx100, a(1)=>nx58, a(0)=>nx223, 
      b(7)=>GND0, b(6)=>GND0, b(5)=>GND0, b(4)=>GND0, b(3)=>GND0, b(2)=>
      current_state_2, b(1)=>current_state_1, b(0)=>current_state_0, d=>
      next_state_0);
   nx156 <= nx155 AND Time_set ;
   nx158 <= nx156 AND Door_open ;
   Start_count_EXMPLR <= nx160 AND Start ;
   nx163 <= nx158 OR Start_count_EXMPLR ;
   nx166 <= nx160 AND Door_open ;
   nx168 <= nx163 OR nx166 ;
   Stop_count_EXMPLR <= nx207 AND Door_open ;
   In_light <= nx168 OR Stop_count_EXMPLR ;
   nx178 <= nx192 AND Full_power ;
   nx181 <= nx195 AND Full_power ;
   nx183 <= nx178 OR nx181 ;
   nx186 <= nx200 AND Full_power ;
   Full <= nx183 OR nx186 ;
   nx193 <= nx192 AND Half_power ;
   nx196 <= nx195 AND Half_power ;
   nx198 <= nx193 OR nx196 ;
   nx201 <= nx200 AND Half_power ;
   Half <= nx198 OR nx201 ;
   nx208 <= nx207 AND Timeout ;
   nx211 <= nx210 AND NOT_Door_open ;
   Finished <= nx208 OR nx211 ;
   nx223 <= Full_power OR Half_power ;
   nx227 <= Half_power OR nx67 ;
   nx240 <= nx244 OR nx245 ;
   modgen_select_33 : select_3_3 port map ( a(2)=>nx244, a(1)=>nx245, a(0)=>
      NOT_Time_set, b(2)=>GND0, b(1)=>PWR, b(0)=>GND0, d=>nx100);
   nx254 <= Door_open OR Start ;
   NOT_Time_set <= NOT Time_set ;
   NOT_Full_power <= NOT Full_power ;
   nx35 <= Half_power AND NOT_Full_power ;
   NOT_nx46 <= NOT nx67 ;
   NOT_Start <= NOT Start ;
   NOT_Timeout <= NOT Timeout ;
   nx58 <= NOT_nx46 OR Half_power ;
   nx79 <= NOT_nx46 OR Full_power ;
   nx111 <= NOT_Start OR Door_open ;
   nx133 <= NOT_Timeout OR Door_open ;
   ix401 : decoder_3 port map ( data(2)=>current_state_2, data(1)=>
      current_state_1, data(0)=>current_state_0, eq(7)=>DANGLING(0), eq(6)=>
      nx160, eq(5)=>nx207, eq(4)=>nx210, eq(3)=>nx195, eq(2)=>nx155, eq(1)=>
      nx200, eq(0)=>nx192);
end Behavioral ;

