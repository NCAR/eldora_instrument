--  Copyright 2003, BittWare, Inc.
--  The user is hereby granted a non-exclusive license to
--  use and or modify this software provided that it runs
--  on BittWare hardware. Usage of this software on 
--  non-BittWare hardware without the express written 
--  permission of BittWare is strictly prohibited.
--
--    Ver.  Dates     Author  Changes                                
--    ----  --------  ------  ----------------------------
--    1.0   02/11/03   KK	Initial coding
--   -------------------------------------------------------
--
-- This module moves data from a 64 bit fifo to a link port.
--
 
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity F64toL is
    Port (	Reset : in std_logic;									-- Active high.
				LinkClk : in std_logic;									-- Runs at 2X the link port clock frequency 
				TxBP : in std_logic;										-- Signal from link ports asserted means stop sending link data.
				FIFODat : in std_logic_vector (63 downto 0);		-- Data in from FIFO  
				FIFORdCnt : in std_logic_vector (8 downto 0);	-- each count of 1 means there are 2 words in the fifo
				FIFORd : out std_logic;									-- Active high FIFORd signal;  
				LinkDat : out std_logic_vector (7 downto 0);		-- Data out to link 
				LinkWr  : out std_logic								-- Write strobe to link port.
			);	
		   
end F64toL;

architecture Behavioral of F64toL is

type StateType is (Idle, Pack0, Pack1, Pack2, Pack3, Pack4, Pack5, Pack6, Pack7);	 
signal State : StateType;

signal LinkMuxSel : std_logic_vector(2 downto 0);

signal WordCnt : std_logic;
signal WordCntReset : std_logic;
signal WordCntEn : std_logic;

signal FIFOEmpty : std_logic;	-- Active high. fifo empty signal.

begin

-- Goes 1 when there are less than 2 words in the FIFO
FIFOEmpty <= '1' when FIFORdCnt = "00000000" else '0';

LinkMuxProc : process (LinkMuxSel, FIFODat)
begin
	case LinkMuxSel is
		when "000" =>
			LinkDat <= FIFODat(7 downto 0);
		when "001" =>
			LinkDat <= FIFODat(15 downto 8);
		when "010" =>
			LinkDat <= FIFODat(23 downto 16);
		when "011" =>
			LinkDat <= FIFODat(31 downto 24);
		when "100" =>
			LinkDat <= FIFODat(39 downto 32);
		when "101" =>
			LinkDat <= FIFODat(47 downto 40);
		when "110" =>
			LinkDat <= FIFODat(55 downto 48);
		when "111" =>
			LinkDat <= FIFODat(63 downto 56);
		when others =>
			LinkDat <= FIFODat(63 downto 56);
	end case;
end process;

StateMachineProc : process (Reset, LinkClk)
  begin
	if (Reset = '1')then
		State <= Idle;
		LinkMuxSel <= "000";
		LinkWr <= '0';
	elsif Rising_Edge(LinkClk) then 
		case State is
			when Idle =>	-- We sit here when the module is disabled 
				if FIFOEmpty = '0' and TxBP = '0' then
					State <= Pack0;
					LinkWr <= '1';
				else
					LinkWr <= '0';
				end if;
				LinkMuxSel <= "000";
			when Pack0 =>	-- Write first byte of FIFO word here
				LinkWr <= TxBP;
				if TxBP = '0' then
					State <= Pack1;
					LinkWr <= '1';
				else
					LinkWr <= '0';
				end if;
				LinkMuxSel <= "001";
			when Pack1 =>	-- Write first byte of FIFO word here
				LinkWr <= TxBP;
				if TxBP = '0' then
					State <= Pack2;
					LinkWr <= '1';
				else
					LinkWr <= '0';
				end if;
				LinkMuxSel <= "010";
			when Pack2 =>	-- Write first byte of FIFO word here
				LinkWr <= TxBP;
				if TxBP = '0' then
					State <= Pack3;
					LinkWr <= '1';
				else
					LinkWr <= '0';
				end if;
				LinkMuxSel <= "011";
			when Pack3 =>	-- Write first byte of FIFO word here
				LinkWr <= TxBP;
				if TxBP = '0' then
					State <= Pack4;
					LinkWr <= '1';
				else
					LinkWr <= '0';
				end if;
				LinkMuxSel <= "100";
			when Pack4 =>	-- Write first byte of FIFO word here
				LinkWr <= TxBP;
				if TxBP = '0' then
					State <= Pack5;
					LinkWr <= '1';
				else
					LinkWr <= '0';
				end if;
				LinkMuxSel <= "101";
			when Pack5 =>	-- Write first byte of FIFO word here
				LinkWr <= TxBP;
				if TxBP = '0' then
					State <= Pack6;
					LinkWr <= '1';
				else
					LinkWr <= '0';
				end if;
				LinkMuxSel <= "110";
			when Pack6 =>	-- Write first byte of FIFO word here
				LinkWr <= TxBP;
				if TxBP = '0' then
					State <= Pack7;
					LinkWr <= '1';
				else
					LinkWr <= '0';
				end if;
				LinkMuxSel <= "111";
			when Pack7 =>	-- Write eighth byte of FIFO word here
				if TxBP = '0' and (FIFOEmpty = '0' or WordCnt /= '0') then
					LinkWr <= '1';
					State <= Pack0;
				else
					if WordCnt = '0' and FIFOEmpty = '1' then
						State <= Idle;
					else
						State <= Pack7;
					end if;
					LinkWr <= '0';
				end if;
				LinkMuxSel <= "000";
		end case;
	end if;
end process;

FIFORd <= '1' when TxBP = '0' and (FIFOEmpty = '0' or WordCnt /= '0') and (State = Idle  or State = Pack7) else '0';

WordCntReset <= '1' when State = Idle else '0';
WordCntEn <= '1' when State = Pack0 else '0';

WordCountProc : process (WordCntReset,LinkClk)
begin
	if WordCntReset = '1' then
		WordCnt <= '0';
	elsif Rising_Edge(LinkClk) then
		if WordCntEn = '1' then
			WordCnt <= not WordCnt;
		end if;
	end if;
end process;

end Behavioral;
