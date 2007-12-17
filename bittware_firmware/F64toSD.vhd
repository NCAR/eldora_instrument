--  Copyright 2003, BittWare, Inc.
--  The user is hereby granted a non-exclusive license to
--  use and or modify this software provided that it runs
--  on BittWare hardware. Usage of this software on 
--  non-BittWare hardware without the express written 
--  permission of BittWare is strictly prohibited.
--
--    Ver.  Dates     Author  Changes                                
--    ----  --------  ------  ----------------------------
--    1.0   02/11/03   Kento	Initial coding
--   -------------------------------------------------------
--
-- This module masters data from a 64 bit fifo to SDRAM on the HH Bus.
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

entity F64toSD is
	Port (	Clk				: in std_logic;
				Reset				: in std_logic;
				BML2SAddrWr		: in std_logic;								-- Write Select line for the Address
				BML2SAddrDatIn	: in std_logic_vector(31 downto 0);		-- Data written when address reg is written to
				BusGnt			: in std_logic;								-- BusGnt giving this module the bus
				Ack				: in std_logic;								-- Ack lines from the HH Bus
				LinkDatVal		: in std_logic;								-- DataValid signal from the link port
				FIFOEmp			: in std_logic;								-- FIFO empty signal
				FIFORdCnt		: in std_logic_vector (8 downto 0);		-- each count of 1 means there are 2 words in the fifo
				FIFODat			: in std_logic_vector (63 downto 0);	-- Data from the FIFO
				FIFORd			: out std_logic;								-- FIFO Read signal;  
				BML2SAddrDatout: out std_logic_vector(31 downto 0);	-- Data read when address reg is read from
				AddrOut			: out std_logic_vector(31 downto 0);	-- Address we are accessing
				MS0Out_l			: out std_logic;								-- SDRAM Select line
				WrLOut_l			: out std_logic;								-- HH bus low word write line
				WrHOut_l			: out std_logic;								-- HH bus hi word write line
				BrstOut			: out std_logic;								-- HH bus burst signal
				BusReq			: out std_logic;
				BusInUse			: out std_logic;
				BusDatEn			: out std_logic_vector(63 downto 0);
				DataOut			: out std_logic_vector(63 downto 0)
			);
end F64toSD;

architecture F64toSD_Arch of F64toSD is

	component BMAddrCnter port (
		l		: in  std_logic_vector (25 downto 0);
		load	: in std_logic;
		ce		: in std_logic;
		clk	: in std_logic;
		ainit	: in std_logic;
		q		: out  std_logic_vector (25 downto 0));
	end component;

type StateType is (Idle, Wr, WrEnd);	 
signal State : StateType;

signal BMRdCntBrstDis : std_logic;
signal BMAddrBrstDis : std_logic;
signal LinkDatValReg : std_logic;
signal BMAddrReg : std_logic_vector(31 downto 0);
signal AddrCntEn : std_logic;
signal BusInUse_Int : std_logic;
signal Wr_l : std_logic;
signal MS0Out_l_Int : std_logic;
signal BrstOut_Int : std_logic;
signal DataAv : std_logic;
signal BrstEn : std_logic;

begin

AddrCntEn <= '1' when Ack = '1' and ((State = Wr and MS0Out_l_Int = '1') or State = WrEnd) else '0';

CurrentAddr : BMAddrCnter
port map(
	l 	=> BML2SAddrDatIn(26 downto 1),
	load 	=> BML2SAddrWr,
	ce 	=> AddrCntEn,
	clk 	=> Clk,	
	ainit	=> Reset,
	q 	=>  BMAddrReg(26 downto 1)); 

BMAddrReg(0) <= '0';
BMAddrReg(31 downto 27) <= (others => '0');

BML2SAddrDatOut <= BMAddrReg;

BMRdCntBrstDis <= '1' when FIFORdCnt(8 downto 1) = "0000000000" else '0';
BMAddrBrstDis <= '1' when BMAddrReg(8 downto 3) = "111111" and BMAddrReg(2 downto 1) /= "00" else '0';

BrstEnRegProc : process (Reset, Clk)
begin
	if Reset = '1' then
		BrstEn <= '0';
	elsif Rising_Edge(Clk) then
		if BMAddrBrstDis = '1' or BMRdCntBrstDis = '1' then
			BrstEn <= '0';
		else
			BrstEn <= '1';
		end if;
	end if;
end process;

-- This is on another clock domain.  Sample it to eliminate domain crossing issues
LinkDatValDFF : process (Reset, Clk)
begin
	if Reset = '1' then
			LinkDatValReg <= '0';
	elsif Rising_Edge(Clk) then
		LinkDatValReg <= LinkDatVal;
	end if;
end process;

DataAv <= FIFORdCnt(8) when LinkDatValReg = '1' else not FIFOEmp;

BusReq <= DataAv;
BusInUse <= BusInUse_Int;
BusDatEn <= (others => BusInUse_Int);
DataOut <= FIFODat;

AddrOut(30 downto 0) <= BMAddrReg(30 downto 0);
-- This keeps us from writting into this chips space
-- This chip ignores MS0 and the SDRAM controller ignores Addr 31
AddrOut(31) <= '1';
MS0Out_l <= MS0Out_l_Int;
WrLOut_l <= Wr_l;
WrHOut_l <= Wr_l;
BrstOut <= BrstOut_Int;

StateMachineProc :process (Reset, Clk)
begin
	if (Reset = '1')then
		state <= idle;
		Wr_l <= '1';
		MS0Out_l_Int <= '1';
		BrstOut_Int <= '0';
		BusInUse_Int <= '0';
	elsif Rising_Edge(Clk) then 
		case state is
			when Idle => 
		     	if Ack = '1'  and DataAv = '1' and BusGnt = '1'then
					state <= Wr;		-- Data available and the bus is ours start a transaction
					MS0Out_l_Int <= '0';
					Wr_l <= '0';
					BusInUse_Int <= '1';
				else 
					state <= Idle;			-- Nothing to do!!!  Be a couchpotato and hang out
					MS0Out_l_Int <= '1';
					Wr_l <= '1';
					BusInUse_Int <= '0';
				end if;
				BrstOut_Int <= '0';
			when Wr => -- Load a 64 bit word into data reg which drives the data bus
				if Ack = '1' then
					MS0Out_l_Int <= '1';
					if BrstEn = '1' then 
						state <= Wr; 	-- Keep on bursting
						BrstOut_Int <= '1';
					else
						state <= WrEnd;		-- Terminate this burst
						BrstOut_Int <= '0';
					end if;
				else
					state <= Wr;			-- No Ack Hang out here
					MS0Out_l_Int <= MS0Out_l_Int;
					BrstOut_Int <= BrstOut_Int;
				end if;
				Wr_l <= '0';
				BusInUse_Int <= '1';
			when WrEnd=>  -- Wait for Ack on last word of Write
				if Ack = '1' then
					state <= Idle;			-- Go Back to Idle to restart if necessary
					MS0Out_l_Int <= '1';
					Wr_l <= '1';
					BusInUse_Int <= '0';
				else
					state <= WrEnd;				-- No Ack or Not InSync  Stay put
					Wr_l <= '0';
					MS0Out_l_Int <= '1';
				end if;
				BrstOut_Int <= '0';
		end case;
	end if;
end process;

FIFORd <= '1' when Ack = '1' and ((State = Wr and MS0Out_l_Int = '1') or (State = Idle and DataAv = '1' and BusGnt = '1')) else '0';

end F64toSD_Arch;

