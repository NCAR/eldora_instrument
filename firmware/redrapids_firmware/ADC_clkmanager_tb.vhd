
--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   09:47:43 05/14/2008
-- Design Name:   adc_dcm_m314
-- Module Name:   C:/Work_Projects/eldora/firmware/redrapids_firmware_test/ADC_clkmanager_tb.vhd
-- Project Name:  ChannelAdapter_SX35
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: adc_dcm_m314
--
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends 
-- that these types always be used for the top-level I/O of a design in order 
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;

ENTITY ADC_clkmanager_tb_vhd IS
END ADC_clkmanager_tb_vhd;

ARCHITECTURE behavior OF ADC_clkmanager_tb_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT adc_dcm_m314
	PORT(
		rst : IN std_logic;
		sampleclk_p : IN std_logic;
		sampleclk_n : IN std_logic;          
		adc_clk : OUT std_logic;
		filter_clk : OUT std_logic;
		start_clk : OUT std_logic;
		locked : OUT std_logic
		);
	END COMPONENT;

	--Inputs
	SIGNAL rst :  std_logic := '0';
	SIGNAL sampleclk_p :  std_logic := '1';
	SIGNAL sampleclk_n :  std_logic := '0';

	--Outputs
	SIGNAL adc_clk :  std_logic;
	SIGNAL filter_clk :  std_logic;
	SIGNAL start_clk :  std_logic;
	SIGNAL locked :  std_logic;

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: adc_dcm_m314 PORT MAP(
		rst => rst,
		sampleclk_p => sampleclk_p,
		sampleclk_n => sampleclk_n,
		adc_clk => adc_clk,
		filter_clk => filter_clk,
		start_clk => start_clk,
		locked => locked
	);

   sampleclk_p <= not sampleclk_p after 100 ns;
	sampleclk_n <= not sampleclk_n after 100 ns;

	tb : PROCESS
	BEGIN

		-- Wait 100 ns for global reset to finish
		wait for 100 ns;

		-- Place stimulus here

		--rst <= '1';
		--wait for 100 ns;
		--rst <= '0';
		

		wait; -- will wait forever
	END PROCESS;

END;
