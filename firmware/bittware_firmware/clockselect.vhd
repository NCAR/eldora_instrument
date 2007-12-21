----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:57:11 12/17/2007 
-- Design Name: 
-- Module Name:    clockselect - Behavioral 
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

entity clockselect is
    Port ( ClkIn : in  STD_LOGIC;
           IntClkIn : in  STD_LOGIC;
			  Sel : in  STD_LOGIC;
           ClkOut : out  STD_LOGIC);
end clockselect;

architecture Behavioral of clockselect is

begin

ClkOut <= IntClkIn when Sel = '1' else ClkIn;

end Behavioral;

