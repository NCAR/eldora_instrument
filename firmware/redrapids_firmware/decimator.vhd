----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    08:39:50 09/05/2006 
-- Design Name: 
-- Module Name:    decimator - Behavioral 
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

entity decimator is
    Port ( clk_in : in  STD_LOGIC;
			  reset : in STD_LOGIC;
           decimation : in  STD_LOGIC_VECTOR (7 downto 0);
           dec_clk : out  STD_LOGIC;
			  fifo_clk: out STD_LOGIC);
end decimator;

architecture Behavioral of decimator is

signal count1	 : std_logic_vector(7 downto 0);
signal count2   : std_logic_vector(7 downto 0);
signal out_clk1 : std_logic;
signal out_clk2 : std_logic;
signal dec_reg1 : std_logic_vector(7 downto 0);
signal dec_reg2 : std_logic_vector(7 downto 0);

begin

dec_reg1 <= decimation - 1 ;
dec_reg2 <= decimation + decimation + decimation - 1;

process (clk_in, reset) 
begin
	if (reset = '1') then 
		count1 <= (others => '0');
		count2 <= (others => '0');
		out_clk1 <= '0';
		out_clk2 <= '0';
	elsif rising_edge(clk_in) then
		count1 <= count1 + 1;
		count2 <= count2 + 1;
		if (count1 = dec_reg1) then
			count1 <= (others => '0');
			out_clk1 <= not out_clk1;
		end if;
		if (count2 = dec_reg2) then
			count2 <= (others => '0');
			out_clk2 <= not out_clk2;
		end if;
	end if;
	
end process;

fifo_clk <= out_clk1;
dec_clk  <= out_clk2;

end Behavioral;