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
    generic(NTIMERS : integer := 2);
    Port ( Clk 					: in std_logic;
           Reset 					: in std_logic;
			  AddrWr 				: in std_logic;
			  DataWr 				: in std_logic;
           AddrIn 				: in std_logic_vector(31 downto 0);
           DataIn 				: in std_logic_vector(31 downto 0);
           TimerAddrOut 		: out std_logic_vector(2 downto 0);
           DataInOut 			: inout std_logic_vector(31 downto 0);
			  AddrOut 				: out std_logic_vector(31 downto 0);
           WR 						: out std_logic;
			  GlobalEnable 		: out std_logic;
			  ------------------------------------------------------------------
			  GPSEnable 			: out std_logic; -------- not used for now-----
			  GPSReset 				: out std_logic;
           ------------------------------------------------------------------
			  TimerSelect 			: out std_logic_vector(NTIMERS-1 downto 0);
			  AddrTrigger 			: out std_logic);
			  
end TimerInterface;

architecture Behavioral of TimerInterface is

signal out_data : std_logic_vector(31 downto 0);
signal out_timeraddr : std_logic_vector(2 downto 0);
signal out_addr : std_logic_vector(31 downto 0);
signal out_timerselect : std_logic_vector(NTIMERS-1 downto 0);
signal out_wr : std_logic;
signal out_globalenable : std_logic;
signal out_gpsenable : std_logic;
signal out_gpsreset : std_logic;
signal out_addrtrigger : std_logic;

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
			out_gpsenable <= '0';
			out_gpsreset <= '0';
			out_addrtrigger <= '0';
		elsif Rising_Edge(Clk) then
			if AddrWr = '1' then
				out_timeraddr <= AddrIn(2 downto 0);
				out_wr <= AddrIn(3);
				out_timerselect <= AddrIn(NTIMERS+3 downto 4);
				out_globalenable <= AddrIn(12);
				out_addr <= AddrIn;
				out_gpsenable <= AddrIn(13);
				out_gpsreset <= AddrIn(14);
				out_addrtrigger <= AddrIn(15);
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
	GPSEnable <= out_gpsenable;
	GPSReset <= out_gpsreset;
	AddrTrigger <= out_addrtrigger;
	AddrOut <= out_addr;
	DataInOut(31 downto 16) <= out_data(31 downto 16) when (out_wr = '1') else (OTHERS => '0');
	DataInOut(15 downto 0) <= out_data(15 downto 0) when (out_wr = '1') else (OTHERS => 'Z');	------- Why high impedance??

end Behavioral;
