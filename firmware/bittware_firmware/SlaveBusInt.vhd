--  Copyright 2003, BittWare, Inc.
--  The user is hereby granted a non-exclusive license to
--  use and or modify this software provided that it runs
--  on BittWare hardware. Usage of this software on 
--  non-BittWare hardware without the express written 
--  permission of BittWare is strictly prohibited.
--
--    Ver.  Dates     Author  Changes                                
--    ----  --------  ------  ----------------------------
--    1.0    02/10/03	Kento		Initial coding
--    -------------------------------------------------------
--
--		Bus slave interface/address decoder logic for Remora IO. 
--
--		Address Map:
-- 		Chip base address is 0x00100000 on hammerhead bus (0x100000 (32 bit addr) offset from BAR2 from PCI Bus)
--			Starting at that address this implements 16 32 bit registers
--
--			The FIN/Hammerhead protcol dictates that Read are 1 wait state and writes are 0 wait state
--			Read decodes are performed on the raw address because they are multicycle
--			Write decodes are performed on the registers address because they are single cycle and have
--			trouble meeting timing for a full 32 bit address decode.

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity SlaveBusInt is
    Port ( 
				Clk				: in std_logic;
				Reset				: in std_logic;
				AddrIn			: in std_logic_vector(31 downto 0);
				DataIn			: in std_logic_vector(63 downto 0);
				RdLIn_l			: in std_logic;
				RdHIn_l			: in std_logic;
				WrLIn_l			: in std_logic;
				WrHIn_l			: in std_logic;
				AckOE_l			: out std_logic;
				DataLOE_l		: out std_logic;
				DataHOE_l		: out std_logic;
				Reg0Wr			: out std_logic;
				Reg0DatIn		: out std_logic_vector(31 downto 0);
				Reg0DatOut		: in std_logic_vector(31 downto 0);
				Reg1Wr			: out std_logic;
				Reg1DatIn		: out std_logic_vector(31 downto 0);
				Reg1DatOut		: in std_logic_vector(31 downto 0);
				Reg2Wr			: out std_logic;
				Reg2DatIn		: out std_logic_vector(31 downto 0);
				Reg2DatOut		: in std_logic_vector(31 downto 0);
				Reg3Wr			: out std_logic;
				Reg3DatIn		: out std_logic_vector(31 downto 0);
				Reg3DatOut		: in std_logic_vector(31 downto 0);
				Reg4Wr			: out std_logic;
				Reg4DatIn		: out std_logic_vector(31 downto 0);
				Reg4DatOut		: in std_logic_vector(31 downto 0);
				Reg5Wr			: out std_logic;
				Reg5DatIn		: out std_logic_vector(31 downto 0);
				Reg5DatOut		: in std_logic_vector(31 downto 0);
				Reg6Wr			: out std_logic;
				Reg6DatIn		: out std_logic_vector(31 downto 0);
				Reg6DatOut		: in std_logic_vector(31 downto 0);
				Reg7Wr			: out std_logic;
				Reg7DatIn		: out std_logic_vector(31 downto 0);
				Reg7DatOut		: in std_logic_vector(31 downto 0);
				Reg8Wr			: out std_logic;
				Reg8DatIn		: out std_logic_vector(31 downto 0);
				Reg8DatOut		: in std_logic_vector(31 downto 0);
				Reg9Wr			: out std_logic;
				Reg9DatIn		: out std_logic_vector(31 downto 0);
				Reg9DatOut		: in std_logic_vector(31 downto 0);
				RegAWr			: out std_logic;
				RegADatIn		: out std_logic_vector(31 downto 0);
				RegADatOut		: in std_logic_vector(31 downto 0);
				RegBWr			: out std_logic;
				RegBDatIn		: out std_logic_vector(31 downto 0);
				RegBDatOut		: in std_logic_vector(31 downto 0);
				RegCWr			: out std_logic;
				RegCDatIn		: out std_logic_vector(31 downto 0);
				RegCDatOut		: in std_logic_vector(31 downto 0);
				RegDWr			: out std_logic;
				RegDDatIn		: out std_logic_vector(31 downto 0);
				RegDDatOut		: in std_logic_vector(31 downto 0);
				RegEWr			: out std_logic;
				RegEDatIn		: out std_logic_vector(31 downto 0);
				RegEDatOut		: in std_logic_vector(31 downto 0);
				RegFWr			: out std_logic;
				RegFDatIn		: out std_logic_vector(31 downto 0);
				RegFDatOut		: in std_logic_vector(31 downto 0);
				DataInReg		: out std_logic_vector(63 downto 0);
				RegDataOut		: out std_logic_vector(63 downto 0)
			);
end SlaveBusInt;

architecture Behavioral of SlaveBusInt is

signal AddrInReg_Int	: std_logic_vector(31 downto 0);
signal DataInReg_Int	: std_logic_vector(63 downto 0);
signal WrLInReg_l		: std_logic;
signal WrHInReg_l		: std_logic;

signal FPGAWrSel		: std_logic;

