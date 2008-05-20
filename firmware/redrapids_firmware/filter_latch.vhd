----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    11:35:07 05/20/2008 
-- Design Name: 
-- Module Name:    Filter_Latch - Behavioral 
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

entity filter_latch is
    Port ( clk : in  STD_LOGIC;
           stop_in : in  STD_LOGIC;
           stop_out : out  STD_LOGIC);
end filter_latch;

architecture Behavioral of filter_latch is

begin

process (clk)
begin
	if rising_edge(clk) then 
		stop_out <= stop_in;
	end if;
end process;

end Behavioral;

