
--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   10:29:41 05/14/2008
-- Design Name:   timer_dcm_m314
-- Module Name:   C:/Work_Projects/eldora/firmware/redrapids_firmware_test/Timer_clkmanager_tb.vhd
-- Project Name:  ChannelAdapter_SX35
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: timer_dcm_m314
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

ENTITY Timer_clkmanager_tb_vhd IS
END Timer_clkmanager_tb_vhd;

ARCHITECTURE behavior OF Timer_clkmanager_tb_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT timer_dcm_m314
	PORT(
		rst : IN std_logic;
		start_clk : IN std_logic;          
		timer_clk : OUT std_logic;
		locked : OUT std_logic
		);
	END COMPONENT;

	--Inputs
	SIGNAL rst :  std_logic := '0';
	SIGNAL start_clk :  std_logic := '1';

	--Outputs
	SIGNAL timer_clk :  std_logic;
	SIGNAL locked :  std_logic;

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: timer_dcm_m314 PORT MAP(
		rst => rst,
		start_clk => start_clk,
		timer_clk => timer_clk,
		locked => locked
	);

   start_clk <= not start_clk after 100 ns;

	tb : PROCESS
	BEGIN

		-- Wait 100 ns for global reset to finish
		wait for 100 ns;

		-- Place stimulus here

		wait; -- will wait forever
	END PROCESS;

END;
