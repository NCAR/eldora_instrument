--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    10:07:46 01/12/06
-- Design Name:    
-- Module Name:    TimerInterface - Behavioral
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

entity TimerInterface is
    Port ( Clk : in std_logic;
           Reset : in std_logic;
           AddrIn : in std_logic_vector(31 downto 0);
           DataIn : in std_logic_vector(31 downto 0);
			  TimerAddrOut : out std_logic_vector(2 downto 0);
           DataInOut : inout std_logic_vector(31 downto 0);
			  AddrOut : out std_logic_vector(31 downto 0);
           WR : out std_logic;
			  GlobalEnable : out std_logic;
			  TimerSelect : out std_logic_vector(20 downto 0);
			  AddrTrigger : out std_logic;
			  Timer_Rst : out std_logic;
			  AddrWr : in std_logic;
			  DataWr : in std_logic);
end TimerInterface;

architecture Behavioral of TimerInterface is

signal out_data : std_logic_vector(31 downto 0);
signal out_timeraddr : std_logic_vector(2 downto 0);
signal out_addr : std_logic_vector(31 downto 0);
signal out_timerselect : std_logic_vector(20 downto 0);
signal out_wr : std_logic;
signal out_globalenable : std_logic;
signal out_addrtrigger : std_logic;
signal out_timerrst : std_logic;

begin

	process(Clk, Reset)
	begin
		if Reset = '1' then
			out_data <= (others => '0');
			out_timeraddr <= (others => '0');
			out_addr <= (others => '0');
			out_timerselect <= (others => '0');
			out_wr <= '0';
			out_globalenable <= '0';
			out_addrtrigger <= '0';
			out_timerrst <= '1';
		elsif Rising_Edge(Clk) then
			if AddrWr = '1' then
				out_timeraddr <= AddrIn(2 downto 0);
				--out_timerselect <= AddrIn(11 downto 4);
				out_globalenable <= AddrIn(12);
				out_wr <= AddrIn(3);
				out_addr <= AddrIn;
				out_addrtrigger <= AddrIn(15);
				out_timerrst <= AddrIn(13);
				
				if (AddrIn(11 downto 4) = "00000000") then
					out_timerselect <= "000000000000000000001";
				elsif (AddrIn(11 downto 4) = "00000001") then
					out_timerselect <= "000000000000000000010";
				elsif (AddrIn(11 downto 4) = "00000010") then
					out_timerselect <= "000000000000000000100";
				elsif (AddrIn(11 downto 4) = "00000011") then
					out_timerselect <= "000000000000000001000";
				elsif (AddrIn(11 downto 4) = "00000100") then
					out_timerselect <= "000000000000000010000";
				elsif (AddrIn(11 downto 4) = "00000101") then
					out_timerselect <= "000000000000000100000";
				elsif (AddrIn(11 downto 4) = "00000110") then
					out_timerselect <= "000000000000001000000";
				elsif (AddrIn(11 downto 4) = "00000111") then
					out_timerselect <= "000000000000010000000";
				elsif (AddrIn(11 downto 4) = "00001000") then
					out_timerselect <= "000000000000100000000";
				elsif (AddrIn(11 downto 4) = "00001001") then
					out_timerselect <= "000000000001000000000";
				elsif (AddrIn(11 downto 4) = "00001010") then
					out_timerselect <= "000000000010000000000";
				elsif (AddrIn(11 downto 4) = "00001011") then
					out_timerselect <= "000000000100000000000";
				elsif (AddrIn(11 downto 4) = "00001100") then
					out_timerselect <= "000000001000000000000";
				elsif (AddrIn(11 downto 4) = "00001101") then
					out_timerselect <= "000000010000000000000";
				elsif (AddrIn(11 downto 4) = "00001110") then
					out_timerselect <= "000000100000000000000";
				elsif (AddrIn(11 downto 4) = "00001111") then
					out_timerselect <= "000001000000000000000";
				elsif (AddrIn(11 downto 4) = "00010000") then
					out_timerselect <= "000010000000000000000";
				elsif (AddrIn(11 downto 4) = "00010001") then
					out_timerselect <= "000100000000000000000";
				elsif (AddrIn(11 downto 4) = "00010010") then
					out_timerselect <= "001000000000000000000";
				elsif (AddrIn(11 downto 4) = "00010011") then
					out_timerselect <= "010000000000000000000";
				elsif (AddrIn(11 downto 4) = "00010100") then
					out_timerselect <= "100000000000000000000";
				else
					out_timerselect <= out_timerselect;
				end if;
					
			end if;
			if DataWr = '1' then
				out_data <= DataIn;
			end if;
		end if;
	end process;

	TimerAddrOut <= out_timeraddr;
	TimerSelect	<= out_timerselect;
	WR <= out_wr;
	GlobalEnable <= out_globalenable;
	AddrTrigger <= out_addrtrigger;
	Timer_Rst <= out_timerrst;
	AddrOut <= out_addr;
	DataInOut(31 downto 16) <= out_data(31 downto 16) when (out_wr = '1') else (OTHERS => '0');
	DataInOut(15 downto 0) <= out_data(15 downto 0) when (out_wr = '1') else (OTHERS => 'Z');

end Behavioral;
