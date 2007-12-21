--------------------------------------------------------------------------------
-- Company: 		 NCAR/EOL/RSF
-- Engineer:		 Tom Brimeyer
--
-- Create Date:    15:14:57 11/21/05
-- Design Name:    
-- Module Name:    Signal_Block - Behavioral
-- Project Name:   Radar_Timing
-- Target Device:  VIRTEX II
-- Tool versions:  
-- Description:	 Various parameter inputs which result in the output of 
--						 a radar timing pulse.
-- Dependencies:
-- 
-- Revision:		 0.01
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
--------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Signal_Block is
    Port ( 
	 		  --Clock Signals
			  CLK : in std_logic;

			  --Misc. Signals
			  ENABLE_L : in std_logic;
			  TRIGGER : in std_logic;
			  CNTL : in std_logic_vector(15 downto 0);
			  MULT : in std_logic_vector(15 downto 0);
           
			  --Transmit Pulse Signals
			  DELAY : in std_logic_vector(15 downto 0);
			  WIDTH : in std_logic_vector(15 downto 0);
			  PERIOD : in std_logic_vector(15 downto 0);
			  OFF_DELAY : in std_logic_vector(15 downto 0);
			  OFF_WIDTH : in std_logic_vector(15 downto 0);
			  OFF_PERIOD : in std_logic_vector(15 downto 0);			  			  
			  PULSE_OUT : out std_logic);
end Signal_Block;

architecture Behavioral of Signal_Block is

--Transmit Pulse Variables
signal delay_int : std_logic_vector(15 downto 0) := "0000000000000000";
signal width_int : std_logic_vector(15 downto 0) := "0000000000000000";
signal period_int : std_logic_vector(15 downto 0) := "1111111111111110";
signal start : std_logic := '1';
signal pulse : std_logic := CNTL(1);

--New Signals
signal off_delay_int : std_logic_vector(15 downto 0) := "0000000000000000";
signal off_width_int : std_logic_vector(15 downto 0) := "0000000000000000";
signal off_period_int : std_logic_vector(15 downto 0)	:= "1111111111111110";
signal off_flag : std_logic := '0';
signal off_start : std_logic := '1';

--Multiple PRT Signals
signal prt_mult : std_logic_vector(3 downto 0) := "0000";

begin

process (CLK, TRIGGER, CNTL(0))
begin 
if ((CNTL(0) = '1') and (TRIGGER = '1') and (Rising_Edge(CLK) or (start = '1')))  then
--if ((CNTL(0) = '1') and (TRIGGER = '1') and (Rising_Edge(CLK)))  then	
	if (period_int < ((PERIOD*prt_mult)-WIDTH-DELAY-1)) and (prt_mult > 0) then
		period_int <= period_int + 1;
	else
		start <= '0';	
		if (delay_int < DELAY) then
			delay_int <= delay_int + 1;
		else
			pulse <= not CNTL(1);
			if (width_int < WIDTH) then
				width_int <= width_int + 1;
			else
				width_int <= "0000000000000000";
				period_int <= "0000000000000000";
				delay_int <= "0000000000000000";		
				pulse <= CNTL(1);
				if	(prt_mult = MULT(3 downto 0)) and (MULT(7 downto 4) > "000") then
					prt_mult <= MULT(7 downto 4);
				elsif (prt_mult = MULT(7 downto 4)) and (MULT(11 downto 8) > "000") then
					prt_mult <= MULT(11 downto 8);
				elsif (prt_mult = MULT(11 downto 8)) and (MULT(15 downto 12) > "000") then
					prt_mult <= MULT(15 downto 12);				
				else
					prt_mult <= MULT(3 downto 0);
				end if;
			end if;
		end if;		
	end if;
else
	if (TRIGGER = '0') then
		width_int <= "0000000000000000";
		period_int <= "1111111111111110";
		delay_int <= "0000000000000000";
		prt_mult <= "0000";		
		pulse <= CNTL(1);
		start <= '1';	
	end if;
end if;
end process;


process (CLK, TRIGGER, CNTL(0))
begin 
if ((CNTL(0) = '1') and (TRIGGER = '1') and (((CLK'event) and (CLK = '1')) or (off_start = '1')))  then
	if (off_period_int < ((OFF_PERIOD)-OFF_WIDTH-OFF_DELAY-1)) then
		off_period_int <= off_period_int + 1;
	else
		off_start <= '0';	
		if (off_delay_int < OFF_DELAY) then
			off_delay_int <= off_delay_int + 1;
		else
			off_flag <= '1';
			if (off_width_int < OFF_WIDTH) then
				off_width_int <= off_width_int + 1;
			else
				off_width_int <= "0000000000000000";
				off_period_int <= "0000000000000000";
				off_delay_int <= "0000000000000000";		
				off_flag <= '0';
			end if;
		end if;		
	end if;
else
	if (TRIGGER = '0') then
		off_width_int <= "0000000000000000";
		off_period_int <= "1111111111111110";
		off_delay_int <= "0000000000000000";		
		off_flag <= '0';
		off_start <= '1';
	end if;
end if;
end process;




PULSE_OUT <= pulse when (ENABLE_L = '0' and off_flag = '0') else CNTL(1);
end Behavioral;