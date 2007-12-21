--  Copyright 2003, BittWare, Inc.
--  The user is hereby granted a non-exclusive license to
--  use and or modify this software provided that it runs
--  on BittWare hardware. Usage of this software on 
--  non-BittWare hardware without the express written 
--  permission of BittWare is strictly prohibited.
--
--    Ver.  Dates     Author  Changes                                
--    ----  --------  ------  ----------------------------
--    1.0    02/13/03	Kento		Initial coding
--    -------------------------------------------------------
--
--		Implements logic to for the misc and stat registers. 
--
--		Address Map:
-- 		Chip base address is 0x00100000
--
--			The registers handled in this module are:
--			0x0 32 bit Miscelaneous Register  (Default Value = 0x0)
--				B0 Drives HBGEn
--				B4 Drives UserLED(0) when B5 is lo
--				B5 Selects between B4 (lo) and UserSw(0) (hi) as a source for UserLED(0)
--				B6 Drives UserLED(1) when B7 is lo
--				B7 Selects between B6 (lo) and UserSw(1) (hi) as a source for UserLED(1)
--				B8 When 1 is written to this bit it disables the driver on xPA_SCL
--				B8 When 0 is written to this bit it enables the driver on xPA_SCL and drives it to 0
--				B9 When 1 is written to this bit it disables the driver on xPA_SDA
--				B9 When 0 is written to this bit it enables the driver on xPA_SDA and drives it to 0
--				Unused bits are still readable and writable
--			0x1 32 bit read only status register
--				B0 to B3 return the status of of the corner pins of the FPIO connector
--				B0 = Pin 35, B1 = Pin 68, B2 = Pin 1 and B3 = Pin 34
--				B7 to B4 return the status of UserSw(0) to UserSw(3) Respectively
--				Reading a 0 means the Switch is On
--				B8 returns the status of xPA_SCL
--				B9 returns the status of xPA_SDA
--				B16 is the empty flag for the Link0 to SDRAM FIFO
--				B17 is the full flag for the Link0 to SDRAM FIFO
--				B18 is the empty flag for the SDRAM to Link1
--				B19 is the full flag for the SDRAM to Link1
--				B20 is the empty flag for the Link2 to Link3 FIFO
--				B21 is the full flag for the Link2 to Link3 FIFO
--				B22 is the empty flag for the Link3 to Link2 FIFO
--				B23 is the full flag for the Link3 to Link2 FIFO
--				B27 to 24 is the chip Build ID
--				B31 to 28 is the chip Rev ID
--				Unused bits return a 0
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity MiscStatRegs is
    Port ( 
				Clk				: in std_logic;
				Reset				: in std_logic;
				MiscRegWr		: in std_logic;
				MiscRegDatIn	: in std_logic_vector(31 downto 0);
				L0SDEF			: in std_logic;
				L0SDFF			: in std_logic;
				SDL1EF			: in std_logic;
				SDL1FF			: in std_logic;
				L2L3EF			: in std_logic;
				L2L3FF			: in std_logic;
				L3L2EF			: in std_logic;
				L3L2FF			: in std_logic;
				FPIOCorner		: in std_logic_vector(3 downto 0);
				UserSw			: in std_logic_vector(3 downto 0);
				PASClIn			: in std_logic;
				PASDAIn			: in std_logic;
				PASClOE_l		: out std_logic;
				PASDAOE_l		: out std_logic;
				HBGEn_l			: out std_logic;
				HBGSyncPre		: out std_logic;
				MiscRegDatOut	: out std_logic_vector(31 downto 0);
				StatRegDatOut	: out std_logic_vector(31 downto 0);
				UserLED			: out std_logic_vector(1 downto 0)
			);
end MiscStatRegs;

architecture Behavioral of MiscStatRegs is

signal MiscReg	: std_logic_vector(31 downto 0);
signal StatReg	: std_logic_vector(31 downto 0);
-- This keeps track of what version we are
CONSTANT RevID	: std_logic_vector(3 downto 0) := "0000";
CONSTANT BuildID	: std_logic_vector(3 downto 0) := "0001";
-- Build 1 fixes swappes Clk/Ack lines

begin

	RegWrProc : process (Reset, Clk)
	begin
		if Reset = '1' then
			MiscReg <= (others => '0');
		elsif Rising_Edge(Clk) then
			if MiscRegWr = '1' then
				MiscReg <= MiscRegDatIn;
			end if;
		end if;
	end process;


HBGEn_l <= not MiscReg(0);
HBGSyncPre <= MiscRegWr and MiscRegDatIn(0) and not MiscReg(0);

UserLED(0) <= UserSw(0) when MiscReg(5) = '1' else MiscReg(4);
UserLED(1) <= UserSw(1) when MiscReg(7) = '1' else MiscReg(6);

PASCLOE_l <= MiscReg(8);
PASDAOE_l <= MiscReg(9);

StatReg(3 downto 0) <= FPIOCorner(3 downto 0);
StatReg(7 downto 4) <= UserSw(3 downto 0);

StatReg(8) <= PASCLIn;
StatReg(9) <= PASDAIn;

StatReg(16)  <= L0SDEF;
StatReg(17)  <= L0SDFF;
StatReg(18) <= SDL1EF;
StatReg(19) <= SDL1FF;
StatReg(20) <= L2L3EF;
StatReg(21) <= L2L3FF;
StatReg(22) <= L3L2EF;
StatReg(23) <= L3L2FF;

StatReg(15 downto 10) <= (others => '0');

StatReg(31 downto 28) <= RevID;
StatReg(27 downto 24) <= BuildID;

MiscRegDatOut <= MiscReg;
StatRegDatOut <= StatReg;

end Behavioral;

