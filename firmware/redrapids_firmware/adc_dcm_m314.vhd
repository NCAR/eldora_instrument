----********************************************************************************************
-- Red Rapids
-- 797 N. Grove Rd, Suite 101
-- Richardson, TX 75081
-- www.redrapids.com (support@redrapids.com)
--
-- File: adc_dcm_m314.vhd - Release DSK-320-003-R05 - Thu Dec 21 17:19:56 CST 2006
--


library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use IEEE.numeric_std.all; 

Library UNISIM; 
use UNISIM.Vcomponents.all; 


entity adc_dcm_m314 is
	GENERIC(
		 CLKIN_PERIOD 	  		: real := 20.8333;
		 DLL_FREQUENCY_MODE	: string := "LOW" ;
		 DFS_FREQUENCY_MODE	: string := "LOW" ;
		 CLKDV_DIVIDE        : real := 12.0;
		 CLKFX_MULTIPLY		: integer := 3;
		 CLKFX_DIVIDE			: integer := 1;
		 PHASE_SHIFT        	: integer := 0);
    PORT(
    	rst			: in std_logic;
    	sampleclk_p	: in std_logic;
    	sampleclk_n	: in std_logic;
    	
    	adc_clk		: out std_logic;
    	filter_clk	: out std_logic;
		timer_clk   : out std_logic;
    	locked		: out std_logic); 
end adc_dcm_m314;
    	
architecture behavioral of adc_dcm_m314 is     	
	
	signal adc_clk_ibufg 	: std_logic;
	signal adc_dcm_clk0		: std_logic;
	signal adc_dcm_clk3x 	: std_logic;
	signal adc_clk_int		: std_logic;
	signal adc_dcm_clk_div6 : std_logic;
	
begin 
   ADC_CLK_IBUFG_INST 		: IBUFGDS PORT MAP (I => sampleclk_p, IB=> sampleclk_n, O=> adc_clk_ibufg);
	ADC_CLK_BUFG_INST 		: BUFG PORT MAP(I => adc_dcm_clk0, O => adc_clk_int);
	FILTER_CLK_BUFG_INST  	: BUFG PORT MAP(I => adc_dcm_clk3x, O => filter_clk);
	TIMER_CLK_BUFG_INST		: BUFG PORT MAP(I => adc_dcm_clk_div6, O => timer_clk);
	
	adc_clk <= adc_clk_int;

	
	DCM_ADV_HI_INST : DCM_ADV
	    GENERIC MAP (
	       CLKIN_PERIOD 	   	=> CLKIN_PERIOD,	--48 MHz  
	       DLL_FREQUENCY_MODE  => DLL_FREQUENCY_MODE,
	       DFS_FREQUENCY_MODE  => DFS_FREQUENCY_MODE,
	       CLKOUT_PHASE_SHIFT  => "FIXED",
			 CLKDV_DIVIDE        => CLKDV_DIVIDE,
	       CLKFX_MULTIPLY		=> CLKFX_MULTIPLY,
			 CLKFX_DIVIDE			=> CLKFX_DIVIDE,
	       PHASE_SHIFT         => PHASE_SHIFT) 
		PORT MAP (
				RST          =>  rst,
	       	PSCLK        =>  '0',
		   	PSEN         =>  '0',
		   	PSINCDEC     =>  '0',
		        
		   	CLKIN        =>  adc_clk_ibufg,
		   	CLKFB        =>  adc_clk_int,
		   
		   	LOCKED       =>  locked,
				CLK0         =>  adc_dcm_clk0,
				CLKDV        =>  adc_dcm_clk_div6,
				CLK180       =>  open,
				CLK270       =>  open,
				CLK2X        =>  open,
				CLK2X180     =>  open,
				CLK90        =>  open,
				CLKFX        =>  adc_dcm_clk3x,
				CLKFX180     =>  open);
        
end behavioral;
    	
    	