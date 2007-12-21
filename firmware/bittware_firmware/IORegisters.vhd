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
--		Implements logic to control IO connectors for Remora IO. 
--
--		Address Map:
-- 		Chip base address is 0x00100000
--
--			The registers handled in this module are:
--			0x8 and 0x9 64 bit front panel connector Read/Write register (Default Value = 0x0)
--				A write to this location writes to the output register
--				A read of this location returns the value on the connector pins
--				Bits 0 to 31 correspond to pins 36 to 67 repectively on the connector
--				Bits 32 to 63 correspond to pins 2 to 33  repectively  on the connector
--				pins 1, 34, 35 and 68 can be read in the status register
--				to prevent potential damage to the board these pins can not
--				be driven in this example
--			0xA and 0xB 64 bit front panel connector drive enable register  (Default Value = 0x0)
--				A write to this location writes to the output enable register
--				A read of this location returns the output enable register
--				writing a 0 to a bit disables the corresponding driver
--				writing a 1 to a bit enables the corresponding driver 
--				and outputs the corresponding bit in the output register
--				Bits 0 to 31 correspond to pins 36 to 67 repectively on the connector
--				Bits 32 to 63 correspond to pins 2 to 33  repectively  on the connector
--				pins 1, 34, 35 and 68 can be read in the status register
--				to prevent potential damage to the board these pins can not
--				be driven in this example
--			0xC 32 bit Header 1 connector Read/Write register (Default Value = 0x0)
--				A write to this location writes to the output register
--				A read of this location returns the value on the connector pins
--				Bits 0 to 15 correspond to pins 2,4,6,8,10,12,14,16,18,20,22,24,26,28,30 and 32
--				(Data) respectively on the connector
--				Bit 16 corresponds to pin 21 (Ack) on the connector
--				This pin is also tied to flag0 going to the FIN
--				Bit 17 corresponds to pin 1 (Dir) on the connector 
--				Bit 18 corresponds to pin 11 (Clk) on the connector
--				When the Dir pin is hi, the 16 Data pins act as outputs from the Virtex
--				When the Dir pin is lo, the 16 Data pins act as inputs to the Virtex
--				The functionality of Dir is overridden on a per pin basis by
--          a 1 in the drive enable register.
--				The dir pin is pulled up
--			0xD 32 bit Header 1 connector drive enable register (Default Value = 0x0)
--				A write to this location writes to the output enable register
--				A read of this location returns the output enable register
--				writing a 0 to a makes the pin an input to the Virtex
--				writing a 1 to a makes the pin an output from the Virtex
--				and outputs the corresponding bit in the output register
--				Bits 0 to 15 correspond to pins 2,4,6,8,10,12,14,16,18,20,22,24,26,28,30 and 32
--				respectively on the connector
--				Bit 16 corresponds to pin 21 (Ack) on the connector 
--				Bit 17 corresponds to pin 1 (Dir) on the connector 
--				Bit 18 corresponds to pin 11 (Clk) on the connector
--				Bit 19 also corresponds to pin 11 (Clk) on the connector
--				When bit 19 and bit 18 are set, pin 11 outputs Clk instead of the output register bit 18
--			0xE 32 bit Header 2 connector Read/Write register (Default Value = 0x0)
--				A write to this location writes to the output register
--				A read of this location returns the value on the connector pins
--				Bits 0 to 15 correspond to pins 2,4,6,8,10,12,14,16,18,20,22,24,26,28,30 and 32
--				(Data) respectively on the connector
--				Bit 16 corresponds to pin 21 (Ack) on the connector
--				This pin is also tied to flag1 going to the FIN
--				Bit 17 corresponds to pin 1 (Dir) on the connector 
--				Bit 18 corresponds to pin 11 (Clk) on the connector
--				When the Dir pin is lo, the 16 Data pins act as outputs from the Virtex
--				When the Dir pin is hi, the 16 Data pins act as inputs to the Virtex
--				The functionality of Dir is overridden on a per pin basis by
--          a 1 in the drive enable register
--				The dir pin is pulled up
--			0xF 32 bit Header 2 connector drive enable register (Default Value = 0x0)
--				A write to this location writes to the output enable register
--				A read of this location returns the output enable register
--				writing a 0 to a makes the pin an input to the Virtex
--				writing a 1 to a makes the pin an output from the Virtex
--				and outputs the corresponding bit in the output register
--				Bits 0 to 15 correspond to pins 2,4,6,8,10,12,14,16,18,20,22,24,26,28,30 and 32
--				respectively on the connector
--				Bit 16 corresponds to pin 21 (Ack) on the connector 
--				Bit 17 corresponds to pin 1 (Dir) on the connector 
--				Bit 18 corresponds to pin 11 (Clk) on the connector
--				Bit 19 also corresponds to pin 11 (Clk) on the connector
--				When bit 19 and bit 18 are set, pin 11 outputs Clk instead of the output register bit 18
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity IORegisters is
    Port ( 
				Clk				: in std_logic;
				Reset				: in std_logic;
				FPIOIn			: in std_logic_vector(63 downto 0);
				Hdr1IOIn			: in std_logic_vector(18 downto 0);
				Hdr2IOIn			: in std_logic_vector(18 downto 0);
				FPIORWRegWrL	: in std_logic;
				FPIORWRegWrH	: in std_logic;
				FPIORWDatInL	: in std_logic_vector(31 downto 0);
				FPIORWDatInH	: in std_logic_vector(31 downto 0);
				FPIORWDatOutL	: out std_logic_vector(31 downto 0);
				FPIORWDatOutH	: out std_logic_vector(31 downto 0);
				FPIOEnRegWrL	: in std_logic;
				FPIOEnRegWrH	: in std_logic;
				FPIOEnDatInL	: in std_logic_vector(31 downto 0);
				FPIOEnDatInH	: in std_logic_vector(31 downto 0);
				FPIOEnDatOutL	: out std_logic_vector(31 downto 0);
				FPIOEnDatOutH	: out std_logic_vector(31 downto 0);
				Hdr1IORWRegWr	: in std_logic;
				Hdr1IORWDatIn	: in std_logic_vector(31 downto 0);
				Hdr1IORWDatOut	: out std_logic_vector(31 downto 0);
				Hdr1IOEnRegWr	: in std_logic;
				Hdr1IOEnDatIn	: in std_logic_vector(31 downto 0);
				Hdr1IOEnDatOut	: out std_logic_vector(31 downto 0);
				Hdr2IORWRegWr	: in std_logic;
				Hdr2IORWDatIn	: in std_logic_vector(31 downto 0);
				Hdr2IORWDatOut	: out std_logic_vector(31 downto 0);
				Hdr2IOEnRegWr	: in std_logic;
				Hdr2IOEnDatIn	: in std_logic_vector(31 downto 0);
				Hdr2IOEnDatOut	: out std_logic_vector(31 downto 0);
				FPIOOut			: out std_logic_vector(63 downto 0);
				FPIOOE_l			: out std_logic_vector(63 downto 0);
				Hdr1IOOut		: out std_logic_vector(18 downto 0);
				Hdr1IOOE_l		: out std_logic_vector(18 downto 0);
				Hdr2IOOut		: out std_logic_vector(18 downto 0);
				Hdr2IOOE_l		: out std_logic_vector(18 downto 0)
			);
