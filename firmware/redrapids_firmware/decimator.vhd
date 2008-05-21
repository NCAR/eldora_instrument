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

Library UNISIM; 
use UNISIM.Vcomponents.all; 

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity decimator is
    Port ( clk_in : in  STD_LOGIC;
			  trigger : in STD_LOGIC;
			  reset : in STD_LOGIC;
           decimation : in  STD_LOGIC_VECTOR (7 downto 0);
           dec_clk : out  STD_LOGIC;
			  fifo_clk: out STD_LOGIC);
end decimator;

architecture Behavioral of decimator is

signal count1	 : std_logic_vector(7 downto 0);
signal count2   : std_logic_vector(7 downto 0);
signal out_clk1 : std_logic;
signal out_clk1_buf : std_logic;
signal out_clk2_buf : std_logic;
signal out_clk2 : std_logic;
signal dec_reg1 : std_logic_vector(7 downto 0);
signal dec_reg2 : std_logic_vector(7 downto 0);

signal trigger_flag : std_logic;

begin

DEC_CLK_BUFG_INST       : BUFG PORT MAP(I => out_clk2_buf, O => dec_clk);
FIFO_CLK_BUFG_INST      : BUFG PORT MAP(I => out_clk1_buf, O => fifo_clk);

dec_reg1 <= decimation - 1 ;
dec_reg2 <= decimation + decimation + decimation - 1;

process (trigger, reset)
begin
	if (reset = '1') then
		trigger_flag <= '0';
	elsif falling_edge(trigger) then 
		trigger_flag <= '1';
	else 
		trigger_flag <= trigger_flag;
	end if;
end process;

process (clk_in, reset, trigger_flag) 
begin
	if (reset = '1' or trigger_flag = '0') then 
		count1 <= (others => '0');
		count2 <= (others => '0');
		out_clk1 <= '0';
		out_clk2 <= '0';
	elsif rising_edge(clk_in) and (trigger_flag = '1') then
		if (count1 = 0) then
			out_clk1 <= not out_clk1;
			count1 <= dec_reg1;
		else
			count1 <= count1 - 1;
		end if;
		if (count2 = 0) then
			out_clk2 <= not out_clk2;
			count2 <= dec_reg2;
		else
			count2 <= count2 - 1;
		end if;
	end if;
end process;

process (clk_in)
begin
   if rising_edge(clk_in) then
		out_clk1_buf <= out_clk1;
		out_clk2_buf <= out_clk2;
	else
		out_clk1_buf <= out_clk1_buf;
		out_clk2_buf <= out_clk2_buf;
	end if;
end process;

end Behavioral;