begin

	-- With the proper project settings (Pack I/O registers into IOBs under synthesize/xilinx specific)
	-- these registers reside in the IOB's and consume no global chip resources
	RegInputProc : process (Reset, Clk)
	begin
		if Reset = '1' then
			WrLInReg_l <= '1';
			WrHInReg_l <= '1';
		elsif Rising_Edge(Clk) then
			WrLInReg_l <= WrLIn_l;
			WrHInReg_l <= WrHIn_l;
			DataInReg_Int <= DataIn;
			AddrInReg_Int <= AddrIn;
		end if;
	end process;

	DataInReg <= DataInReg_Int;

	-- Chip select decode.  The Virtex2 is mapped starting at location 0x00100000
	FPGAWrSel <= '1' when AddrInReg_Int(31 downto 4) = X"0010000" else '0';

	AckOE_l <= '0' when AddrIn(31 downto 4) = X"0010000" and (RdLIn_l = '0' or RdHIn_l = '0' or WrLIn_l = '0' or WrHIn_l = '0') else '1';
	DataLOE_l <= '0' when AddrIn(31 downto 4) = X"0010000" and RdLIn_l = '0' else '1';
	DataHOE_l <= '0' when AddrIn(31 downto 4) = X"0010000" and RdHIn_l = '0' else '1';

	Reg0Wr <= '1' when WrLInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "000" else '0';
	Reg0DatIn <= DataInReg_Int(31 downto 0);
	Reg1Wr <= '1' when WrHInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "000" else '0';
	Reg1DatIn <= DataInReg_Int(63 downto 32);
	Reg2Wr <= '1' when WrLInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "001" else '0';
	Reg2DatIn <= DataInReg_Int(31 downto 0);
	Reg3Wr <= '1' when WrHInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "001" else '0';
	Reg3DatIn <= DataInReg_Int(63 downto 32);
	Reg4Wr <= '1' when WrLInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "010" else '0';
	Reg4DatIn <= DataInReg_Int(31 downto 0);
	Reg5Wr <= '1' when WrHInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "010" else '0';
	Reg5DatIn <= DataInReg_Int(63 downto 32);
	Reg6Wr <= '1' when WrLInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "011" else '0';
	Reg6DatIn <= DataInReg_Int(31 downto 0);
	Reg7Wr <= '1' when WrHInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "011" else '0';
	Reg7DatIn <= DataInReg_Int(63 downto 32);
	Reg8Wr <= '1' when WrLInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "100" else '0';
	Reg8DatIn <= DataInReg_Int(31 downto 0);
	Reg9Wr <= '1' when WrHInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "100" else '0';
	Reg9DatIn <= DataInReg_Int(63 downto 32);
	RegAWr <= '1' when WrLInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "101" else '0';
	RegADatIn <= DataInReg_Int(31 downto 0);
	RegBWr <= '1' when WrHInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "101" else '0';
	RegBDatIn <= DataInReg_Int(63 downto 32);
	RegCWr <= '1' when WrLInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "110" else '0';
	RegCDatIn <= DataInReg_Int(31 downto 0);
	RegDWr <= '1' when WrHInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "110" else '0';
	RegDDatIn <= DataInReg_Int(63 downto 32);
	RegEWr <= '1' when WrLInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "111" else '0';
	RegEDatIn <= DataInReg_Int(31 downto 0);
	RegFWr <= '1' when WrHInReg_l = '0' and FPGAWrSel = '1' and AddrInReg_Int(3 downto 1) = "111" else '0';
	RegFDatIn <= DataInReg_Int(63 downto 32);

-- These two will instantiate as 2 32 bit wide MUX8_1E's
-- It will be optimized down to much less as many of the bits are
-- unused.
	LoWord : process (AddrIn, RdLIn_l, Reg0DatOut, Reg2DatOut, Reg4DatOut, Reg6DatOut, Reg8DatOut, RegADatOut, RegCDatOut, RegEDatOut)
	begin
		if (AddrIn(31 downto 4) = X"0010000" and RdLIn_l = '0') then
			case AddrIn(3 downto 1) is
				when "000" => RegDataOut(31 downto 0) <= Reg0DatOut;
				when "001" => RegDataOut(31 downto 0) <= Reg2DatOut;
				when "010" => RegDataOut(31 downto 0) <= Reg4DatOut;
				when "011" => RegDataOut(31 downto 0) <= Reg6DatOut;
				when "100" => RegDataOut(31 downto 0) <= Reg8DatOut;
				when "101" => RegDataOut(31 downto 0) <= RegADatOut;
				when "110" => RegDataOut(31 downto 0) <= RegCDatOut;
				when "111" => RegDataOut(31 downto 0) <= RegEDatOut;
				when others => NULL;
			end case;
		else
			RegDataOut(31 downto 0) <= (others => '0');
		end if;
	end process;

	HiWord : process (AddrIn, RdHIn_l, Reg1DatOut, Reg3DatOut, Reg5DatOut, Reg7DatOut, Reg9DatOut, RegBDatOut, RegDDatOut, RegFDatOut)
	begin
		if (AddrIn(31 downto 4) = X"0010000" and RdHIn_l = '0') then
			case AddrIn(3 downto 1) is
				when "000" => RegDataOut(63 downto 32) <= Reg1DatOut;
				when "001" => RegDataOut(63 downto 32) <= Reg3DatOut;
				when "010" => RegDataOut(63 downto 32) <= Reg5DatOut;
				when "011" => RegDataOut(63 downto 32) <= Reg7DatOut;
				when "100" => RegDataOut(63 downto 32) <= Reg9DatOut;
				when "101" => RegDataOut(63 downto 32) <= RegBDatOut;
				when "110" => RegDataOut(63 downto 32) <= RegDDatOut;
				when "111" => RegDataOut(63 downto 32) <= RegFDatOut;
				when others => NULL;
			end case;
		else
			RegDataOut(63 downto 32) <= (others => '0');
		end if;
	end process;

end Behavioral;