end IORegisters;

architecture Behavioral of IORegisters is

signal FPIOOE		: std_logic_vector(63 downto 0);
signal Hdr1AckOut	: std_logic;
signal Hdr1DirOut	: std_logic;
signal Hdr1ClkOut	: std_logic;
signal Hdr1ClkEn	: std_logic;
signal Hdr1IOOE	: std_logic_vector(18 downto 0);
signal Hdr1DirOE	: std_logic_vector(15 downto 0);
signal Hdr2AckOut	: std_logic;
signal Hdr2DirOut	: std_logic;
signal Hdr2ClkOut	: std_logic;
signal Hdr2IOOE	: std_logic_vector(18 downto 0);
signal Hdr2ClkEn	: std_logic;
signal Hdr2DirOE	: std_logic_vector(15 downto 0);

begin

	RegWrProc : process (Reset, Clk)
	begin
		if Reset = '1' then
			FPIOOut	<= (others => '0');
			FPIOOE	<= (others => '0');
			Hdr1IOOut(15 downto 0) <= (others => '0');
			Hdr1AckOut	<= '0';
			Hdr1DirOut	<= '0';
			Hdr1ClkOut	<= '0';
			Hdr1IOOE 	<= (others => '0');
			Hdr1ClkEn	<= '0';
			Hdr2IOOut(15 downto 0) <= (others => '0');
			Hdr2AckOut	<= '0';
			Hdr2DirOut	<= '0';
			Hdr2ClkOut	<= '0';
			Hdr2IOOE 	<= (others => '0');
			Hdr2ClkEn	<= '0';
		elsif Rising_Edge(Clk) then
			if FPIORWRegWrL = '1' then
				FPIOOut(31 downto 0) <= FPIORWDatInL;
			end if;
			if FPIORWRegWrH = '1' then
				FPIOOut(63 downto 32) <= FPIORWDatInH;
			end if;
			if FPIOEnRegWrL = '1' then
				FPIOOE(31 downto 0) <= FPIOEnDatInL;
			end if;
			if FPIOEnRegWrH = '1' then
				FPIOOE(63 downto 32) <= FPIOEnDatInH;
			end if;
			if Hdr1IORWRegWr = '1' then
				Hdr1IOOut(15 downto 0) <= Hdr1IORWDatIn(15 downto 0);
				Hdr1AckOut	<= Hdr1IORWDatIn(16);
				Hdr1DirOut	<= Hdr1IORWDatIn(17);
				Hdr1ClkOut	<= Hdr1IORWDatIn(18);
			end if;

			if Hdr1IOEnRegWr = '1' then
				Hdr1IOOE(18 downto 0)	<= Hdr1IOEnDatIn(18 downto 0);
				Hdr1ClkEn	<= Hdr1IOEnDatIn(19);
			end if;
			if Hdr2IORWRegWr = '1' then
				Hdr2IOOut(15 downto 0) <= Hdr2IORWDatIn(15 downto 0);
				Hdr2AckOut	<= Hdr2IORWDatIn(16);
				Hdr2DirOut	<= Hdr2IORWDatIn(17);
				Hdr2ClkOut	<= Hdr2IORWDatIn(18);
			end if;

			if Hdr2IOEnRegWr = '1' then
				Hdr2IOOE(18 downto 0)	<= Hdr2IOEnDatIn(18 downto 0);
				Hdr2ClkEn	<= Hdr2IOEnDatIn(19);
			end if;
		end if;
	end process;

