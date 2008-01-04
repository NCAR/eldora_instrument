----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:45:17 10/24/2006 
-- Design Name: 
-- Module Name:    Gate_Splitter - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Gate_Splitter is
    Port ( Reset   : in STD_LOGIC;
			  Gate_In_Int : in  STD_LOGIC;
			  Gate_In_Ext : in  STD_LOGIC;
			  Timing_Sel : in STD_LOGIC;
           Dual_PRT : in  STD_LOGIC;
           Gate1_Out : out  STD_LOGIC;
           Gate2_Out : out  STD_LOGIC);
end Gate_Splitter;

architecture Behavioral of Gate_Splitter is

signal pulse	: std_logic;
signal Gate_In : std_logic;

begin

Gate_In <= Gate_In_Int when Timing_Sel = '1' else Gate_In_Ext;

gate_splitter : process (Gate_In, Reset)
begin
	if (Reset = '1') then
		pulse <= '0';
	elsif rising_edge(Gate_In) then
		if (Dual_PRT = '1') then
			if (pulse = '0') then
				pulse <= '1';
			elsif (pulse = '1') then
				pulse <= '0';
			end if;
		elsif (Dual_PRT = '0') then
			pulse <= '1';
		end if;
	end if;
end process;	

Gate1_Out <= Gate_In when pulse = '1' else '0';
Gate2_Out <= Gate_In when pulse = '0' else '0';
end Behavioral;

