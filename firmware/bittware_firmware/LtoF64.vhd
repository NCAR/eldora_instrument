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
-- This module moves data from a link port to a 64 bit fifo.
--
-- 																						 .

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity LtoF64 is
    Port (	Reset				: in std_logic;								-- Active high.
				LinkClk			: in std_logic;								-- Runs at 2X the link port clock frequency 
				LinkDat			: in std_logic_vector (7 downto 0);		-- Data in from the RXlink 
				LinkDataValid	: in std_logic;								-- Signal to the link ports asserted means stop sending link data.
				FIFOWrCnt		: in std_logic_vector (8 downto 0);		-- each count of 1 means there are 2 words in the fifo
				RxBP				: out std_logic;								-- Signal to the link ports asserted means stop sending link data.
				FIFOWr			: out std_logic;								-- Writes to the FIFO signal;  
				FIFODat			: out std_logic_vector (63 downto 0)	-- FIFO data to the FIFOs
			);
		   
end LtoF64;
 
architecture Behavioral of LtoF64 is

type StateType is (Pack0, Pack1, Pack2, Pack3, Pack4, Pack5, Pack6, WrFIFO);	 
signal State : StateType;

signal Buf0, Buf1, Buf2, Buf3, Buf4, Buf5, Buf6 : std_logic_vector(7 downto 0);

begin

-- Goes 1 when there is not enough room for 2 words in the FIFO
RxBP <= '1' when FIFOWrCnt = "111111111" else '0';

StateMachineProc : process (Reset, LinkClk)
  begin
	if (reset = '1')then
		State <= Pack0;
	elsif Rising_Edge(LinkClk) then 
		case State is
			when Pack0 =>
				if LinkDataValid = '1' then
					State <= Pack1;
					Buf0 <= LinkDat;
				end if; 
			when Pack1 =>
				if LinkDataValid = '1' then
					State <= Pack2;
					Buf1 <= LinkDat;
				end if; 
			when Pack2 =>
				if LinkDataValid = '1' then
					State <= Pack3;
					Buf2 <= LinkDat;
				end if; 
			when Pack3 =>
				if LinkDataValid = '1' then
					State <= Pack4;
					Buf3 <= LinkDat;
				end if; 
			when Pack4 =>
				if LinkDataValid = '1' then
					State <= Pack5;
					Buf4 <= LinkDat;
				end if; 
			when Pack5 =>
				if LinkDataValid = '1' then
					State <= Pack6;
					Buf5 <= LinkDat;
				end if; 
			when Pack6 =>
				if LinkDataValid = '1' then
					State <= WrFIFO;
					Buf6 <= LinkDat;
				end if; 
			when WrFIFO => 
				if LinkDataValid = '1' then
					State <= Pack0;
				end if; 
		end case;
	end if;
end process;

FIFOWr <= '1' when State = WrFIFO and LinkDataValid = '1' else '0';

FIFODat(7 downto 0) <= Buf0;
FIFODat(15 downto 8) <= Buf1;
FIFODat(23 downto 16) <= Buf2;
FIFODat(31 downto 24) <= Buf3;
FIFODat(39 downto 32) <= Buf4;
FIFODat(47 downto 40) <= Buf5;
FIFODat(55 downto 48) <= Buf6;
FIFODat(63 downto 56) <= LinkDat;

end Behavioral;
