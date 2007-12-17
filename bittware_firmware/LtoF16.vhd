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
-- This module moves data from a link port to a 16 bit fifo.
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

entity LtoF16 is
    Port (	Reset				: in std_logic;								-- Active high.
				LinkClk			: in std_logic;								-- Runs at 2X the link port clock frequency 
				LinkDat			: in std_logic_vector (7 downto 0);		-- Data in from the RXlink 
				LinkDataValid	: in std_logic;								-- Signal to the link ports asserted means stop sending link data.
				FIFOWrCnt		: in std_logic_vector (7 downto 0);		-- each count of 1 means there are 8 words in the fifo
				RxBP				: out std_logic;								-- Signal to the link ports asserted means stop sending link data.
				FIFOWr			: out std_logic;								-- Writes to the FIFO signal;  
				FIFODat			: out std_logic_vector (15 downto 0)	-- FIFO data to the FIFOs
			);
		   
end LtoF16;
 
architecture Behavioral of LtoF16 is

type StateType is (Pack0, WrFIFO);	 
signal State : StateType;

signal Buf0 : std_logic_vector(7 downto 0);

begin

-- Goes 1 when there is not enough room for 8 words in the FIFO
RxBP <= '1' when FIFOWrCnt = "11111111" else '0';

StateMachineProc : process (Reset, LinkClk)
  begin
	if (reset = '1')then
		State <= Pack0;
	elsif Rising_Edge(LinkClk) then 
		case State is
			when Pack0 =>
				if LinkDataValid = '1' then
					State <= WrFIFO;
					Buf0 <= LinkDat;
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
FIFODat(15 downto 8) <= LinkDat;

end Behavioral;
