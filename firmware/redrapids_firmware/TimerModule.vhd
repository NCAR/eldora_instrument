--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    15:04:49 01/12/06
-- Design Name:    
-- Module Name:    TimerModule - Behavioral
-- Project Name:   
-- Target Device:  
-- Tool versions:  
-- Description:
--
-- Dependencies:
-- 
-- Revision:
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

entity TimerModule is
    Port ( 
	 		  --Clock Signals
			  Clk      : in std_logic;							  -- Clock

			  --Misc. Signals
			  Enable   : in std_logic;						     -- Component enable
			  Trigger  : in std_logic;						     -- Trigger to start pulse
			  Rst      : in std_logic;						  	  -- Component reset
			  Cntrl    : in std_logic_vector(15 downto 0); -- Control Register
			  Mult     : in std_logic_vector(15 downto 0); -- Multiple PRT data
           
			  --Transmit Pulse Signals
			  Delay    : in std_logic_vector(15 downto 0); -- # of clock cycles preceding +pulse edge
			  Width    : in std_logic_vector(15 downto 0); -- # of clock cycles to make pulse width
			  Period   : in std_logic_vector(15 downto 0); -- # of clock cycles to make pulse period
			  PulseOut : out std_logic);
end TimerModule;

architecture Behavioral of TimerModule is

-- Attributes Declarations
attribute clock_signal : string;
attribute buffer_type : string;
attribute uselowskewlines : string;

-- Pulse Generation Variables
signal delay_int  : std_logic_vector(15 downto 0);
signal width_int  : std_logic_vector(15 downto 0);
signal period_int : std_logic_vector(18 downto 0);
signal prt_mult   :std_logic_vector(3 downto 0); 

-- Derived Values
signal product : std_logic_vector (19 downto 0);

signal trig : std_logic;

-- Output Signal
signal pulse      : std_logic;

-- Attribute Definitions
attribute clock_signal of pulse : signal is "yes";
attribute buffer_type of pulse: signal is "bufg";
attribute uselowskewlines of pulse: signal is "yes";

begin
    

process(Rst, Trigger)
begin
if (Rst = '1') then
	trig <= '0';
elsif (Trigger = '1') then
	trig <= '1';
else
	trig <= trig;
end if;
end process;

-- Scheme to turn pulse on and off   
process (Clk)
begin 
if (Rst = '1' or trig = '0') then
	width_int   <= (others => '0');
	period_int  <= (others => '0');
	delay_int   <= (others => '0');
	prt_mult		<= Mult(3 downto 0);						
	pulse       <= Cntrl(1);
elsif (Rising_Edge(Clk) and Enable = '1')  then
	if (delay_int >= Delay) then
		pulse <= not Cntrl(1);
		if (width_int >= Width) then
			pulse <= Cntrl(1);
			if (period_int >= product) then
				period_int  <= (others => '0');
				width_int   <= (others => '0');
				delay_int   <= (others => '0');
				-- Check next prt_multiple and if it exists then use it, if not start over.
				if (prt_mult = Mult(3 downto 0)) and (Mult(7 downto 4) /= "0000") then
					 prt_mult <= Mult(7 downto 4);
				 elsif (prt_mult = Mult(7 downto 4)) and (Mult(11 downto 8) /= "0000") then
					 prt_mult <= Mult(11 downto 8);
				 elsif (prt_mult = Mult(11 downto 8)) and (Mult(15 downto 12) /= "0000") then
					 prt_mult <= Mult(15 downto 12);				
				 else
					 prt_mult <= Mult(3 downto 0);
				 end if;
			else
				period_int <= period_int + 1;
			end if;
		else
			width_int <= width_int + 1;
			period_int <= period_int + 1;
		end if;
	else
		delay_int <= delay_int + 1;
		period_int <= period_int + 1;
	end if;
end if;	
end process;

product <= period * prt_mult - 1 when (prt_mult /= 0 and prt_mult /= 1) else ("0000" & period) - 1;

PulseOut <= pulse when (Cntrl(0) = '1') else (Cntrl(1));

end Behavioral;