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
-- This module masters data from SDRAM on the HH Bus to a 64 bit fifo.
--
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity SDtoF64 is
	Port (	Clk				: in std_logic;
				Reset				: in std_logic;
				BMS2LAddrWr		: in std_logic;								-- Write Select line for the Address
				BMS2LAddrDatIn	: in std_logic_vector (31 downto 0);	-- Data written when addr register is written
				BMS2LCntWr		: in std_logic;								-- Write Select line for the Count
				BMS2LCntDatIn	: in std_logic_vector (31 downto 0);	-- Data written when cnt register is written
				BusGnt			: in std_logic;								-- BusGnt giving this module the bus
				DataInReg		: in std_logic_vector (63 downto 0);	-- Data from the SDRAM
				Ack				: in std_logic;								-- Ack lines from the HH Bus
				FIFOWrCnt		: in std_logic_vector (8 downto 0);		-- each count of 1 means there are 2 words in the fifo
				FIFODat			: out std_logic_vector (63 downto 0);	-- Data to the FIFO
				FIFOWr			: out std_logic;								-- FIFO Read signal;  
				BMS2LAddrDatOut: out std_logic_vector (31 downto 0);	-- Data read when addr register is read
				BMS2LCntDatOut	: out std_logic_vector (31 downto 0);	-- Data read when cnt register is read
				AddrOut			: out std_logic_vector(31 downto 0);	-- Address we are accessing
				MS0Out_l			: out std_logic;								-- SDRAM Select line
				RdLOut_l			: out std_logic;								-- HH bus low word Read line
				RdHOut_l			: out std_logic;								-- HH bus hi word Read line
				BrstOut			: out std_logic;								-- HH bus burst signal
				BusReq			: out std_logic;
				BusInUse			: out std_logic
			);
end SDtoF64;

architecture SDtoF64_Arch of SDtoF64 is

	component BMAddrCnter port (
		l		: in  std_logic_vector (25 downto 0);
		load	: in std_logic;
		ce		: in std_logic;
		clk	: in std_logic;
		ainit	: in std_logic;
		q		: out  std_logic_vector (25 downto 0));
	end component;

	component BMCntCnter port (
		l				: in  std_logic_vector (24 downto 0);
		load			: in std_logic;
		ce				: in std_logic;
		clk			: in std_logic;
		aclr			: in std_logic;
		thresh0		: out std_logic;
		thresh1		: out std_logic;
		q				: out  std_logic_vector (24 downto 0));
	end component;

type StateType is (Idle, Rd, RdEnd);	 
signal State : StateType;

signal BMAddrReg : std_logic_vector(31 downto 0);
signal BMCnterReg : std_logic_vector(31 downto 0);
signal BMCnterDone : std_logic;
signal BMCnterAlmostDone : std_logic;
signal BMCnterBrstDis : std_logic;
signal BMAddrBrstDis : std_logic;
signal BMEn : std_logic;
signal AddrCntEn : std_logic;
signal CnterCntEn : std_logic;
signal BusInUse_Int : std_logic;
signal Rd_l : std_logic;
signal MS0Out_l_Int : std_logic;
signal BrstOut_Int : std_logic;
signal DataAv : std_logic;
signal BrstEn : std_logic;

begin

BMS2LAddrDatOut <= BMAddrReg;
BMS2LCntDatOut <= BMCnterReg;

AddrCntEn <= '1' when Ack = '1' and ((State = Rd and MS0Out_l_Int = '1') or State = RdEnd) else '0';
CnterCntEn <= '1' when Ack = '1' and State = Rd else '0';

CurrentAddr : BMAddrCnter
port map(
	l 	=> BMS2LAddrDatIn(26 downto 1),
	load 	=> BMS2LAddrWr,
	ce 	=> AddrCntEn,
	clk 	=> Clk,	
	ainit	=> Reset,
	q 	=>  BMAddrReg(26 downto 1)); 

BMAddrReg(0) <= '0';
BMAddrReg(31 downto 27) <= (others => '0');

