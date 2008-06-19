----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    13:53:58 01/04/2008 
-- Design Name: 
-- Module Name:    Revision_Reg - Behavioral 
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

entity Revision_Reg is
    Port ( Revision_Number : out  STD_LOGIC_VECTOR (31 downto 0));
end Revision_Reg;

architecture Behavioral of Revision_Reg is

begin

Revision_Number <= conv_std_logic_vector(2169, 32);

end Behavioral;

