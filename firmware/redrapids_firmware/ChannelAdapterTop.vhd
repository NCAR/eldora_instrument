----********************************************************************************************
-- Red Rapids
-- 797 N. Grove Rd, Suite 101
-- Richardson, TX 75081
-- www.redrapids.com (support@redrapids.com)
--
-- File: CA_LBExampleTop.vhd - Release DSK-320-003-R05 - Thu Dec 21 17:19:56 CST 2006
-- 

--
-- File Name 		:  	CA_ExampleTop.vhd
-- Author      		:  	PTJ & ATS
-- Description 		: 	This is the top level module for the ChannelAdapter (Model 314/320/316).
--				  			It shows an example of the 4 Channel LB interface connected to 
--							4 counters which are advanced each time counter is hit via a DMA 
--							transfer from the LB core.
--
-- Rev History  :
--				
--						1.0.1 (10032006a)	- Linked agaist 		
--						1.0.0 (08282006a)	- Intial release
--						Tom's Revisions
--						1.1.0 - FIFO Status includes both FIFOs for each channel
--								- FF Flush signals are no longer self clearing. Allows time for FIFOs to reset.
--
--
-- Requires		:	
--						
--						Local_Bus_Interface_Wrapper.vhd, Local_Bus_Interface.ngc addr_mem.xco & tx_dma_fifo -LB Interface
--						**Note** PWB R00 required D02 of LB interface.
--*******************************************************************************************
 
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use IEEE.numeric_std.all; 

Library UNISIM; 
use UNISIM.Vcomponents.all; 

entity ChannelAdapter_Top is 
    port(
	 -- Local bus to bridge
		LBClk :              in    std_logic;   
 		LBRstn :             in    std_logic;
		LB_user_req :        in    std_logic;
		LB_user_mult :       in    std_logic;
		LB_user_rdwr :       in    std_logic;
		LB_user_addr_vld :	in    std_logic;
		LB_user_stop :       out   std_logic;
		bus_select :         out   std_logic_vector (2 downto 0);
		LB_bus_cs :          out   std_logic;
		fifo_flag :          in    std_logic;	
		LBAD :        	      inout std_logic_vector (24 downto 0);
		D :                  inout std_logic_vector (63 downto 0);
		Int_to_FPGA :        in    std_logic;
		Int_to_Bridge :      out   std_logic;
		
		--ADC Interfaces
	   sampleclk_p :     in std_logic;    -- ADC sample clk AD9512 part (LVDS 2.5V)
      sampleclk_n :     in std_logic;
		
		adca_in :         in std_logic_vector (13 downto 0); -- ADC A data
      adcb_in :         in std_logic_vector (13 downto 0); -- ADC B data
      adcc_in :         in std_logic_vector (13 downto 0); -- ADC C data
      adcd_in :         in std_logic_vector (13 downto 0); -- ADC D data
      adca_or :         in std_logic;    -- ADC A over range, Active High
      adcb_or :         in std_logic;    -- ADC B over range, Active High
      adcc_or :         in std_logic;    -- ADC C over range, Active High
      adcd_or :         in std_logic;    -- ADC D over range, Active High
		
		-- GPIO
		gpio :				in std_logic_vector(3 downto 0));
		
end ChannelAdapter_Top;           
                    
