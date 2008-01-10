--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    12:11:48 01/13/06
-- Design Name:    
-- Module Name:    ClockDivider - Behavioral
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

entity ClockDivider is
    Port ( Clk : in std_logic;
           Sel : in std_logic_vector(1 downto 0);
           ClkOut : out std_logic);
end ClockDivider;

architecture Behavioral of ClockDivider is

signal count : std_logic_vector(3 downto 0);
signal out_clk : std_logic;

begin

process(Clk)
begin
if Rising_Edge(Clk) then
	count <= count + 1;	
	case Sel is
		when "01" => out_clk <= count(0);	
		when "10" => out_clk <= count(1);		
		when "11" => out_clk <= count(2);	
		when others => null;
	end case;	
end if;
end process;
ClkOut <= Clk when Sel = "00" else out_clk;
end Behavioral;
