--  Copyright 2003, BittWare, Inc.
--  The user is hereby granted a non-exclusive license to
--  use and or modify this software provided that it runs
--  on BittWare hardware. Usage of this software on 
--  non-BittWare hardware without the express written 
--  permission of BittWare is strictly prohibited.
--
--    Ver.  Dates     Author  Changes                                
--    ----  --------  ------  ----------------------------
--    1.0   02/10/03   KK	Initial coding
--   -------------------------------------------------------
--
-- This module moves data from a 16 bit fifo to a link port.
--
 
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity F16toL is
    Port (	Reset : in std_logic;									-- Active high.
				LinkClk : in std_logic;									-- Runs at 2X the link port clock frequency 
				TxBP : in std_logic;										-- Signal from link ports asserted means stop sending link data.
				FIFODat : in std_logic_vector (15 downto 0);		-- Data in from FIFO  
				FIFORdCnt : in std_logic_vector (7 downto 0);	-- each count of 1 means there are 8 words in the fifo
				FIFORd : out std_logic;									-- Active high FIFORd signal;  
				LinkDat : out std_logic_vector (7 downto 0);		-- Data out to link 
				LinkWr  : out std_logic								-- Write strobe to link port.
			);	
		   
end F16toL;

architecture Behavioral of F16toL is

type StateType is (Idle, Pack0, Pack1);	 
signal State : StateType;

signal LinkMuxSel : std_logic;

signal WordCnt : std_logic_vector(2 downto 0);
signal WordCntReset : std_logic;
signal WordCntEn : std_logic;

signal FIFOEmpty : std_logic;	-- Active high. fifo empty signal.

begin

-- Goes 1 when there are less than 8 words in the FIFO
FIFOEmpty <= '1' when FIFORdCnt = "00000000" else '0';

LinkMuxProc : process (LinkMuxSel, FIFODat)
begin
	if LinkMuxSel = '0' then
		LinkDat <= FIFODat(7 downto 0);
	else
		LinkDat <= FIFODat(15 downto 8);
	end if;
end process;

StateMachineProc : process (Reset, LinkClk)
  begin
	if (Reset = '1')then
		State <= Idle;
		LinkMuxSel <= '0';
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
				LinkMuxSel <= '0';
			when Pack0 =>	-- Write first half of FIFO word here
				LinkWr <= TxBP;
				if TxBP = '0' then
					State <= Pack1;
					LinkWr <= '1';
				else
					LinkWr <= '0';
				end if;
				LinkMuxSel <= '1';
			when Pack1 =>	-- Write second half of FIFO word here
				if TxBP = '0' and (FIFOEmpty = '0' or WordCnt /= "000") then
					LinkWr <= '1';
					State <= Pack0;
				else
					if WordCnt = "000" and FIFOEmpty = '1' then
						State <= Idle;
					else
						State <= Pack1;
					end if;
					LinkWr <= '0';
				end if;
				LinkMuxSel <= '0';
		end case;
	end if;
end process;

FIFORd <= '1' when TxBP = '0' and (FIFOEmpty = '0' or WordCnt /= "000") and (State = Idle  or State = Pack1) else '0';

WordCntReset <= '1' when State = Idle else '0';
WordCntEn <= '1' when State = Pack0 else '0';

WordCountProc : process (WordCntReset,LinkClk)
begin
	if WordCntReset = '1' then
		WordCnt <= (others => '0');
	elsif Rising_Edge(LinkClk) then
		if WordCntEn = '1' then
			WordCnt <= WordCnt + 1;
		end if;
	end if;
end process;

end Behavioral;
