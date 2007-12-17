--*******************************************************************
--
--  Module :        Local_Bus_Interface_Wrapper
--  File Name :     Local_Bus_Interface_Wrapper.vhd
--  Version :       1.0
--  Code Author :   ATS, Red River
--  Date :          01 August 2003
--  Description :   This is a black box wrapper for the PLX PCI
--                  bridge on the SketchBoard
--
--*******************************************************************

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Local_Bus_Interface_Wrapper is generic (
    	NUM_CH : integer := 8;
    	CH_CNT_WIDTH : integer := 3;
    	ADDR_BITS : integer := 32);
    port (
        -- Local bus to PLX bridge
	 	LBClk :                in    std_logic;   
	    LBRstn :               in    std_logic;
		LB_user_req :          in    std_logic;
		LB_user_mult :         in    std_logic;
		LB_user_rdwr :         in    std_logic;
		LB_user_addr_vld :     in    std_logic;
		LB_user_stop :         out   std_logic;
		bus_select :           out   std_logic_vector (2 downto 0);
		LB_bus_cs :            out   std_logic;
		fifo_flag :            in    std_logic;	
		LBAD :        	       inout std_logic_vector (24 downto 0);
		D :                    inout std_logic_vector (63 downto 0);
		Int_to_FPGA :          in    std_logic;
		Int_to_Bridge :        out   std_logic;
        -- User interface
        PCIclk, Rst :         out std_logic; -- Clock and active high reset to the user logic
        PCIclk_2x :           out std_logic;
        Srst :             in std_logic;  -- soft reset
        Base_Addr :        in std_logic_vector (31 downto 0);  -- Base Address of DMA engine registers
        Local_Addr :       in std_logic_vector (31 downto 0);  -- Local address for a DMA
        DMA_SG_En :        in std_logic;  -- Enable Scatter gather mode for DMAs
        DMA_Channel :      in std_logic_vector (4 downto 0);  -- DMA Channel to use
        DMA_Go :           in std_logic;  -- Active high, signals when a DMA descriptor block is 
                                          -- valid (valid PCI_Addr, Local_Addr, DMA_Size, and 
                                          -- Desc_Addr
        DMA_Busy :         out std_logic; -- Indicates that a DMA is in progress
        DMA_Direction :    in std_logic;  -- used for DMAs to/from host logic 1 will cause DMA from 
        								  -- virtex to host computer.
        Local_Data_ADS_N : out std_logic; -- Active low address strobe, signals the start
                                          -- of a local bus transaction
        Local_Data_Rdy_N : in std_logic;  -- Signals when local bus write data has been stored or when
                                          -- local bus read data is valid on Local_Data_Out also increments
                                          -- address in burst transactions
        Local_Data_Rd_N :  out std_logic; -- signals a local bus read
        Local_Data_Wr_N :  out std_logic; -- signals a local bus write
        Local_Data_Addr :  out std_logic_vector (31 downto 0);   -- Local bus addrss
        Local_Data_In :    out std_logic_vector (63 downto 0);   -- Local bus data into FPGA
        Local_Data_Out :   inout std_logic_vector (63 downto 0); -- Local bus data out of FPGA
        PCI_int :          in std_logic;  -- User logic interrupt status, logic 0 = interrupt pending
        Last_Brst_Wd_N :   out std_logic; -- Active low signal last burst data, end of burst transaction
	    DMA_Ch0_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch1_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch2_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch3_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch4_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch5_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch6_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch7_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Last_Blk :         out   std_logic_vector (31 downto 0);
        DMA_Interrupt_Status : out std_logic); -- indicates that the PCI core has an interrupt pending
        
end Local_Bus_Interface_Wrapper;             

architecture structural of Local_Bus_Interface_Wrapper is

	signal debug_port : std_logic_vector (1 downto 0);
    --attribute BOX_TYPE : string;

--     constant NUM_CH : integer := 32; 
--     constant CH_CNT_WIDTH : integer := 5;
--     constant ADDR_BITS  : integer := 32;

    component Local_Bus_Interface generic (
    	NUM_CH : integer := NUM_CH;
    	CH_CNT_WIDTH : integer := CH_CNT_WIDTH;
    	ADDR_BITS : integer := ADDR_BITS);
    port (
        -- Local bus to PLX bridge
	 	LBClk :                in    std_logic;   
	    LBRstn :               in    std_logic;
		LB_user_req :          in    std_logic;
		LB_user_mult :         in    std_logic;
		LB_user_rdwr :         in    std_logic;
		LB_user_addr_vld :     in    std_logic;
		LB_user_stop :         out   std_logic;
		bus_select :           out   std_logic_vector (2 downto 0);
		LB_bus_cs :            out   std_logic;
		fifo_flag :            in    std_logic;	
		LBAD :        	       inout std_logic_vector (24 downto 0);
		D :                    inout std_logic_vector (63 downto 0);
		Int_to_FPGA :          in    std_logic;
		Int_to_Bridge :        out   std_logic;
        -- User interface
        PCIclk, Rst :         out std_logic;
        PCIclk_2x :           out std_logic;
        Srst :             in std_logic;  -- soft reset
        Base_Addr :        in std_logic_vector (31 downto 0);
        Local_Addr :       in std_logic_vector (31 downto 0);
        DMA_SG_En :        in std_logic;
        DMA_Channel :      in std_logic_vector (CH_CNT_WIDTH-1 downto 0);
        DMA_Go :           in std_logic;
        DMA_Busy :         out std_logic;
        DMA_Direction :    in std_logic;
        Local_Data_ADS_N : out std_logic;
        Local_Data_Rdy_N : in std_logic;
        Local_Data_Rd_N :  out std_logic;
        Local_Data_Wr_N :  out std_logic;
        Local_Data_Addr :  out std_logic_vector (31 downto 0);
        Local_Data_In :    out std_logic_vector (63 downto 0);
        Local_Data_Out :   inout std_logic_vector (63 downto 0);
        PCI_int :          in std_logic;
        Last_Brst_Wd_N :   out std_logic;
	    DMA_Ch0_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch1_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch2_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch3_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch4_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch5_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch6_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Ch7_BlkSize :      out   std_logic_vector (15 downto 0);
	    DMA_Last_Blk :         out   std_logic_vector (NUM_CH-1 downto 0);
	    DMA_Interrupt_Status : out std_logic
		);
    end component;
    
    --attribute BOX_TYPE of Local_Bus_Interface : component is "Black_BOX";

begin
    Local_Bus_Interface32 : Local_Bus_Interface generic map (
    	NUM_CH => NUM_CH,
    	CH_CNT_WIDTH => CH_CNT_WIDTH,
    	ADDR_BITS => ADDR_BITS)
    port map (
        -- Local bus to PLX bridge
	 	LBClk            => LBClk,
	    LBRstn           => LBRstn,
		LB_user_req      => LB_user_req,
		LB_user_mult     => LB_user_mult,
		LB_user_rdwr     => LB_user_rdwr,
		LB_user_addr_vld => LB_user_addr_vld,
		LB_user_stop     => LB_user_stop,
		bus_select       => bus_select,
		LB_bus_cs        => LB_bus_cs,
		fifo_flag => fifo_flag,
		LBAD             => LBAD,
		D                => D,
		Int_to_FPGA      => Int_to_FPGA,
		Int_to_Bridge    => Int_to_Bridge,
        -- User interface
        PCIclk              => PCIclk,
        PCIclk_2x           => PCIclk_2x,
        Srst             => Srst,
        Rst              => Rst,
        Base_Addr        => Base_Addr,
        Local_Addr       => Local_Addr,
        DMA_SG_En        => DMA_SG_En,
        DMA_Channel      => DMA_Channel (CH_CNT_WIDTH-1 downto 0),
        DMA_Go           => DMA_Go,
        DMA_Busy         => DMA_Busy,
        DMA_Direction    => DMA_Direction,
        Local_Data_ADS_N => Local_Data_ADS_N,
        Local_Data_Rdy_N => Local_Data_Rdy_N,
        Local_Data_Rd_N  => Local_Data_Rd_N,
        Local_Data_Wr_N  => Local_Data_Wr_N,
        Local_Data_Addr  => Local_Data_Addr,
        Local_Data_In    => Local_Data_In,
        Local_Data_Out   => Local_Data_Out,
        PCI_int          => PCI_int,
        Last_Brst_Wd_N   => Last_Brst_Wd_N,
	    DMA_Ch0_BlkSize  => DMA_Ch0_BlkSize,
	    DMA_Ch1_BlkSize  => DMA_Ch1_BlkSize,
	    DMA_Ch2_BlkSize  => DMA_Ch2_BlkSize,
	    DMA_Ch3_BlkSize  => DMA_Ch3_BlkSize,
	    DMA_Ch4_BlkSize  => DMA_Ch4_BlkSize,
	    DMA_Ch5_BlkSize  => DMA_Ch5_BlkSize,
	    DMA_Ch6_BlkSize  => DMA_Ch6_BlkSize,
	    DMA_Ch7_BlkSize  => DMA_Ch7_BlkSize,
	    DMA_Last_Blk     => DMA_Last_Blk (NUM_CH-1 downto 0),
        DMA_Interrupt_Status => DMA_Interrupt_Status
        );

end structural;