-- A 1 in the register enables the output!!
FPIOOE_l <= not FPIOOE;

Hdr1IOOut(16) <= Hdr1AckOut;
Hdr1IOOut(17) <= Hdr1DirOut;
Hdr1IOOut(18) <= not Clk when Hdr1ClkEn = '1' else Hdr1ClkOut;
Hdr1DirOE <= (others => Hdr1IOIn(17));
Hdr1IOOE_l(15 downto 0) <= not Hdr1IOOE(15 downto 0) and Hdr1DirOE;
Hdr1IOOE_l(18 downto 16) <= not Hdr1IOOE(18 downto 16);

Hdr2IOOut(16) <= Hdr2AckOut;
Hdr2IOOut(17) <= Hdr2DirOut;
Hdr2IOOut(18) <= not Clk when Hdr2ClkEn = '1' else Hdr2ClkOut;
Hdr2DirOE <= (others => Hdr2IOIn(17));
Hdr2IOOE_l(15 downto 0) <= not Hdr2IOOE(15 downto 0) and Hdr2DirOE;
Hdr2IOOE_l(18 downto 16) <= not Hdr2IOOE(18 downto 16);

FPIORWDatOutL <= FPIOIn(31 downto 0);
FPIORWDatOutH <= FPIOIn(63 downto 32);
FPIOEnDatOutL <= FPIOOE(31 downto 0);
FPIOEnDatOutH <= FPIOOE(63 downto 32);

Hdr1IORWDatOut(18 downto 0) <= Hdr1IOIn;
Hdr1IORWDatOut(31 downto 19) <= (others => '0');
Hdr1IOEnDatOut(19 downto 0) <= Hdr1ClkEn & Hdr1IOOE;
Hdr1IOEnDatOut(31 downto 20) <= (others => '0');

Hdr2IORWDatOut(18 downto 0) <= Hdr2IOIn;
Hdr2IORWDatOut(31 downto 19) <= (others => '0');
Hdr2IOEnDatOut(19 downto 0) <= Hdr2ClkEn & Hdr2IOOE;
Hdr2IOEnDatOut(31 downto 20) <= (others => '0');


end Behavioral;

