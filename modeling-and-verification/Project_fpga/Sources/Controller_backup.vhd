library IEEE;
use IEEE.STD_LOGIC_1164.all;
library c35_CORELIB;
use c35_CORELIB.vcomponents.all;

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



-- 
-- Definition of  Oven_ctrl
-- 
--      Wed Apr 10 08:41:12 2024
--      
--      LeonardoSpectrum Level 3, 2017a.2
-- 

library c35_CORELIB;
use c35_CORELIB.vcomponents.all;


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

library c35_CORELIB;
use c35_CORELIB.vcomponents.all;

architecture Behavioral_gray of Oven_ctrl is
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
end Behavioral_gray ;

library c35_CORELIB;
use c35_CORELIB.vcomponents.all;

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

