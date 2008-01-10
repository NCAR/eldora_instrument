--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    12:11:37 01/12/06
-- Design Name:    
-- Module Name:    Register_16bit - Behavioral
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
entity Register_16bit is
    Port ( Clk : in std_logic;
	 		  Enable : in std_logic;
			  WR : in std_logic;
           DataIn : inout std_logic_vector(15 downto 0);
           DataOut: out std_logic_vector(15 downto 0);
           Reset: in std_logic
           );
end Register_16bit;

architecture Behavioral of Register_16bit is

signal out_data : std_logic_vector(15 downto 0);

begin
process (Clk,Enable,WR, Reset)
begin
	if Reset = '1' then
			out_data <= (others=>'0');
	elsif Rising_Edge(Clk) and Enable = '1' and WR = '1' then
			out_data <= DataIn;
	end if;
end process;
DataOut <= out_data;
DataIn <= out_data when (Enable = '1' and WR = '0') else (OTHERS=>'Z');
end Behavioral;