architecture behavioral of ChannelAdapter_Top is    

 
---------------------------------------------------------------------------------
-------------------- Signal Declarations ----------------------------------------
---------------------------------------------------------------------------------

   -- constant signals defined this way for modelsim
   signal DMABaseAddr  : std_logic_vector (31 downto 0);  --Adr start of DMA Core / LB interface 0x0
   signal UserBaseAddr : std_logic_vector (31 downto 0);    --Adr start of user space (0x800)
   
    
	 
	-- User Logic Registers accessable via LB 
	signal ctl_reg   			: std_logic_vector (28 downto 0);  		--Control Reg
	signal mask_reg   		: std_logic_vector (31 downto 0);  		--Intr Mask Reg
	
    
   -- LB Register Rd/Wr selection lines
   signal rev_seln   : std_logic;		--0x800
	signal ctl_seln   : std_logic;		--0x80C
	signal stat_seln	: std_logic;		--0x810
	signal mask_seln	: std_logic;		--0x814
	-- DMA Channels
	signal ch0_seln   : std_logic;		--0x900
	signal ch1_seln   : std_logic;		--0x908
	signal ch2_seln   : std_logic;		--0x910
	signal ch3_seln   : std_logic;		--0x918
	signal ch4_seln   : std_logic;		--0x920
	signal ch5_seln   : std_logic;		--0x928
	signal ch6_seln   : std_logic;		--0x930
	signal ch7_seln   : std_logic;		--0x938
	-- Filter Coefficient
	signal k_addr_seln : std_logic;  	--0xA00
	signal k_data_seln : std_logic;  	--0xA04
	signal k_read_seln : std_logic;  	--0xA08
	signal k_write_seln: std_logic;  	--0xA0C
	signal g_addr_seln : std_logic;  	--0xA10
	signal g_data_seln : std_logic;  	--0xA14
	signal g_read_seln : std_logic;  	--0xA18
	signal g_write_seln: std_logic;  	--0xA1C
	-- Clock Decimation
	signal dec_seln	 : std_logic;		--0xA20
	signal dec_rst_seln: std_logic;		--0xA24
	-- Multi Timer
	signal mt_addr_seln: std_logic;		--0xA30
	signal mt_data_seln: std_logic;		--0xA34
	signal mt_wr_seln  : std_logic;		--0xA38
	-- Pulse Pair Processor
	signal pp_m_seln	 : std_logic;		--0xA40
	signal pp_n_seln	 :	std_logic;		--0xA44
	signal iq_index_seln: std_logic;		--0xA48
	signal iq_length_seln: std_logic;	--0xA4C
	signal pp_rst_seln : std_logic;     --0xA54
	-- Gate Splitter
	signal gs_dprt_seln: std_logic;		--0xA50
	-- Timing Switch Internal/External  
	signal timing_seln : std_logic;		--0xA58
	-- SVN Revision #
	signal svn_rev_seln: std_logic;		--0xA5C
	
	
	--Status and Interrupt signals
	signal status_reg	 	: std_logic_vector(31 downto 0);	--Status Reg
	signal status_regR	: std_logic_vector(31 downto 0); --User_clk registered version of status_reg
	signal status_regRR	: std_logic_vector(31 downto 0); --User_clk registered version of status_reg
	signal status_regRRR	: std_logic_vector(31 downto 0); --User_clk registered version of status_reg
	
	
	--Addr decoder signals
	signal addr_decode_en : std_logic;  --Logic 1 means we need to decode the curr LB adr 
	signal user_addr_hit  : std_logic;  --Logic Access located in user space
	signal Local_Data_Rdy : std_logic;  --ACK of User Rd/Wr, 
	
	
	--DMA signals
	signal cur_ch         : std_logic_vector(2 downto 0); --Selects DMA Ch to use
	signal cur_ch_ext     : std_logic_vector(4 downto 0); --Ch ch ext to 32 channels
	signal cur_src_addr   : std_logic_vector(31 downto 0); --Rd location for DMA ch
	signal dma_go         : std_logic;  --Signal to the core to start a DMA for the cur ch
	signal ch_change_strb : std_logic;  --Set to 1 when new ch is selected, set to 0 after DMA starts
	signal dma_busyR		 : std_logic;	--Reg version of dma_Busy, used to create dma_start and dma_done 
	signal dma_busyRR	  	 : std_logic;
	signal dma_done       : std_logic;  --Detects end of DMA w falling edge of dma_busy 
	signal dma_start      : std_logic;  --Detects start of DMA with rising egde of dma_busy
	signal dma_ch_adv     : std_logic;  --Logic 1 means the cur dma channel should be advanced
	 
	signal ch0_src_addr  : std_logic_vector(31 downto 0);  --Locations the DMA core should read from for each ch
	signal ch1_src_addr  : std_logic_vector(31 downto 0);
	signal ch2_src_addr  : std_logic_vector(31 downto 0);
	signal ch3_src_addr  : std_logic_vector(31 downto 0);
	signal ch4_src_addr  : std_logic_vector(31 downto 0); 
	signal ch5_src_addr  : std_logic_vector(31 downto 0);
	signal ch6_src_addr  : std_logic_vector(31 downto 0);
	signal ch7_src_addr  : std_logic_vector(31 downto 0);
		
	
	 --DMA Core interface signals 
	signal user_clk             : std_logic; --LB clock
   signal reset                : std_logic;	--reset
   signal DMA_Busy             : std_logic;	--Logic 1 for DMA in progress for current channel
   signal Local_Data_ADS_N     : std_logic; --Core has a rd or wr pending (Active Low)
   signal Local_Data_Rdy_N     : std_logic; --User logic ACKs wr or marks Rd data valid (Active Low)
   signal rdl                  : std_logic;	--Current adr is for a Rd (Active Low)
   signal wrl                  : std_logic; --Current adr/data is for a Wr (Active Low)
   signal Local_Data_Addr      : std_logic_vector (31 downto 0); --Local Adr
   signal Local_Data_In        : std_logic_vector (63 downto 0);	--Data from core (from writes)
   signal Local_Data_Out       : std_logic_vector (63 downto 0); --Data to core (for reads)
   signal Last_Brst_Wd_N       : std_logic;	--Active low signal for last rd in a burst transfer
   signal dma0_block_size      : std_logic_vector (15 downto 0);	
   signal dma1_block_size      : std_logic_vector (15 downto 0);
   signal dma2_block_size      : std_logic_vector (15 downto 0);
   signal dma3_block_size      : std_logic_vector (15 downto 0);
	signal dma4_block_size      : std_logic_vector (15 downto 0);	
   signal dma5_block_size      : std_logic_vector (15 downto 0);
   signal dma6_block_size      : std_logic_vector (15 downto 0);
   signal dma7_block_size      : std_logic_vector (15 downto 0);
   signal DMA_Interrupt_Status  : std_logic;  --1 for DMA core has an intr pending
	
	
	-- ADC and Filter Clks
	signal adc_clk						: std_logic; -- ADC Sample Clk
	signal filter_clk					: std_logic; -- ADCx3 Filter Clk
	signal adc_dcmrst					: std_logic; -- ADC DCM Reset
	signal adc_dcmlocked				: std_logic; -- ADC DCM Locked
	
	
	--System Enable
	signal adc_enable			  : std_logic;

	
	-- FIFO
   signal ff_flush		 : std_logic; --FIFO Flush signals for 4 DDCs
	
	signal ffa0_full			 : std_logic;
	signal ffa1_full			 : std_logic;
	signal ffb0_full			 : std_logic;
	signal ffb1_full			 : std_logic;
	signal ffc0_full			 : std_logic;
	signal ffc1_full			 : std_logic;
	signal ffd0_full			 : std_logic;
	signal ffd1_full			 : std_logic;
		
	signal ffa0_empty			 : std_logic;
	signal ffa1_empty			 : std_logic;
	signal ffb0_empty			 : std_logic;
	signal ffb1_empty			 : std_logic;
	signal ffc0_empty			 : std_logic;
	signal ffc1_empty			 : std_logic;
	signal ffd0_empty			 : std_logic;
	signal ffd1_empty			 : std_logic;
	
	signal ffa0_data_out : std_logic_vector(63 downto 0); --Output from ADC capture FIFO
	signal ffa1_data_out : std_logic_vector(63 downto 0);
	signal ffb0_data_out : std_logic_vector(63 downto 0); 
	signal ffb1_data_out : std_logic_vector(63 downto 0);
	signal ffc0_data_out : std_logic_vector(63 downto 0); 
	signal ffc1_data_out : std_logic_vector(63 downto 0);
	signal ffd0_data_out : std_logic_vector(63 downto 0); 
	signal ffd1_data_out : std_logic_vector(63 downto 0);
	
	signal ffa0_data_in : std_logic_vector(63 downto 0);  --From strech module into capture FIFO
	signal ffa1_data_in : std_logic_vector(63 downto 0);
	signal ffb0_data_in : std_logic_vector(63 downto 0);  
	signal ffb1_data_in : std_logic_vector(63 downto 0);
	signal ffc0_data_in : std_logic_vector(63 downto 0);  
	signal ffc1_data_in : std_logic_vector(63 downto 0);
	signal ffd0_data_in : std_logic_vector(63 downto 0);  
	signal ffd1_data_in : std_logic_vector(63 downto 0);
	
	signal ffa0_wr : std_logic;	  --Wr to capture FIFO
	signal ffa1_wr : std_logic;
	signal ffb0_wr : std_logic;	  
	signal ffb1_wr : std_logic;
	signal ffc0_wr : std_logic;	  
	signal ffc1_wr : std_logic;
	signal ffd0_wr : std_logic;	  
	signal ffd1_wr : std_logic;
	
	signal ffa0_rd : std_logic;		--Rd from a capture FIFO
	signal ffa1_rd : std_logic;
	signal ffb0_rd : std_logic;		
	signal ffb1_rd : std_logic;
	signal ffc0_rd : std_logic;		
	signal ffc1_rd : std_logic;
	signal ffd0_rd : std_logic;		
	signal ffd1_rd : std_logic;
	
	signal ffa0_ovrthresh : std_logic; --More 64bit words in a capture FIFO than the DMA block size for that channel
	signal ffa1_ovrthresh : std_logic; 
	signal ffb0_ovrthresh : std_logic; 
	signal ffb1_ovrthresh : std_logic; 
	signal ffc0_ovrthresh : std_logic; 
	signal ffc1_ovrthresh : std_logic; 
	signal ffd0_ovrthresh : std_logic; 
	signal ffd1_ovrthresh : std_logic; 
	
	signal ffa0_rd_cnt : std_logic_vector(9 downto 0); --Number of 64 bit words in each capture FIFO
	signal ffa1_rd_cnt : std_logic_vector(10 downto 0);
	signal ffb0_rd_cnt : std_logic_vector(9 downto 0); 
	signal ffb1_rd_cnt : std_logic_vector(10 downto 0);
	signal ffc0_rd_cnt : std_logic_vector(9 downto 0); 
	signal ffc1_rd_cnt : std_logic_vector(10 downto 0);
	signal ffd0_rd_cnt : std_logic_vector(9 downto 0); 
	signal ffd1_rd_cnt : std_logic_vector(10 downto 0);
	

	-- Decimation
	signal dec_clk  : std_logic;
	signal fifo_clk : std_logic;
	signal dec_reg	 : std_logic_vector(31 downto 0);
	signal dec_rst  : std_logic;
	

	-- DDC & Filter 
	signal cha_i		: std_logic_vector(15 downto 0);		-- Ch A I Data 
	signal cha_q		: std_logic_vector(15 downto 0);		-- Ch A Q Data 
	signal chb_i		: std_logic_vector(15 downto 0);		-- Ch B I Data 
	signal chb_q		: std_logic_vector(15 downto 0);		-- Ch B Q Data 
	signal chc_i		: std_logic_vector(15 downto 0);		-- Ch C I Data 
	signal chc_q		: std_logic_vector(15 downto 0);		-- Ch C Q Data 
	signal chd_i		: std_logic_vector(15 downto 0);		-- Ch D I Data 
	signal chd_q		: std_logic_vector(15 downto 0);		-- Ch D Q Data 

	signal k_addr_reg	: std_logic_vector(31 downto 0);  	--Kaiser Filter Coefficient Address
	signal k_data_reg	: std_logic_vector(31 downto 0);  	--Kaiser Filter Coefficient Data
	signal k_read_reg	: std_logic_vector(31 downto 0);  	--Kaiser Filter Coefficient Read Back
	signal k_write_reg: std_logic;							  	--Kaiser Filter Coefficient Write Strobe
	signal g_addr_reg	: std_logic_vector(31 downto 0);  	--Gaussian Filter Coefficient Address
	signal g_data_reg	: std_logic_vector(31 downto 0);  	--Gaussian Filter Coefficient Data
	signal g_read_reg	: std_logic_vector(31 downto 0);  	--Gaussian Filter Coefficient Read Back
	signal g_write_reg: std_logic;  								--Gaussian Filter Coefficient Write Strobe
	
	-- MultiTimer and Gating
	signal mt_addr_reg : std_logic_vector(31 downto 0);	--MultiTimer Address Register
	signal mt_data_reg : std_logic_vector(31 downto 0);	--MultiTimer Data Register
	signal mt_addr_wr  : std_logic;								--MultiTimer Address Write
	signal mt_data_wr  : std_logic;								--MultiTimer Data Write
	signal pulse_out   : std_logic_vector(3 downto 0);		--MultiTimer Output Pulses
	signal timer_clk   : std_logic;								--MultiTimer Clk (60 MHz)
	signal timer_dcmlocked : std_logic;							--Timer DCM lock status bit.
	signal timer_dcmrst: std_logic;								--Timer DCM reset bit.
	

	-- Pulse Pair Processor
	signal pp_m_reg	: std_logic_vector(31 downto 0); 	--# of Gates for Pulse Pair Processing
	signal pp_n_reg	: std_logic_vector(31 downto 0);		--# of samples for Pulse Pair Processing
	signal iq_index_reg: std_logic_vector(31 downto 0);   --Start Gate of IQ capture
	signal iq_length_reg: std_logic_vector(31 downto 0);  --# of IQ Gates to capture

	signal cha_iq_g	: std_logic_vector(1 downto 0);		--Channel A Gate Signals
	signal cha_pp_g	: std_logic_vector(1 downto 0);
	signal chb_iq_g	: std_logic_vector(1 downto 0);		--Channel B Gate Signals
	signal chb_pp_g	: std_logic_vector(1 downto 0);
	signal chc_iq_g	: std_logic_vector(1 downto 0);		--Channel C Gate Signals
	signal chc_pp_g	: std_logic_vector(1 downto 0);
	signal chd_iq_g	: std_logic_vector(1 downto 0);		--Channel D Gate Signals
	signal chd_pp_g	: std_logic_vector(1 downto 0);
	
	signal cha_iq		: std_logic_vector(31 downto 0);		--Channel A Data
	signal cha_a		: std_logic_vector(31 downto 0);
	signal cha_b		: std_logic_vector(31 downto 0);
	signal cha_p		: std_logic_vector(31 downto 0);
	signal chb_iq		: std_logic_vector(31 downto 0);		--Channel B Data
	signal chb_a		: std_logic_vector(31 downto 0);
	signal chb_b		: std_logic_vector(31 downto 0);
	signal chb_p		: std_logic_vector(31 downto 0);
	signal chc_iq		: std_logic_vector(31 downto 0);		--Channel C Data
	signal chc_a		: std_logic_vector(31 downto 0);
	signal chc_b		: std_logic_vector(31 downto 0);
	signal chc_p		: std_logic_vector(31 downto 0);
	signal chd_iq		: std_logic_vector(31 downto 0);		--Channel D Data
	signal chd_a		: std_logic_vector(31 downto 0);
	signal chd_b		: std_logic_vector(31 downto 0);
	signal chd_p		: std_logic_vector(31 downto 0);
	
	signal a_sync_error  : std_logic;							--Timing Sync Error Signal
	signal b_sync_error  : std_logic;							--Timing Sync Error Signal
	signal c_sync_error  : std_logic;							--Timing Sync Error Signal
	signal d_sync_error  : std_logic;							--Timing Sync Error Signal
	signal pp_rst		: std_logic;								--Pulse Pair Reset
	
	
	-- Gate Splitter
	signal gs_dprt_reg: std_logic;							-- Dual PRT Register
	signal cha_gate_in: std_logic_vector(1 downto 0);	-- Gate 1
	signal chb_gate_in: std_logic_vector(1 downto 0);	-- Gate 1
	signal chc_gate_in: std_logic_vector(1 downto 0);	-- Gate 1
	signal chd_gate_in: std_logic_vector(1 downto 0);	-- Gate 1
	
	
	-- GPIO Buffered Signals
	signal gpio_buf :	  std_logic_vector(3 downto 0);
	
	
	-- Timing Select Signal
	signal timing_sel : std_logic;
	
	-- SVN Revision Register
	signal svn_rev_reg :std_logic_vector(31 downto 0);
	
	
--------------------------------------
-- Component Instatantiations
--------------------------------------

	-- ADC Sample and Filter Clk DCM
	COMPONENT adc_dcm_m314 
	PORT (
			rst		  	: in std_logic;
			sampleclk_p	: in std_logic;
			sampleclk_n	: in std_logic;
			adc_clk		: out std_logic;
			filter_clk	: out std_logic;
			locked		: out std_logic);
	END COMPONENT;
	
	
	-- Timer Clk DCM
	COMPONENT timer_dcm_m314
	PORT (
			rst			: in std_logic;
			adc_clk		: in std_logic;
			timer_clk	: out std_logic;
			locked		: out std_logic);
	END COMPONENT;
	
	
	-- Register that Holds the SVN Revision #
   COMPONENT Revision_Reg
   PORT ( 
			Revision_Number : out  STD_LOGIC_VECTOR (31 downto 0));
	END COMPONENT;