CurrentCnt : BMCntCnter
port map(
	l 	=> BMS2LCntDatIn(24 downto 0),
	load 	=> BMS2LCntWr,
	ce 	=> CnterCntEn,
	clk 	=> Clk,	
	aclr	=> Reset,
	thresh0	=> BMCnterDone,
	thresh1	=> BMCnterAlmostDone,
	q 	=>  BMCnterReg(24 downto 0)); 

BMCnterReg(31 downto 25) <= (others => '0');

BMCnterBrstDis <= '1' when BMCnterReg(24 downto 2) = "00000000000000000000000" and BMCnterReg(1 downto 0) /= "11"  else '0';
BMAddrBrstDis <= '1' when BMAddrReg(8 downto 3) = "111111" and BMAddrReg(2 downto 1) /= "00" else '0';
BrstEnRegProc : process (Reset, Clk)
begin
	if Reset = '1' then
		BrstEn <= '0';
	elsif Rising_Edge(Clk) then
		if BMAddrBrstDis = '1' or BMCnterBrstDis = '1' then
			BrstEn <= '0';
		else
			BrstEn <= '1';
		end if;
	end if;
end process;

DataAvProc : process (Reset, Clk)
begin
	if Reset = '1' then
			DataAv <= '0';
	elsif Rising_Edge(Clk) then
		-- Wait till less than half full to do a transfer
			if BMCnterDone = '0' and FIFOWrCnt(8) = '0' and (BMCnterAlmostDone = '0' or State /= Rd)  then
				DataAv <= '1';
			else
				DataAv <= '0';
			end if;
	end if;
end process;

BusReq <= DataAv;

BusInUse <= BusInUse_Int;

FIFODat <= DataInReg;

AddrOut(30 downto 0) <= BMAddrReg(30 downto 0);
-- This keeps us from reading from this chips space
-- This chip ignores MS0 and the SDRAM controller ignores Addr 31
AddrOut(31) <= '1';
MS0Out_l <= MS0Out_l_Int;
RdLOut_l <= Rd_l;
RdHOut_l <= Rd_l;
BrstOut <= BrstOut_Int;

StateMachineProc :process (Reset, Clk)
begin
	if (Reset = '1')then
		state <= idle;
		Rd_l <= '1';
		MS0Out_l_Int <= '1';
		BrstOut_Int <= '0';
		BusInUse_Int <= '0';
		FIFOWr <= '0';
	elsif Rising_Edge(Clk) then 
		case state is
			when Idle => 
		     	if Ack = '1'  and DataAv = '1' and BusGnt = '1'then
					state <= Rd;		-- Data available and the bus is ours start a transaction
					MS0Out_l_Int <= '0';
					Rd_l <= '0';
					BusInUse_Int <= '1';
				else 
					state <= Idle;			-- Nothing to do!!!  Be a couchpotato and hang out
					MS0Out_l_Int <= '1';
					Rd_l <= '1';
					BusInUse_Int <= '0';
				end if;
				FIFOWr <= '0';
				BrstOut_Int <= '0';
			when Rd => -- Load a 64 bit word into data reg which drives the data bus
				if Ack = '1' then
					if BrstEn = '1' then 
						state <= Rd; 	-- Keep on bursting
						BrstOut_Int <= '1';
					else
						state <= RdEnd;		-- Terminate this burst
						BrstOut_Int <= '0';
					end if;
					FIFOWr <= MS0Out_l_Int;
					MS0Out_l_Int <= '1';
				else
					state <= Rd;			-- No Ack Hang out here
					FIFOWr <= '0';
					MS0Out_l_Int <= MS0Out_l_Int;
					BrstOut_Int <= BrstOut_Int;
				end if;
				Rd_l <= '0';
				BusInUse_Int <= '1';
			when RdEnd=>  -- Wait for Ack on last word of Write
				if Ack = '1' then
					state <= Idle;			-- Go to Idle to crank up again if necessary
					Rd_l <= '1';
					FIFOWr <= '1';
					BusInUse_Int <= '0';
				else
					state <= RdEnd;				-- No Ack or Not InSync  Stay put
					Rd_l <= '0';
					FIFOWr <= '0';
					BusInUse_Int <= '1';
				end if;
				MS0Out_l_Int <= '1';
				BrstOut_Int <= '0';
		end case;
	end if;
end process;

end SDtoF64_Arch;


