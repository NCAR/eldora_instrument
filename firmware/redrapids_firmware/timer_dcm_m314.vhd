----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    11:23:52 01/08/2008 
-- Design Name: 
-- Module Name:    timer_dcm_m314 - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity filter_dcm_m314 is
	GENERIC(
		 CLKIN_PERIOD 	  		: real := 20.8333;
		 DLL_FREQUENCY_MODE	: string := "LOW" ;
		 DFS_FREQUENCY_MODE	: string := "LOW" ;
		 CLKFX_MULTIPLY		: integer := 3;
		 CLKFX_DIVIDE			: integer := 1;
		 PHASE_SHIFT        	: integer := 0);
    PORT(
    	rst			: in std_logic;
    	adc_clk  	: in std_logic;  	
    	filter_clk   : out std_logic;
    	locked		: out std_logic); 
end filter_dcm_m314;

architecture Behavioral of filter_dcm_m314 is


	signal adc_clk0			: std_logic;
	signal adc_clk3x     	: std_logic;
	signal adc_clk_int		: std_logic;

begin

	ADC_CLK_BUFG_INST 		: BUFG PORT MAP(I => adc_clk0, O => adc_clk_int);
	FILTER_CLK_BUFG_INST		: BUFG PORT MAP(I => adc_clk3x, O => filter_clk);

	
	DCM_ADV_HI_INST2 : DCM_ADV
	    GENERIC MAP (
	       CLKIN_PERIOD 	   	=> CLKIN_PERIOD,	--48 MHz  
	       DLL_FREQUENCY_MODE  => DLL_FREQUENCY_MODE,
	       DFS_FREQUENCY_MODE  => DFS_FREQUENCY_MODE,
	       CLKOUT_PHASE_SHIFT  => "FIXED",
	       CLKFX_MULTIPLY		=> CLKFX_MULTIPLY,
			 CLKFX_DIVIDE			=> CLKFX_DIVIDE,
	       PHASE_SHIFT         => PHASE_SHIFT) 
		PORT MAP (
				RST          =>  rst,
	       	PSCLK        =>  '0',
		   	PSEN         =>  '0',
		   	PSINCDEC     =>  '0',
		        
		   	CLKIN        =>  adc_clk,
		   	CLKFB        =>  adc_clk_int,
		   
		   	LOCKED       =>  locked,
				CLK0         =>  adc_clk0,
				CLKDV        =>  open,
				CLK180       =>  open,
				CLK270       =>  open,
				CLK2X        =>  open,
				CLK2X180     =>  open,
				CLK90        =>  open,
				CLKFX        =>  adc_clk3x,
				CLKFX180     =>  open);

end Behavioral;
