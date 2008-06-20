
--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:38:56 05/14/2008
-- Design Name:   decimator
-- Module Name:   C:/Work_Projects/eldora/firmware/redrapids_firmware_test/decimator_tb.vhd
-- Project Name:  ChannelAdapter_SX35
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: decimator
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

ENTITY decimator_tb_vhd IS
END decimator_tb_vhd;

ARCHITECTURE behavior OF decimator_tb_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT decimator
	PORT(
		clk_in : IN std_logic;
		trigger : IN std_logic;
		reset : IN std_logic;
		decimation : IN std_logic_vector(7 downto 0);          
		dec_clk : OUT std_logic;
		fifo_clk : OUT std_logic
		);
	END COMPONENT;

	--Inputs
	SIGNAL clk_in :  std_logic := '1';
	SIGNAL trigger : std_logic := '0';
	SIGNAL reset :  std_logic := '0';
	SIGNAL decimation :  std_logic_vector(7 downto 0) := (others=>'0');

	--Outputs
	SIGNAL dec_clk :  std_logic;
	SIGNAL fifo_clk :  std_logic;

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: decimator PORT MAP(
		clk_in => clk_in,
		trigger => trigger,
		reset => reset,
		decimation => decimation,
		dec_clk => dec_clk,
		fifo_clk => fifo_clk
	);

   clk_in <= not clk_in after 50 ns;
   
	tb : PROCESS
	BEGIN

		-- Wait 100 ns for global reset to finish
		wait for 100 ns;
      
		decimation <= "00001100";
		wait for 200 ns;
		trigger <= '1';
		wait for 200 ns;
		reset <= '1';
		wait for 100 ns;
		reset <= '0';
		wait for 200 ns;
      trigger <= '0';

		wait for 4000 ns;
		reset <= '1';
		wait for 100 ns;
		reset <= '0';
		wait for 2000 ns;
		trigger <= '1';
		wait for 100 ns;
		trigger <= '0';

		-- Place stimulus here

		wait; -- will wait forever
	END PROCESS;

END;