--	-- Multiple Timers to produce the gating signals.
	COMPONENT MultiTimer 
   Generic (NTIMERS : integer := 4);
   PORT ( 
			BUSCLK       : in  std_logic;
         RST          : in  std_logic;
         ADDRWR       : in  std_logic;
         ADDRIN       : in  std_logic_vector(31 downto 0);
         DATAWR       : in  std_logic;
			TIMERCLK     : in  std_logic;
         ONEPPS       : in  std_logic;         -- not needed for now??
         DATAIN       : in  std_logic_vector(31 downto 0);
         DATAOUT      : inout std_logic_vector(31 downto 0);
         PULSEOUT     : out std_logic_vector(NTIMERS-1 downto 0);
         ADDROUT      : out std_logic_vector(31 downto 0));
	END COMPONENT;
	
	
	-- Gate Splitter for Multiple PRT
	COMPONENT gate_splitter_cw  
	PORT (
			a_gate_in_ext: in std_logic;
			a_gate_in_int: in std_logic;
			b_gate_in_ext: in std_logic;
			b_gate_in_int: in std_logic;
			c_gate_in_ext: in std_logic;
			c_gate_in_int: in std_logic;
			ce: in std_logic := '1';
			clk: in std_logic;
			d_gate_in_ext: in std_logic;
			d_gate_in_int: in std_logic;
			dual_prt: in std_logic;
			reset: in std_logic;
			timing_sel: in std_logic;
			a_gate1_out: out std_logic;
			a_gate2_out: out std_logic;
			b_gate1_out: out std_logic;
			b_gate2_out: out std_logic;
			c_gate1_out: out std_logic;
			c_gate2_out: out std_logic;
			d_gate1_out: out std_logic;
			d_gate2_out: out std_logic);
	END COMPONENT;

	
	-- IQ Data Capture
	COMPONENT iq_capture 
	PORT (
			clk           : in std_logic;	
			reset         : in std_logic;	
			enable        : in std_logic;	
			sync	        : in std_logic;	
			iq_data_in    : in std_logic_vector(31 downto 0); 
			gate_1  		  : in std_logic; 
			gate_2  		  : in std_logic; 
			channel_id	  : in std_logic_vector(3 downto 0); 
			prt_mode		  : in std_logic; 
			data_valid64  : out std_logic; 
			iq_data_out	  : out std_logic_vector(63 downto 0)); 
	END COMPONENT;	

	
	-- PP Data Capture
	COMPONENT pp_capture 
	PORT (
			clk           : in std_logic;	
			reset         : in std_logic;	
			enable        : in std_logic;	
			sync	        : in std_logic;	
			a_data_in     : in std_logic_vector(31 downto 0); 
			b_data_in     : in std_logic_vector(31 downto 0); 
			p_data_in     : in std_logic_vector(31 downto 0); 
			gate_1  		  : in std_logic; 
			gate_2  		  : in std_logic; 
			channel_id	  : in std_logic_vector(3 downto 0); 
			prt_mode		  : in std_logic; 
			sync_error    : in std_logic;
			data_valid64  : out std_logic; 
			pp_data_out	  : out std_logic_vector(63 downto 0)); 
	END COMPONENT;	

	
	-- ADC Capture FIFOs, 64x1k 
	COMPONENT fifo_64x1k_sa
	PORT (
			din				: in std_logic_vector(63 downto 0);
			rd_clk			: in std_logic;
			rd_en				: in std_logic;
			rst				: in std_logic;
			wr_clk			: in std_logic;
			wr_en				: in std_logic;
			dout				: out std_logic_VECTOR(63 downto 0);
			empty				: out std_logic;
			full				: out std_logic;
			rd_data_count	: out std_logic_VECTOR(9 downto 0));
	END COMPONENT;
	
	
	-- ADC Capture FIFOs, 64x2k 
	COMPONENT fifo_64x2k_sa
	PORT (
			din				: in std_logic_vector(63 downto 0);
			rd_clk			: in std_logic;
			rd_en				: in std_logic;
			rst				: in std_logic;
			wr_clk			: in std_logic;
			wr_en				: in std_logic;
			dout				: out std_logic_VECTOR(63 downto 0);
			empty				: out std_logic;
			full				: out std_logic;
			rd_data_count	: out std_logic_VECTOR(10 downto 0));
	END COMPONENT;
	
	
	-- Decimator
	COMPONENT decimator
	PORT ( 
			clk_in 		: in  std_logic;
			reset 		: in  std_logic;
			decimation 	: in  std_logic_vector(7 downto 0);
			dec_clk     : out std_logic;
			fifo_clk    : out std_logic);
	END COMPONENT;
	 
	
	-- DDC 
	COMPONENT casc_filter_cw  
	PORT (
			ce: in std_logic := '1';
			cha_in: in std_logic_vector(13 downto 0);
			chb_in: in std_logic_vector(13 downto 0);
			chc_in: in std_logic_vector(13 downto 0);
			chd_in: in std_logic_vector(13 downto 0);
			clk: in std_logic;
			g_addr: in std_logic_vector(3 downto 0);
			g_data: in std_logic_vector(17 downto 0);
			g_sel: in std_logic_vector(1 downto 0);
			g_wr: in std_logic;
			k_addr: in std_logic_vector(3 downto 0);
			k_data: in std_logic_vector(17 downto 0);
			k_sel: in std_logic_vector(1 downto 0);
			k_wr: in std_logic;
			stop: in std_logic;
			cha_i_out: out std_logic_vector(15 downto 0);
			cha_q_out: out std_logic_vector(15 downto 0);
			chb_i_out: out std_logic_vector(15 downto 0);
			chb_q_out: out std_logic_vector(15 downto 0);
			chc_i_out: out std_logic_vector(15 downto 0);
			chc_q_out: out std_logic_vector(15 downto 0);
			chd_i_out: out std_logic_vector(15 downto 0);
			chd_q_out: out std_logic_vector(15 downto 0);
			g_readcoef: out std_logic_vector(17 downto 0);
			k_readcoef: out std_logic_vector(17 downto 0));
	END COMPONENT;
	
	
	-- Pulse Pair Processors 
	COMPONENT pulsepairproc_cw PORT (
			a_gate1: in std_logic;
			a_gate2: in std_logic;
			a_iin: in std_logic_vector(15 downto 0);
			a_qin: in std_logic_vector(15 downto 0);
			b_gate1: in std_logic;
			b_gate2: in std_logic;
			b_iin: in std_logic_vector(15 downto 0);
			b_qin: in std_logic_vector(15 downto 0);
			c_gate1: in std_logic;
			c_gate2: in std_logic;
			c_iin: in std_logic_vector(15 downto 0);
			c_qin: in std_logic_vector(15 downto 0);
			ce: in std_logic := '1';
			clk: in std_logic;
			d_gate1: in std_logic;
			d_gate2: in std_logic;
			d_iin: in std_logic_vector(15 downto 0);
			d_qin: in std_logic_vector(15 downto 0);
			iq_index: in std_logic_vector(9 downto 0);
			iq_length: in std_logic_vector(9 downto 0);
			m: in std_logic_vector(9 downto 0);
			n: in std_logic_vector(6 downto 0);
			reset: in std_logic;
			a_adata: out std_logic_vector(31 downto 0);
			a_bdata: out std_logic_vector(31 downto 0);
			a_iqdata: out std_logic_vector(31 downto 0);
			a_iqgate1: out std_logic;
			a_iqgate2: out std_logic;
			a_pdata: out std_logic_vector(31 downto 0);
			a_ppgate1: out std_logic;
			a_ppgate2: out std_logic;
			a_sync_error: out std_logic;
			b_adata: out std_logic_vector(31 downto 0);
			b_bdata: out std_logic_vector(31 downto 0);
			b_iqdata: out std_logic_vector(31 downto 0);
			b_iqgate1: out std_logic;
			b_iqgate2: out std_logic;
			b_pdata: out std_logic_vector(31 downto 0);
			b_ppgate1: out std_logic;
			b_ppgate2: out std_logic;
			b_sync_error: out std_logic;
			c_adata: out std_logic_vector(31 downto 0);
			c_bdata: out std_logic_vector(31 downto 0);
			c_iqdata: out std_logic_vector(31 downto 0);
			c_iqgate1: out std_logic;
			c_iqgate2: out std_logic;
			c_pdata: out std_logic_vector(31 downto 0);
			c_ppgate1: out std_logic;
			c_ppgate2: out std_logic;
			c_sync_error: out std_logic;
			d_adata: out std_logic_vector(31 downto 0);
			d_bdata: out std_logic_vector(31 downto 0);
			d_iqdata: out std_logic_vector(31 downto 0);
			d_iqgate1: out std_logic;
			d_iqgate2: out std_logic;
			d_pdata: out std_logic_vector(31 downto 0);
			d_ppgate1: out std_logic;
			d_ppgate2: out std_logic;
			d_sync_error: out std_logic);
	END COMPONENT;

	
	-- Local Bus Interface for DMA
   COMPONENT Local_Bus_Interface_Wrapper 
	PORT (
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
			LBAD :        	        inout std_logic_vector (24 downto 0);
			D :                    inout std_logic_vector (63 downto 0);
			Int_to_FPGA :          in    std_logic;
			Int_to_Bridge :        out   std_logic;
	        
			-- User interface
	      PCIclk, Rst :      	  out std_logic;
	      PCIclk_2x	:	  	  	  out std_logic;
	      Srst :             	  in std_logic;
	      Base_Addr :        	  in std_logic_vector (31 downto 0);
	      Local_Addr :       	  in std_logic_vector (31 downto 0);
	      DMA_SG_En :        	  in std_logic;
	      DMA_Channel :      	  in std_logic_vector(4 downto 0);
	      DMA_Go :           	  in std_logic;
	      DMA_Busy :         	  out std_logic;
	      DMA_Direction :    	  in std_logic;
	      Local_Data_ADS_N : 	  out std_logic;
	      Local_Data_Rdy_N : 	  in std_logic;
	      Local_Data_Rd_N :  	  out std_logic;
	      Local_Data_Wr_N :  	  out std_logic;
	      Local_Data_Addr :  	  out std_logic_vector (31 downto 0);
	      Local_Data_In :    	  out std_logic_vector (63 downto 0);
	      Local_Data_Out :   	  inout std_logic_vector (63 downto 0);
	      PCI_int :          	  in std_logic;
	      Last_Brst_Wd_N :   	  out std_logic;
		   DMA_Ch0_BlkSize :      out   std_logic_vector (15 downto 0);
		   DMA_Ch1_BlkSize :      out   std_logic_vector (15 downto 0);
		   DMA_Ch2_BlkSize :      out   std_logic_vector (15 downto 0);
		   DMA_Ch3_BlkSize :      out   std_logic_vector (15 downto 0);
			DMA_Ch4_BlkSize :      out   std_logic_vector (15 downto 0);
		   DMA_Ch5_BlkSize :      out   std_logic_vector (15 downto 0);
		   DMA_Ch6_BlkSize :      out   std_logic_vector (15 downto 0);
		   DMA_Ch7_BlkSize :      out   std_logic_vector (15 downto 0);
		   DMA_Last_Blk	  :     out std_logic_vector(31 downto 0);
		   DMA_Interrupt_Status : out std_logic);
	END COMPONENT;
    
begin                                             
	 DMABaseAddr  <= X"00000000";  --0x0, start of DMA Core
	 UserBaseAddr <= X"00000800";  --0x800, start of user logic
	
	-- ==== SVN Revision Register ===================================================
	SVN_Revision_Reg: Revision_Reg PORT MAP(
	    Revision_Number => svn_rev_reg);
	
	-- ==== GPIO =====================================================================
	IBUF_3 : IBUF PORT MAP (
      O => gpio_buf(3), -- Buffer output
      I => gpio(3));    -- Buffer input (connect directly to top-level port)
	
	IBUF_2 : IBUF PORT MAP (
      O => gpio_buf(2), -- Buffer output
      I => gpio(2));    -- Buffer input (connect directly to top-level port)
		
	IBUF_1 : IBUF PORT MAP (
      O => gpio_buf(1), -- Buffer output
      I => gpio(1));    -- Buffer input (connect directly to top-level port)
		
	IBUF_0 : IBUF PORT MAP (
      O => gpio_buf(0), -- Buffer output
      I => gpio(0));    -- Buffer input (connect directly to top-level port)
	
--	-- ==== GPIO =====================================================================
--	gpio(3) <= cha_gate_in(1);
--	gpio(2) <= cha_gate_in(0);
--	gpio(1) <= cha_gate_out(1);
--	gpio(0) <= cha_gate_out(0);
	
	-- ===== Analog Sample Clk DCM ====================================================
	--This DCM is used for the incomming LVDS ADC sample clock.  
	--A Multiplied (3x) version is used for the filter clk.
	adc_dcmrst <= ctl_reg(0) or ctl_reg(15);  --reset with ADC DCM Reset or soft reset
	ADC_clkmanager: adc_dcm_m314 PORT MAP(
			rst		  	=> adc_dcmrst,
			sampleclk_p	=> sampleclk_p,
			sampleclk_n	=> sampleclk_n,
			adc_clk		=> adc_clk,
			filter_clk	=> filter_clk,
			locked		=> adc_dcmlocked);
	 
	-- ===== Decimation Clk DCM ====================================================
	--This DCM is used for decimated sample clock.  
	--A Multiplied (3x) version is used for the FIFO storage clock.
   
	-- Decimator Instantiation
	ADC_Clk_Decimator : decimator PORT MAP (
		clk_in      => adc_clk,
		reset       => dec_rst,
      decimation  => dec_reg(7 downto 0),
      dec_clk     => dec_clk,
		fifo_clk    => fifo_clk);

	-- ===== Timer Clk DCM ====================================================
	--This DCM is used for the timer sample clock.  
	--Produces a 60 MHz clock for timers from 48 MHz adc clock.
	timer_dcmrst <= ctl_reg(1) or ctl_reg(15);  --reset with Timer DCM Reset or soft reset
	Timer_clkmanager: timer_dcm_m314 PORT MAP(
			rst		  	=> timer_dcmrst,
			adc_clk		=> adc_clk,
			timer_clk	=> timer_clk,
			locked		=> timer_dcmlocked);

    -- ===== Address Decoder =========================================================
    --This logic will detect that a Rd/Wr is being done but looking at the Local_Data_ADS_N
	--When and addr match is found the select line for that location will be driven low.
	 
	 -- check and see if we are the address target.  This will speed up decode later on
	 user_addr_hit <= '1' when (Local_Data_Addr(31 downto 12) = UserBaseAddr(31 downto 12)) else '0';
	 	
    
    -- The DMA core wants an active low signal to ACK a wr or indicate data valid in response
	 -- to a rd.
    Local_Data_Rdy_N <= not Local_Data_Rdy;   
	 
	 
    -- Local addr decoder
	 -- define the select bits based on the input address and base address.  Will look at 
	 --bits 24:2 of the local addr from the DMA core
	 --All select lines are active low
    selects : process (user_clk, reset)
    begin
        if (reset = '1') then
         rev_seln      <= '1';	
			ctl_seln      <= '1';
			stat_seln	  <= '1';
			mask_seln	  <= '1';
			ch0_seln      <= '1';
			ch1_seln      <= '1';
			ch2_seln      <= '1';
			ch3_seln      <= '1';	
			ch4_seln      <= '1';
			ch5_seln      <= '1';
			ch6_seln      <= '1';
			ch7_seln      <= '1';	
			k_addr_seln		<= '1';
			k_data_seln		<= '1';
			k_read_seln		<= '1';
			k_write_seln	<= '1';
			g_addr_seln		<= '1';
			g_data_seln		<= '1';
			g_read_seln		<= '1';
			g_write_seln	<= '1';
			dec_seln			<= '1';
			dec_rst_seln	<= '1';
			mt_addr_seln	<= '1';
			mt_data_seln	<= '1';
			mt_wr_seln		<= '1';
			pp_m_seln		<= '1';
			pp_n_seln		<= '1';
			pp_rst_seln		<= '1';
			iq_index_seln	<= '1';
			iq_length_seln	<= '1';
			gs_dprt_seln	<= '1';
			timing_seln    <= '1';
			svn_rev_seln   <= '1';
			addr_decode_en <= '0';
			Local_Data_Rdy  <= '0';
        elsif rising_edge (user_clk) 
		  then
            -- We got an access to user space, set a flag to decode the addr.  If this is a target rd/wr
				-- Local_Data_ADS_N will be active for one clk. If this is a target access leave the 
				-- decode flag low until we see the last word of the burst happen
            if (Local_Data_ADS_N = '0' and user_addr_hit = '1') then
                addr_decode_en <= '1';
            elsif (Last_Brst_Wd_N = '0') then
                addr_decode_en <= '0';
			else
				addr_decode_en <=  addr_decode_en;
            end if;

				
			--The negated version of this is the DV/ACK signal to the DMA core.  If it is 0 it will get
			--the addr_decode_en signal to signal align with rd data going back to the core or 
			--or to ACK the target write.  It will hold it's value as long as a burst transfer is going on
			if (Local_Data_Rdy = '0' or Last_Brst_Wd_N = '1') then
                Local_Data_Rdy <= addr_decode_en;
            else
                Local_Data_Rdy <= '0';
            end if;
                
				            
            -- drive a select line low based on the input address
            if (addr_decode_en = '1') 
				then
					--User revison register
					--PCI Adr = 0x800
	                if (Local_Data_Addr(11 downto 2) = "1000000000") then 
	                    rev_seln <= '0';
	                else
	                    rev_seln <= '1';
	                end if;

					--Control Register
					--PCI Adr = 0x80C
	                if (Local_Data_Addr(11 downto 2) = "1000000011") then 
	                    ctl_seln <= '0';
	                else
	                    ctl_seln <= '1';
	                end if;
	              
					--Status Register
					--PCI Adr = 0x810
						 if (Local_Data_Addr(11 downto 2) = "1000000100") then
							  stat_seln <= '0';
						 else
							  stat_seln <= '1';
						 end if;
					  
	            --Mask Register
					--PCI Adr = 0x814
						 if (Local_Data_Addr(11 downto 2) = "1000000101") then
							  mask_seln <= '0';
						 else
							  mask_seln <= '1';
						 end if;
					 
					--Ch 0 Data
					--PCI Adr = 0x900								 
	                if (Local_Data_Addr(11 downto 2) = "1001000000") then 
	                    ch0_seln <= '0';
	                else
	                    ch0_seln <= '1';
	                end if;
	                
	            --Ch 1 Data
					--PCI Adr = 0x908	                              						 
	                if (Local_Data_Addr(11 downto 2) = "1001000010") then 
	                    ch1_seln <= '0';
	                else
	                    ch1_seln <= '1';
	                end if;
	                
	            --Ch 2 Data
					--PCI Adr = 0x910	                                    						 
	                if (Local_Data_Addr(11 downto 2) = "1001000100") then 
	                    ch2_seln <= '0';
	                else
	                    ch2_seln <= '1';
	                end if;
	                
	            --Ch 3 Data
					--PCI Adr = 0x918	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1001000110") then 
	                    ch3_seln <= '0';
	                else
	                    ch3_seln <= '1';
	                end if;
					
					--Ch 4 Data
					--PCI Adr = 0x920								 
	                if (Local_Data_Addr(11 downto 2) = "1001001000") then 
	                    ch4_seln <= '0';
	                else
	                    ch4_seln <= '1';
	                end if;
	                
	            --Ch 5 Data
					--PCI Adr = 0x928	                              						 
	                if (Local_Data_Addr(11 downto 2) = "1001001010") then 
	                    ch5_seln <= '0';
	                else
	                    ch5_seln <= '1';
	                end if;
	                
	            --Ch 6 Data
					--PCI Adr = 0x930	                                    						 
	                if (Local_Data_Addr(11 downto 2) = "1001001100") then 
	                    ch6_seln <= '0';
	                else
	                    ch6_seln <= '1';
	                end if;
	                
	            --Ch 7 Data
					--PCI Adr = 0x938	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1001001110") then 
	                    ch7_seln <= '0';
	                else
	                    ch7_seln <= '1';
	                end if;
					
					--Kaiser Coefficient Address
					--PCI Adr = 0xA00	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010000000") then 
	                    k_addr_seln <= '0';
	                else
	                    k_addr_seln <= '1';
	                end if;
					
					--Kaiser Coefficient Data
					--PCI Adr = 0xA04	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010000001") then 
	                    k_data_seln <= '0';
	                else
	                    k_data_seln <= '1';
	                end if;
						 
					--Kaiser Coefficient Read Back
					--PCI Adr = 0xA08	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010000010") then 
	                    k_read_seln <= '0';
	                else
	                    k_read_seln <= '1';
	                end if;
						 
					--Kaiser Coefficient Write
					--PCI Adr = 0xA0C	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010000011") then 
	                    k_write_seln <= '0';
	                else
	                    k_write_seln <= '1';
	                end if;
						 
					--Gaussian Coefficient Address
					--PCI Adr = 0xA10	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010000100") then 
	                    g_addr_seln <= '0';
	                else
	                    g_addr_seln <= '1';
	                end if;	 
					
					--Gaussian Coefficient Data
					--PCI Adr = 0xA14	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010000101") then 
	                    g_data_seln <= '0';
	                else
	                    g_data_seln <= '1';
	                end if;	 
						 
					--Gaussian Coefficient Read Back
					--PCI Adr = 0xA18	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010000110") then 
	                    g_read_seln <= '0';
	                else
	                    g_read_seln <= '1';
	                end if;	 
						 
					--Gaussian Coefficient Write
					--PCI Adr = 0xA1C	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010000111") then 
	                    g_write_seln <= '0';
	                else
	                    g_write_seln <= '1';
	                end if;	
						 
					--Clk Decimation 
					--PCI Adr = 0xA20	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010001000") then 
	                    dec_seln <= '0';
	                else
	                    dec_seln <= '1';
	                end if;	
					
					--Clk Decimation Reset
					--PCI Adr = 0xA24	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010001001") then 
	                    dec_rst_seln <= '0';
	                else
	                    dec_rst_seln <= '1';
	                end if;	
					
					--MultiTimer Addr
					--PCI Adr = 0xA30	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010001100") then 
	                    mt_addr_seln <= '0';
	                else
	                    mt_addr_seln <= '1';
	                end if;	
						 
					--MultiTimer Data
					--PCI Adr = 0xA34	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010001101") then 
	                    mt_data_seln <= '0';
	                else
	                    mt_data_seln <= '1';
	                end if;	
					
					--MultiTimer Wr
					--PCI Adr = 0xA38	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010001110") then 
	                    mt_wr_seln <= '0';
	                else
	                    mt_wr_seln <= '1';
	                end if;	
				 
					--Pulse Pair M Data
					--PCI Adr = 0xA40	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010010000") then 
	                    pp_m_seln <= '0';
	                else
	                    pp_m_seln <= '1';
	                end if;	
						 
					--Pulse Pair N Data
					--PCI Adr = 0xA44	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010010001") then 
	                    pp_n_seln <= '0';
	                else
	                    pp_n_seln <= '1';
	                end if;	
					
					--IQ index Data
					--PCI Adr = 0xA48	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010010010") then 
	                    iq_index_seln <= '0';
	                else
	                    iq_index_seln <= '1';
	                end if;	
						 
					--IQ length Data
					--PCI Adr = 0xA4C	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010010011") then 
	                    iq_length_seln <= '0';
	                else
	                    iq_length_seln <= '1';
	                end if;	
									
					--Dual PRT
					--PCI Adr = 0xA50	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010010100") then 
	                    gs_dprt_seln <= '0';
	                else
	                    gs_dprt_seln <= '1';
	                end if;	
						 
					--Pulse Pair Reset
					--PCI Adr = 0xA54	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010010101") then 
	                    pp_rst_seln <= '0';
	                else
	                    pp_rst_seln <= '1';
	                end if;	
					
					--Timing Select
					--PCI Adr = 0xA58	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010010110") then 
	                    timing_seln <= '0';
	                else
	                    timing_seln <= '1';
	                end if;	
						 
					--SVN Revision Register
					--PCI Adr = 0xA58	                                 						 
	                if (Local_Data_Addr(11 downto 2) = "1010010111") then 
	                    svn_rev_seln <= '0';
	                else
	                    svn_rev_seln <= '1';
	                end if;	
						 
						 
            else -- clear all of the selects if data rdy isn't active
	            rev_seln      <= '1';	
					ctl_seln      <= '1';
					stat_seln	  <= '1';
					mask_seln	  <= '1';
					ch0_seln      <= '1';
					ch1_seln      <= '1';
					ch2_seln      <= '1';
					ch3_seln      <= '1';
					ch4_seln      <= '1';
					ch5_seln      <= '1';
					ch6_seln      <= '1';
					ch7_seln      <= '1';
					k_addr_seln		<= '1';
					k_data_seln		<= '1';
					k_read_seln		<= '1';
					k_write_seln	<= '1';
					g_addr_seln		<= '1';
					g_data_seln		<= '1';
					g_read_seln		<= '1';
					g_write_seln	<= '1';
					dec_seln			<= '1';
					dec_rst_seln   <= '1';
					mt_addr_seln	<= '1';
					mt_data_seln	<= '1';
					mt_wr_seln		<= '1';
					pp_m_seln		<= '1';
					pp_n_seln		<= '1';
					pp_rst_seln    <= '1';
					iq_index_seln	<= '1';
					iq_length_seln	<= '1';
					gs_dprt_seln	<= '1';
					timing_seln    <= '1';
					svn_rev_seln   <= '1';
					
            end if;
        end if;
    end process; -- selects
    
	 
	 --This process detects a user space write via the select lines and wrl, then writes the 
	 --data on the Local_Data_In bus to the appropriate register
    input_data : process (reset, user_clk)
    begin
    	if (reset = '1') 
		then 		
			ctl_reg      <= (others => '0');
			mask_reg		 <= (others => '0');
			k_addr_reg	 <= (others => '0');
			k_data_reg	 <= (others => '0');
			k_write_reg	 <= '0';
			g_addr_reg	 <= (others => '0');
			g_data_reg	 <= (others => '0');
			g_write_reg	 <= '0';
			dec_reg		 <= (others => '0');
			dec_rst		 <= '0';
			mt_addr_reg  <= (others => '0');
			mt_data_reg  <= (others => '0');
			mt_addr_wr   <= '0';
			mt_data_wr	 <= '0';
			pp_m_reg		 <= (others => '0');
			pp_n_reg		 <= (others => '0');
			pp_rst       <= '0';
			iq_index_reg <= (others => '0');
			iq_length_reg<= (others => '0');
			gs_dprt_reg	 <= '0';
			timing_sel   <= '0';
    	elsif rising_edge (user_clk) 
		then
			--Control
    		if (wrl = '0' and ctl_seln = '0') then
				ctl_reg(28 downto 23) <= Local_Data_In(28 downto 23);
				ctl_reg(22 downto 21) <= (others => '0');
				ctl_reg(20 downto 5)  <= Local_Data_In(20 downto 5);
				ctl_reg(4 downto 2)   <= (others => '0');
				ctl_reg(1 downto 0)   <= Local_Data_In(1 downto 0);
			else
				ctl_reg(28 downto 23) <= ctl_reg(28 downto 23);
				ctl_reg(20 downto 16) <= ctl_reg(20 downto 16);
				ctl_reg(15)           <= '0';									-- Soft Reset
				ctl_reg(14 downto 10) <= ctl_reg(14 downto 10);			-- Clear FF Flushes
				ctl_reg(9 downto 5)   <= (others => '0');					-- Clear Snapshot Enables
				ctl_reg(4 downto 2)   <= (others => '0');					-- Reserved
				ctl_reg(1 downto 0)   <= (others => '0');					-- DCM Reset
    		end if;
			
			--Mask
			if (wrl = '0' and mask_seln = '0') then
				mask_reg(31 downto 30) <= Local_Data_In(31 downto 30);
				mask_reg(29 downto 20) <= (others => '0');				-- Reserved
				mask_reg(19 downto 0)  <= Local_Data_In(19 downto 0);
			else
				mask_reg <= mask_reg;
			end if;
			
			--Kaiser Coefficient Address 
			if (wrl = '0' and k_addr_seln = '0') then
				k_addr_reg <= Local_Data_In(31 downto 0);
			else
				k_addr_reg <= k_addr_reg;
			end if;
			
			--Kaiser Coefficient Data
			if (wrl = '0' and k_data_seln = '0') then
				k_data_reg <= Local_Data_In(31 downto 0);
			else
				k_data_reg <= k_data_reg;
			end if;
			
			--Kaiser Coefficient Write
			if (wrl = '0' and k_write_seln = '0') then
				k_write_reg <= Local_Data_In(0);
			else
				k_write_reg <= k_write_reg;
			end if;			
			
			--Gaussian Coefficient Address 
			if (wrl = '0' and g_addr_seln = '0') then
				g_addr_reg <= Local_Data_In(31 downto 0);
			else
				g_addr_reg <= g_addr_reg;
			end if;
			
			--Gaussian Coefficient Data
			if (wrl = '0' and g_data_seln = '0') then
				g_data_reg <= Local_Data_In(31 downto 0);
			else
				g_data_reg <= g_data_reg;
			end if;
			
			--Gaussian Coefficient Write
			if (wrl = '0' and g_write_seln = '0') then
				g_write_reg <= Local_Data_In(0);
			else
				g_write_reg <= g_write_reg;
			end if;
			
			--Decimation Register
			if (wrl = '0' and dec_seln = '0') then
				dec_reg <= Local_Data_In(31 downto 0);
			else
				dec_reg <= dec_reg;
			end if;
			
			--Decimation Reset
			if (wrl = '0' and dec_rst_seln = '0') then
				dec_rst <= Local_Data_In(0);
			else
				dec_rst <= dec_rst;
			end if;
			
			--MultTimer Addr Register
			if (wrl = '0' and mt_addr_seln = '0') then
				mt_addr_reg <= Local_Data_In(31 downto 0);
			else
				mt_addr_reg <= mt_addr_reg;
			end if;
			
			--MultiTimer Data Register
			if (wrl = '0' and mt_data_seln = '0') then
				mt_data_reg <= Local_Data_In(31 downto 0);
			else
				mt_data_reg <= mt_data_reg;
			end if;
			
			--MultiTimer Wr Register
			if (wrl = '0' and mt_wr_seln = '0') then
				mt_addr_wr <= Local_Data_In(0);
				mt_data_wr <= Local_Data_In(1);
			else
				mt_addr_wr <= mt_addr_wr;
				mt_data_wr <= mt_data_wr;
			end if;
			
			--Pulse Pair M Register
			if (wrl = '0' and pp_m_seln = '0') then
				pp_m_reg <= Local_Data_In(31 downto 0);
			else
				pp_m_reg <= pp_m_reg;
			end if;
			
			--Pulse Pair N Register
			if (wrl = '0' and pp_n_seln = '0') then
				pp_n_reg <= Local_Data_In(31 downto 0);
			else
				pp_n_reg <= pp_n_reg;
			end if;
			
			--IQ Index Register
			if (wrl = '0' and iq_index_seln = '0') then
				iq_index_reg <= Local_Data_In(31 downto 0);
			else
				iq_index_reg <= iq_index_reg;
			end if;
			
			--IQ Length Register
			if (wrl = '0' and iq_length_seln = '0') then
				iq_length_reg <= Local_Data_In(31 downto 0);
			else
				iq_length_reg <= iq_length_reg;
			end if;
			
			--Gate Splitter Register
			if (wrl = '0' and gs_dprt_seln = '0') then
				gs_dprt_reg <= Local_Data_In(0);
			else
				gs_dprt_reg <= gs_dprt_reg;
			end if;
			
			--Pulse Pair Reset Register
			if (wrl = '0' and pp_rst_seln = '0') then
				pp_rst <= Local_Data_In(0);
			else
				pp_rst <= pp_rst;
			end if;
			
			--Timing Select Register
			if (wrl = '0' and timing_seln = '0') then
				timing_sel <= Local_Data_In(0);
			else
				timing_sel <= timing_sel;
			end if;
			
    	end if;
    end process; -- input_data
    
   
	 --Create a status register.  All signals are "sticky" untill cleared by a read
	 --of the status register.  The execption is the intr pending signal from the DMA
	 --core is is already "sticky" (cleared by a read of it's status register.)
	 --This register is async to any other clk domain in the design.
    status : process(reset, stat_seln, rdl,
					--sram_dcmlocked, 
					adc_dcmlocked, 
					adca_or, adcb_or, adcc_or, adcd_or,
					ffa0_full, ffa1_full, ffb0_full, ffb1_full, 
					ffc0_full, ffc1_full, ffd0_full, ffd1_full, 
					ffa0_wr, ffa1_wr, ffb0_wr, ffb1_wr, 
					ffc0_wr, ffc1_wr, ffd0_wr, ffd1_wr,
					a_sync_error, b_sync_error, c_sync_error, d_sync_error,
					DMA_Interrupt_Status)
	 begin
		if(reset = '1') then --init on reset 
			status_reg <= (others => '0');
		elsif (stat_seln = '0' and rdl ='0')then
			status_reg <= (others => '0');  --clear on read of status reg
		else
			status_reg(31 downto 20) <= (others => '0');
			status_reg(30)   <= DMA_Interrupt_Status;  --DMA core already has it sticky
			--status_reg(29 downto 20) <= (others => '0');
			status_reg(29 downto 24) <= (others => '0');
			status_reg(23)   <= a_sync_error or status_reg(23);  --Sync Errors
			status_reg(22)   <= b_sync_error or status_reg(22);
			status_reg(21)   <= c_sync_error or status_reg(21);
			status_reg(20)   <= d_sync_error or status_reg(20);
			status_reg(19)   <= '0'; 						 --Wr after FF Full
 			--status_reg(18)   <= a_sync_error or status_reg(23);  --Sync Errors
			--status_reg(17)   <= b_sync_error or status_reg(22);
			--status_reg(16)   <= c_sync_error or status_reg(21);
			--status_reg(15)   <= d_sync_error or status_reg(20);
			status_reg(18)   <= ((ffa0_full and ffa0_wr) or (ffa1_full and ffa1_wr)) or status_reg(18);
			status_reg(17)   <= ((ffb0_full and ffb0_wr) or (ffb1_full and ffb1_wr)) or status_reg(17);
			status_reg(16)   <= ((ffc0_full and ffc0_wr) or (ffc1_full and ffc1_wr)) or status_reg(16);
			status_reg(15)   <= ((ffd0_full and ffd0_wr) or (ffd1_full and ffd1_wr)) or status_reg(15);
			status_reg(14)   <= '0'; 						--Fifos full
			status_reg(13)   <= ffa0_full or ffa1_full or status_reg(13);
			status_reg(12)   <= ffb0_full or ffb1_full or status_reg(12);
			status_reg(11)   <= ffc0_full or ffc1_full or status_reg(11);
			status_reg(10)   <= ffd0_full or ffd1_full or status_reg(10);
			status_reg(9 downto 6) <= (others => '0');  --ADC cur in over
			status_reg(5)    <= adca_or or status_reg(5); --ADC has over ranged
			status_reg(4)    <= adcb_or or status_reg(4);
			status_reg(3)    <= adcc_or or status_reg(3);
			status_reg(2)    <= adcd_or or status_reg(2);
			status_reg(1)    <= not timer_dcmlocked or status_reg(1); --Timer DCM out of lock
			status_reg(0)    <= not adc_dcmlocked  or status_reg(0); --ADC DCM out of lock
		end if;
	end process;
	
	
	--Create the local interupt signal to the PCI core based on status and mask
	--This version will be double registered to cross it into the haclk domain.
	--This version of the status register will also set bit 31 if, based on the mask,
	--a PCI intr should be generated.
	local_intr_status : process(reset, user_clk)
	begin
		if(reset = '1') then
			status_regR   <= (others => '0');
			status_regRR  <= (others => '0');
			status_regRRR <= (others => '0');
		elsif rising_edge (user_clk) 
		then
			
			--Register status_reg into the haclk domain
			status_regR  <= status_reg;
			status_regRR <= status_regR;
			
			--If bit 31 is set and the AND of any of the status bit and the corresponding mask bit is a 1, set intr
			status_regRRR(30 downto 0) <= status_regRR(30 downto 0);
			if( (status_regRRR(30 downto 0) and mask_reg(30 downto 0))  /= X"00000000") then
				status_regRRR(31) <= mask_reg(31);
			else
				status_regRRR(31) <= '0';
			end if;
		
		end if;
	end process;

	 
	 --This process detects a user space read via the select lines/rdl and 
	 --places the data requested on Local_Data_Out bus to the DMA core. 
	 --For target reads the data should always be placed on the low 32 bits,
	 --For DMA transactions all 64 bits should be used and the DMA core will
	 --handle moving them as dicated by the bus on the system.
    output_data : process (reset, rdl, rev_seln, ctl_seln,
    						ctl_reg, stat_seln, mask_seln,
    						ch0_seln, ch1_seln, ch2_seln, ch3_seln,
							ch4_seln, ch5_seln, ch6_seln, ch7_seln,
							k_addr_seln, k_data_seln, k_read_seln,
							g_addr_seln, g_data_seln, g_read_seln,
							dec_seln, mt_addr_seln, mt_data_seln,
							pp_m_seln, pp_n_seln, iq_index_seln, iq_length_seln,
							gs_dprt_seln, dec_rst_seln, pp_rst_seln, timing_seln,
							svn_rev_seln)
	 begin
		if (reset = '1') then
			Local_Data_Out <= (others => 'Z');
		else
			if (rev_seln = '0' and rdl = '0') then
				Local_Data_Out (63 downto 32) <= (others => '0');
				Local_Data_Out (31 downto 0)  <= X"1013060A";
			elsif (ctl_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 29)  <= (others => '0');
				Local_Data_Out(28 downto 0)   <= ctl_reg;
			elsif (stat_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32)  <= (others => '0');
				Local_Data_Out(31 downto 10)  <= status_regRRR(31 downto 10);
				Local_Data_Out(9 downto 6) 	<= adca_or & adcb_or & adcc_or & adcd_or;
				Local_Data_Out(5 downto 0)    <= status_regRRR(5 downto 0);
			elsif (mask_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32)  <= (others => '0');
				Local_Data_Out(31 downto 0)   <= mask_reg;	
			elsif (ch0_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 0) <= ffa0_data_out;
			elsif (ch1_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 0) <= ffa1_data_out;
			elsif (ch2_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 0) <= ffb0_data_out;
			elsif (ch3_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 0) <= ffb1_data_out;
			elsif (ch4_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 0) <= ffc0_data_out;
			elsif (ch5_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 0) <= ffc1_data_out;
			elsif (ch6_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 0) <= ffd0_data_out;
			elsif (ch7_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 0) <= ffd1_data_out;
			elsif (k_addr_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= k_addr_reg;	
			elsif (k_data_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= k_data_reg;	
			elsif (k_read_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= k_read_reg;	
			elsif (g_addr_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= g_addr_reg;	
			elsif (g_data_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= g_data_reg;	
			elsif (g_read_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= g_read_reg;	
			elsif (dec_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= dec_reg;	
			elsif (dec_rst_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 1) <= (others => '0');
				Local_Data_Out(0)  <= dec_rst;	
			elsif (mt_addr_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= mt_addr_reg;	
			elsif (mt_data_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= mt_data_reg;	
			elsif (pp_m_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= pp_m_reg;	
			elsif (pp_n_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= pp_n_reg;	
			elsif (iq_index_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= iq_index_reg;	
			elsif (iq_length_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= iq_length_reg;	
			elsif (gs_dprt_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 1) <= (others => '0');
				Local_Data_Out(0)  <= gs_dprt_reg;	
			elsif (pp_rst_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 1) <= (others => '0');
				Local_Data_Out(0)  <= pp_rst;	
			elsif (timing_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 1) <= (others => '0');
				Local_Data_Out(0)  <= timing_sel;	
			elsif (svn_rev_seln = '0' and rdl ='0') then
				Local_Data_Out(63 downto 32) <= (others => '0');
				Local_Data_Out(31 downto 0)  <= svn_rev_reg;	
			else
				Local_Data_Out <= (others => 'Z');
			end if;
		end if;
    end process; -- output_data
	 
	-- ============  ADC Capture Logic  =================================================
	-- Based on the control register this logic will handle capturing data from the ADCs 
	-- into the capture FIFOs.
	
	ff_flush		<= ctl_reg(13) or ctl_reg(15) or reset;  --Create FIFO flush signal from either ch flush 
																		  --or soft reset
																			
	--Capture FIFOs are show ahead, so they need to be advanced after each target read.  If 
	--DMAs are being used (ctl_reg(16) is set, they should be advanced unless it is the 
	--last transfer before being broken off, in which case the current value should be 
	--held until the transfer resumes.
	
	ffa0_rd <= (not ch0_seln) and (not ctl_reg(16) or (ctl_reg(16) and Last_Brst_Wd_N and Local_Data_Rdy));
	ffa1_rd <= (not ch1_seln) and (not ctl_reg(16) or (ctl_reg(16) and Last_Brst_Wd_N and Local_Data_Rdy));
	
	ffb0_rd <= (not ch2_seln) and (not ctl_reg(16) or (ctl_reg(16) and Last_Brst_Wd_N and Local_Data_Rdy));
	ffb1_rd <= (not ch3_seln) and (not ctl_reg(16) or (ctl_reg(16) and Last_Brst_Wd_N and Local_Data_Rdy));
	
	ffc0_rd <= (not ch4_seln) and (not ctl_reg(16) or (ctl_reg(16) and Last_Brst_Wd_N and Local_Data_Rdy));
	ffc1_rd <= (not ch5_seln) and (not ctl_reg(16) or (ctl_reg(16) and Last_Brst_Wd_N and Local_Data_Rdy));
	
	ffd0_rd <= (not ch6_seln) and (not ctl_reg(16) or (ctl_reg(16) and Last_Brst_Wd_N and Local_Data_Rdy));
	ffd1_rd <= (not ch7_seln) and (not ctl_reg(16) or (ctl_reg(16) and Last_Brst_Wd_N and Local_Data_Rdy));
	
	
	--This logic sets the adcX_en signal high to start a data capture based on the ctl_reg.  In snapshot mode
	--(ctl_reg(27) low) when the FIFO for a channel goes full the adcX_en signal will be set low stopping the 
	--data collect.  In constant capture mode the enable signal will be held active even if a FIFO overflows.
	adc_captureen : process(reset, user_clk)
	begin
		if (reset = '1')
		then
			adc_enable <= '0';
		elsif rising_edge (user_clk) then
			if(ctl_reg(15) = '1') then   							  --Disable all capture on soft reset
				adc_enable <= '0';
			else
				if(ffa0_full = '1' or ffa1_full = '1' or 
				   ffb0_full = '1' or ffb1_full = '1' or 
					ffc0_full = '1' or ffc1_full = '1' or 
				   ffd0_full = '1' or ffd1_full = '1') then    --If ctl_reg(27) (cont. capture) is not set, stop the caputure 
						adc_enable <= ctl_reg(27);               --when the FIFO goes full.
				elsif(ctl_reg(8) = '1') then                   --Start the capture when the go bit for the ADC is set
					adc_enable <= '1';
				else
					adc_enable <= adc_enable;  				     --Hold current value
				end if;
			end if;
		end if;
	end process;

-- ==================================================
-- = Timing
-- ==================================================
  
--	-- Timers Instantiation
	PRT_Timers : MultiTimer 
	GENERIC MAP(NTIMERS => 4) PORT MAP (
		busclk => user_clk,
		rst => reset,
		addrwr => mt_addr_wr,
		addrin => mt_addr_reg,
		datawr => mt_data_wr,
		timerclk => timer_clk,
		onepps => '0',
		datain => mt_data_reg,
		dataout => open,
		pulseout => pulse_out,
		addrout => open);

   
	-- Gate Splitters
	gate_splitters : gate_splitter_cw PORT MAP (
		a_gate_in_ext => gpio_buf(3),
		a_gate_in_int => pulse_out(0),
		b_gate_in_ext => gpio_buf(2),
		b_gate_in_int => pulse_out(1),
		c_gate_in_ext => gpio_buf(1),
		c_gate_in_int => pulse_out(2),
		ce => adc_enable,
		clk => timer_clk,
		d_gate_in_ext => gpio_buf(0),
		d_gate_in_int => pulse_out(3),
		dual_prt => gs_dprt_reg,
		reset => reset,
		timing_sel => timing_sel,
		a_gate1_out => cha_gate_in(0),
		a_gate2_out => cha_gate_in(1),
		b_gate1_out => chb_gate_in(0),
		b_gate2_out => chb_gate_in(1),
		c_gate1_out => chc_gate_in(0),
		c_gate2_out => chc_gate_in(1),
		d_gate1_out => chd_gate_in(0),
		d_gate2_out => chd_gate_in(1));


	-- DDC Modules
	DDCs : casc_filter_cw PORT MAP (
		ce => adc_enable,
		cha_in => adca_in,
		chb_in => adcb_in,
		chc_in => adcc_in,
		chd_in => adcd_in,
		clk => filter_clk,
		g_addr => g_addr_reg(3 downto 0),
		g_data => g_data_reg(17 downto 0),
		g_sel => g_addr_reg(5 downto 4),
		g_wr => g_write_reg,
		k_addr => k_addr_reg(3 downto 0),
		k_data => k_data_reg(17 downto 0),
		k_sel => k_addr_reg(5 downto 4),
		k_wr => k_write_reg,
		stop => k_addr_reg(12),
		cha_i_out => cha_i,
		cha_q_out => cha_q,
		chb_i_out => chb_i,
		chb_q_out => chb_q,
		chc_i_out => chc_i,
		chc_q_out => chc_q,
		chd_i_out => chd_i,
		chd_q_out => chd_q,
		g_readcoef => g_read_reg(17 downto 0),
		k_readcoef => k_read_reg(17 downto 0));

		
	-- Pulse Pair Processors
	PPPs : pulsepairproc_cw PORT MAP (
		a_gate1 => cha_gate_in(0),
		a_gate2 => cha_gate_in(1),
		a_iin => cha_i,
		a_qin => cha_q,
		b_gate1 => chb_gate_in(0),
		b_gate2 => chb_gate_in(1),
		b_iin => chb_i,
		b_qin => chb_q,
		c_gate1 => chc_gate_in(0),
		c_gate2 => chc_gate_in(1),
		c_iin => chc_i,
		c_qin => chc_q,
		ce => adc_enable,
		clk => dec_clk,
		d_gate1 => chd_gate_in(0),
		d_gate2 => chd_gate_in(1),
		d_iin => chd_i,
		d_qin => chd_q,
		iq_index => iq_index_reg(9 downto 0),
		iq_length => iq_length_reg(9 downto 0),
		m => pp_m_reg(9 downto 0),
		n => pp_n_reg(6 downto 0),
		reset => pp_rst,
		a_adata => cha_a,
		a_bdata => cha_b,
		a_iqdata => cha_iq,
		a_iqgate1 => cha_iq_g(0),
		a_iqgate2 => cha_iq_g(1),
		a_pdata => cha_p,
		a_ppgate1 => cha_pp_g(0),
		a_ppgate2 => cha_pp_g(1),
		a_sync_error => a_sync_error,
		b_adata => chb_a,
		b_bdata => chb_b,
		b_iqdata => chb_iq,
		b_iqgate1 => chb_iq_g(0),
		b_iqgate2 => chb_iq_g(1),
		b_pdata => chb_p,
		b_ppgate1 => chb_pp_g(0),
		b_ppgate2 => chb_pp_g(1),
		b_sync_error => b_sync_error,
		c_adata => chc_a,
		c_bdata => chc_b,
		c_iqdata => chc_iq,
		c_iqgate1 => chc_iq_g(0),
		c_iqgate2 => chc_iq_g(1),
		c_pdata => chc_p,
		c_ppgate1 => chc_pp_g(0),
		c_ppgate2 => chc_pp_g(1),
		c_sync_error => c_sync_error,
		d_adata => chd_a,
		d_bdata => chd_b,
		d_iqdata => chd_iq,
		d_iqgate1 => chd_iq_g(0),
		d_iqgate2 => chd_iq_g(1),
		d_pdata => chd_p,
		d_ppgate1 => chd_pp_g(0),
		d_ppgate2 => chd_pp_g(1),
		d_sync_error => d_sync_error);

-- ==================================================
-- = Channel A Data Flow
-- ==================================================

	--========================
	--= Channel 0 - IQ Data
	--========================  
	-- IQ Data Capture Instantiation
	cha_iq_capture: iq_capture PORT MAP (
		clk          => dec_clk,
		reset        => ff_flush,
		enable       => adc_enable,
		sync         => adc_enable,
		iq_data_in   => cha_iq,
		gate_1       => cha_iq_g(0),
		gate_2       => cha_iq_g(1),
		channel_id   => "0001",
		prt_mode		 => gs_dprt_reg,
		data_valid64 => ffa0_wr,
		iq_data_out  => ffa0_data_in);
 
	-- IQ FIFO Instantiation
	ch0_fifo :  fifo_64x1k_sa PORT MAP (
		din    => ffa0_data_in,
		rd_clk => user_clk,
		rd_en  => ffa0_rd,
		rst    => ff_flush,
		wr_clk => dec_clk,
		wr_en  => ffa0_wr,
		dout   => ffa0_data_out,
		empty  => ffa0_empty,
		full   => ffa0_full,
		rd_data_count => ffa0_rd_cnt);

	--========================
	--= Channel 1 - PP Data
	--========================  
	-- PP Data Capture Instantiation
	cha_pp_capture : pp_capture PORT MAP (
		clk           => fifo_clk,
		reset         => ff_flush,
		enable        => adc_enable,
		sync	        => adc_enable,
		p_data_in     => cha_p,
		a_data_in     => cha_a,
		b_data_in     => cha_b,
		gate_1  		  => cha_pp_g(0),
		gate_2  		  => cha_pp_g(1),
		channel_id	  => "0010",
		prt_mode		  => gs_dprt_reg,
		sync_error    => a_sync_error,
		data_valid64  => ffa1_wr,
		pp_data_out	  => ffa1_data_in);
	
	-- PP FIFO Instantiation
	ch1_fifo :  fifo_64x2k_sa PORT MAP (
		din    => ffa1_data_in,
		rd_clk => user_clk,
		rd_en  => ffa1_rd,
		rst    => ff_flush,
		wr_clk => fifo_clk,
		wr_en  => ffa1_wr,
		dout   => ffa1_data_out,
		empty  => ffa1_empty,
		full   => ffa1_full,
		rd_data_count => ffa1_rd_cnt);
	
 
-- ==================================================
-- = Channel B Data Flow
-- ==================================================

	--========================
	--= Channel 0 - IQ Data
	--========================  
	-- IQ Data Capture Instantiation
	chb_iq_capture: iq_capture PORT MAP (
		clk          => dec_clk,
		reset        => ff_flush,
		enable       => adc_enable,
		sync         => adc_enable,
		iq_data_in   => chb_iq,
		gate_1       => chb_iq_g(0),
		gate_2       => chb_iq_g(1),
		channel_id   => "0011",
		prt_mode		 => gs_dprt_reg,
		data_valid64 => ffb0_wr,
		iq_data_out  => ffb0_data_in);
 
	-- IQ FIFO Instantiation
	ch2_fifo :  fifo_64x1k_sa PORT MAP (
		din    => ffb0_data_in,
		rd_clk => user_clk,
		rd_en  => ffb0_rd,
		rst    => ff_flush,
		wr_clk => dec_clk,
		wr_en  => ffb0_wr,
		dout   => ffb0_data_out,
		empty  => ffb0_empty,
		full   => ffb0_full,
		rd_data_count => ffb0_rd_cnt);

   --========================
   --= Channel 1 - Power Data
   --========================  
	-- PP Data Capture Instantiation
	chb_pp_capture : pp_capture PORT MAP (
		clk           => fifo_clk,
		reset         => ff_flush,
		enable        => adc_enable,
		sync	        => adc_enable,
		p_data_in     => chb_p,
		a_data_in     => chb_a,
		b_data_in     => chb_b,
		gate_1  		  => chb_pp_g(0),
		gate_2  		  => chb_pp_g(1),
		channel_id	  => "0100",
		prt_mode		  => gs_dprt_reg,
		sync_error    => b_sync_error,
		data_valid64  => ffb1_wr,
		pp_data_out	  => ffb1_data_in);
	
	-- PP FIFO Instantiation
	ch3_fifo :  fifo_64x2k_sa PORT MAP (
		din    => ffb1_data_in,
		rd_clk => user_clk,
		rd_en  => ffb1_rd,
		rst    => ff_flush,
		wr_clk => fifo_clk,
		wr_en  => ffb1_wr,
		dout   => ffb1_data_out,
		empty  => ffb1_empty,
		full   => ffb1_full,
		rd_data_count => ffb1_rd_cnt);
	
 
-- ==================================================
-- = Channel C Data Flow
-- ==================================================

	--========================
	--= Channel 0 - IQ Data
	--========================  
	-- IQ Data Capture Instantiation
	chc_iq_capture: iq_capture PORT MAP (
		clk          => dec_clk,
		reset        => ff_flush,
		enable       => adc_enable,
		sync         => adc_enable,
		iq_data_in   => chc_iq,
		gate_1       => chc_iq_g(0),
		gate_2       => chc_iq_g(1),
		channel_id   => "0101",
		prt_mode		 => gs_dprt_reg,
		data_valid64 => ffc0_wr,
		iq_data_out  => ffc0_data_in);
 
	-- IQ FIFO Instantiation
	ch4_fifo :  fifo_64x1k_sa PORT MAP (
		din    => ffc0_data_in,
		rd_clk => user_clk,
		rd_en  => ffc0_rd,
		rst    => ff_flush,
		wr_clk => dec_clk,
		wr_en  => ffc0_wr,
		dout   => ffc0_data_out,
		empty  => ffc0_empty,
		full   => ffc0_full,
		rd_data_count => ffc0_rd_cnt);
		
   --========================
   --= Channel 1 - Power Data
   --========================  
	-- PP Data Capture Instantiation
	chc_pp_capture : pp_capture PORT MAP (
		clk           => fifo_clk,
		reset         => ff_flush,
		enable        => adc_enable,
		sync	        => adc_enable,
		p_data_in     => chc_p,
		a_data_in     => chc_a,
		b_data_in     => chc_b,
		gate_1  		  => chc_pp_g(0),
		gate_2  		  => chc_pp_g(1),
		channel_id	  => "0110",
		prt_mode		  => gs_dprt_reg,
		sync_error    => c_sync_error,
		data_valid64  => ffc1_wr,
		pp_data_out	  => ffc1_data_in);
	
	-- PP FIFO Instantiation
	ch5_fifo :  fifo_64x2k_sa PORT MAP (
		din    => ffc1_data_in,
		rd_clk => user_clk,
		rd_en  => ffc1_rd,
		rst    => ff_flush,
		wr_clk => fifo_clk,
		wr_en  => ffc1_wr,
		dout   => ffc1_data_out,
		empty  => ffc1_empty,
		full   => ffc1_full,
		rd_data_count => ffc1_rd_cnt);
 
-- ==================================================
-- = Channel D Data Flow
-- ==================================================

	--========================
	--= Channel 0 - IQ Data
	--========================  
	-- IQ Data Capture Instantiation
	chd_iq_capture: iq_capture PORT MAP (
		clk          => dec_clk,
		reset        => ff_flush,
		enable       => adc_enable,
		sync         => adc_enable,
		iq_data_in   => chd_iq,
		gate_1       => chd_iq_g(0),
		gate_2       => chd_iq_g(1),
		channel_id   => "0111",
		prt_mode		 => gs_dprt_reg,
		data_valid64 => ffd0_wr,
		iq_data_out  => ffd0_data_in);
 
	-- IQ FIFO Instantiation
	ch6_fifo :  fifo_64x1k_sa PORT MAP (
		din    => ffd0_data_in,
		rd_clk => user_clk,
		rd_en  => ffd0_rd,
		rst    => ff_flush,
		wr_clk => dec_clk,
		wr_en  => ffd0_wr,
		dout   => ffd0_data_out,
		empty  => ffd0_empty,
		full   => ffd0_full,
		rd_data_count => ffd0_rd_cnt);

   --========================
   --= Channel 1 - Power Data
   --========================  
	-- PP Data Capture Instantiation
	chd_pp_capture : pp_capture PORT MAP (
		clk           => fifo_clk,
		reset         => ff_flush,
		enable        => adc_enable,
		sync	        => adc_enable,
		p_data_in     => chd_p,
		a_data_in     => chd_a,
		b_data_in     => chd_b,
		gate_1  		  => chd_pp_g(0),
		gate_2  		  => chd_pp_g(1),
		channel_id	  => "1000",
		prt_mode		  => gs_dprt_reg,
		sync_error    => d_sync_error,
		data_valid64  => ffd1_wr,
		pp_data_out	  => ffd1_data_in);
	
	-- PP FIFO Instantiation
	ch7_fifo :  fifo_64x2k_sa PORT MAP (
		din    => ffd1_data_in,
		rd_clk => user_clk,
		rd_en  => ffd1_rd,
		rst    => ff_flush,
		wr_clk => fifo_clk,
		wr_en  => ffd1_wr,
		dout   => ffd1_data_out,
		empty  => ffd1_empty,
		full   => ffd1_full,
		rd_data_count => ffd1_rd_cnt);
 		
	 -- ==== End ADC Capture logic ====================================================
	 
	 
	 -- ==== ADC DMA Arb Logic ====================================== 
	 -- Define Rd FIFO adr in local space, the DMA core will read from these locations when doing a DMA for
	 -- that channel
	 ch0_src_addr  <= X"00000900";
	 ch1_src_addr  <= X"00000908";
	 ch2_src_addr  <= X"00000910";
	 ch3_src_addr  <= X"00000918";
	 ch4_src_addr  <= X"00000920";
	 ch5_src_addr  <= X"00000928";
	 ch6_src_addr  <= X"00000930";
	 ch7_src_addr  <= X"00000938";
	 
	 
	 --Edge detect DMA start/stop based on bust signal
	 dma_start      <= '1' when ((dma_busyRR = '0') and (dma_busyR = '1')) else '0';
	 dma_done       <= '1' when ((dma_busyRR = '1') and (dma_busyR = '0')) else '0';
	 
	 cur_ch_ext     <= "00" & cur_ch;  --DMA core has 5 bits to support up to 32 channels, we used 2 bits
	 
	 --DMA data from all 32 channels
	 ch_arb : process (user_clk, reset)
	 begin
		if (reset = '1') 
		then
			cur_ch         <= (others => '0');
			dma_busyR      <= '0';
			dma_busyRR     <= '0';
			ch_change_strb <= '1';
			ffa0_ovrthresh <= '0';
			ffa1_ovrthresh <= '0';
			ffb0_ovrthresh <= '0';
			ffb1_ovrthresh <= '0';
			ffc0_ovrthresh <= '0';
			ffc1_ovrthresh <= '0';
			ffd0_ovrthresh <= '0';
			ffd1_ovrthresh <= '0';
			
		elsif rising_edge (user_clk)
		then
			--for edge detect of DMA_Bust
			dma_busyR   <= DMA_Busy;
			dma_busyRR  <= dma_busyR;
			
			--Look for channels that are over threshold
			if(ffa0_rd_cnt >= dma0_block_size(10 downto 0)) then
				ffa0_ovrthresh <= '1';
			else
				ffa0_ovrthresh <= '0';
			end if; 
				
			if(ffa1_rd_cnt >= dma1_block_size(10 downto 0)) then
				ffa1_ovrthresh <= '1';
			else
				ffa1_ovrthresh <= '0';
			end if;
			
			if(ffb0_rd_cnt >= dma2_block_size(10 downto 0)) then
				ffb0_ovrthresh <= '1';
			else
				ffb0_ovrthresh <= '0';
			end if; 
				
			if(ffb1_rd_cnt >= dma3_block_size(10 downto 0)) then
				ffb1_ovrthresh <= '1';
			else
				ffb1_ovrthresh <= '0';
			end if;
			
			if(ffc0_rd_cnt >= dma4_block_size(10 downto 0)) then
				ffc0_ovrthresh <= '1';
			else
				ffc0_ovrthresh <= '0';
			end if; 
				
			if(ffc1_rd_cnt >= dma5_block_size(10 downto 0)) then
				ffc1_ovrthresh <= '1';
			else
				ffc1_ovrthresh <= '0';
			end if;
				
			if(ffd0_rd_cnt >= dma6_block_size(10 downto 0)) then
				ffd0_ovrthresh <= '1';
			else
				ffd0_ovrthresh <= '0';
			end if; 
				
			if(ffd1_rd_cnt >= dma7_block_size(10 downto 0)) then
				ffd1_ovrthresh <= '1';
			else
				ffd1_ovrthresh <= '0';
			end if;
			
			--If a DMA block from a channel has finshed, or the channel needs to be 
			--skipped, move to the next channel in order and set ch_change_strb high
			--which will be the signal to the PCI core we might want to start a new 
			--DMA for this new channel.  
			if(dma_ch_adv = '1' or dma_done = '1') 
			then
				cur_ch          <= cur_ch + X"01";
				ch_change_strb  <= '1';
			else
				cur_ch <= cur_ch;   --Hold the current channel.
					
				if(dma_start = '1') then
					ch_change_strb <= '0';
				else
					ch_change_strb <= ch_change_strb;
				end if;
			end if;
	
		end if;
	end process;
	
	--Assign inputs to the PCI core based on which channel currently has arbed for it.  
	--It will select the local adr to tell the PCI core will read from it it does a DMA, the 
	--dma_go signal (which must be held high until DMA_Busy goes active to start a DMA) and
	--dma_ch_adv, (which caused the arb logic to advance to the next channel.) dma_ch_adv
	--will be set active right away if the current channel is not over thresholdwhich will be set active 
	ch_arb_sm : process (reset, cur_ch, ctl_reg(16), ch_change_strb, DMA_BusyR,
								ffa0_ovrthresh, ffa1_ovrthresh, ffb0_ovrthresh, ffb1_ovrthresh, 
								ffc0_ovrthresh, ffc1_ovrthresh, ffd0_ovrthresh, ffd1_ovrthresh)
								
	begin
		if (reset = '1') then
			cur_src_addr   <= ch0_src_addr;
			dma_go         <= '0';
			dma_ch_adv     <= '0';
		else
			case cur_ch is
					when "000" =>
						cur_src_addr <= ch0_src_addr;
						dma_go       <= ctl_reg(16) and ffa0_ovrthresh and ch_change_strb;
						dma_ch_adv   <= not (ctl_reg(16) and ffa0_ovrthresh) and (not DMA_BusyR);
					when "001" =>
						cur_src_addr <= ch1_src_addr;
						dma_go       <= ctl_reg(16) and ffa1_ovrthresh and ch_change_strb;
						dma_ch_adv   <= not (ctl_reg(16) and ffa1_ovrthresh) and (not DMA_BusyR);
					when "010" =>
						cur_src_addr <= ch2_src_addr;
						dma_go       <= ctl_reg(16) and ffb0_ovrthresh and ch_change_strb;
						dma_ch_adv   <= not (ctl_reg(16) and ffb0_ovrthresh) and (not DMA_BusyR);
					when "011" =>
						cur_src_addr <= ch3_src_addr;
						dma_go       <= ctl_reg(16) and ffb1_ovrthresh and ch_change_strb;
						dma_ch_adv   <= not (ctl_reg(16) and ffb1_ovrthresh) and (not DMA_BusyR);
					when "100" =>
						cur_src_addr <= ch4_src_addr;
						dma_go       <= ctl_reg(16) and ffc0_ovrthresh and ch_change_strb;
						dma_ch_adv   <= not (ctl_reg(16) and ffc0_ovrthresh) and (not DMA_BusyR);
					when "101" =>
						cur_src_addr <= ch5_src_addr;
						dma_go       <= ctl_reg(16) and ffc1_ovrthresh and ch_change_strb;
						dma_ch_adv   <= not (ctl_reg(16) and ffc1_ovrthresh) and (not DMA_BusyR);
					when "110" =>
						cur_src_addr <= ch6_src_addr;
						dma_go       <= ctl_reg(16) and ffd0_ovrthresh and ch_change_strb;
						dma_ch_adv   <= not (ctl_reg(16) and ffd0_ovrthresh) and (not DMA_BusyR);
					when "111" =>
						cur_src_addr <= ch7_src_addr;
						dma_go       <= ctl_reg(16) and ffd1_ovrthresh and ch_change_strb;
						dma_ch_adv   <= not (ctl_reg(16) and ffd1_ovrthresh) and (not DMA_BusyR);
					when others =>
						cur_src_addr <= ch0_src_addr;
						dma_go       <= '0';
						dma_ch_adv   <= '1' and not DMA_BusyR;
				end case;
			end if;
	end process;
   -- ====== End ADC DMA from FIFO arb Logic ==============================
		
   
   
    LBIface : Local_Bus_Interface_Wrapper 
	    PORT MAP (
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
			fifo_flag        => fifo_flag,
			LBAD             => LBAD,
			D                => D,
			Int_to_FPGA      => Int_to_FPGA,
			Int_to_Bridge    => Int_to_Bridge,
	        
			-- User interface
		      PCIclk               => user_clk,				--o, User side interface clk, generated by DMA core
		      PCIclk_2x 		   	=> open,
		      Rst                  => reset,		   		--o, Rst from DMA core
		      Srst                 => '0',			   		--i, Soft Reset, not implemented
		      Base_Addr            => DMABaseAddr,   		--i, Starting address of DMA core in BAR2, defaults to 0
		      Local_Addr           => cur_src_addr,  		--i, Where the DMA core will read from when moving DMA data, set to 0x808
		      DMA_SG_En            => '1',			   		--i, DMA Scatter/Gather Enable ???
		      DMA_Channel          => cur_ch_ext,					--i, Which channel to transfer, 0-3
		      DMA_Go               => dma_go,	      		--i, Kicks off a DMA for the channel selected by DMA_Channel 
		      DMA_Busy             => DMA_Busy,	   			--o, DMA in progress
		      DMA_Direction        => '0',			   		--i, 0 for MEMWR, 1 for MEMRD
		      Local_Data_ADS_N     => Local_Data_ADS_N,		--o, Low for access from LB
		      Local_Data_Rdy_N     => Local_Data_Rdy_N, 	--i, Low when inputs on Local_Data_Out are valid
		      Local_Data_Rd_N      => rdl,			   		--o, Low for local Rd
		      Local_Data_Wr_N      => wrl,		 	   		--o, Low for local Wr
		      Local_Data_Addr      => Local_Data_Addr, 		--Addr for local xaction
		      Local_Data_In        => Local_Data_In,	 	--o, Incomming data from PCI via the bridge
		      Local_Data_Out       => Local_Data_Out, 		--io, Outging data to PCI via the bridge 
		      PCI_int              => DMA_Interrupt_Status, --i, Set to one to cause PCI interrupt
		      Last_Brst_Wd_N       => Last_Brst_Wd_N,  		--o, Low when current rd is last of a burst transfer
		      DMA_Ch0_BlkSize      => dma0_block_size,  	--Ch0 Block Size in 64bit words
		      DMA_Ch1_BlkSize      => dma1_block_size,
		      DMA_Ch2_BlkSize      => dma2_block_size,
		      DMA_Ch3_BlkSize      => dma3_block_size,
				DMA_Ch4_BlkSize      => dma4_block_size,  	
		      DMA_Ch5_BlkSize      => dma5_block_size,
		      DMA_Ch6_BlkSize      => dma6_block_size,
		      DMA_Ch7_BlkSize      => dma7_block_size,
		      DMA_Last_Blk		   => open,
		      DMA_Interrupt_Status => DMA_Interrupt_Status);  
		
		
		
end behavioral;