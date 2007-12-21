----********************************************************************************************
-- Red Rapids
-- 797 N. Grove Rd, Suite 101
-- Richardson, TX 75081
-- www.redrapids.com (support@redrapids.com)
--
-- File: adccapture_m314.vhd - Release DSK-320-003-R05 - Thu Dec 21 17:19:56 CST 2006
--

-- Author       :  PTJ 
-- Description  : 	This module is used to interface with an AD9430 12-bit, 210 MHz ADC.  
--					12-bit 2's data from the ADC is captured and sign extended to 16bits.  Four
--					16-bit words are packed to form a 64-bit word to be written to the capture FIFOs.
--					A write signal to the FIFOs is generated every 4th clock.  Every 2nd clk a write signal
--					is generated for the SRAMs, indicationg the bottom 32bits of the data_out bus is valid.
--					A count can be used in place of the ADC data for debug.
--					
--
-- Rev History : 	1.1.0 (08192600) - Added snapshot count feature
--					1.0.0 (06092006) - Inital Release
--
-- Requires	:	None
--
--*******************************************************************************************

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use IEEE.numeric_std.all; 

entity iq_capture is port (
	clk           : in std_logic;	--ADC capture clk
	reset         : in std_logic;	--reset, active high
	enable        : in std_logic;	--capture ADC data based on sync conditions
	sync	        : in std_logic;	--sync, active high.  Must sync after enable goes high to start data capture
	iq_data_in    : in std_logic_vector(31 downto 0); --IQ data from DDC
	gate_1  		  : in std_logic; -- Data Gate PRT 1
	gate_2  		  : in std_logic; -- Data Gate PRT 2
	channel_id	  : in std_logic_vector(3 downto 0); -- Data Tag
	prt_mode		  : in std_logic; -- PRT Mode, 1 = dual PRT, 0 = single PRT
	data_valid64  : out std_logic; --data_out[63:0] is valid
	iq_data_out	  : out std_logic_vector(63 downto 0)); --4 ADC words packed
end iq_capture;	


architecture behavioral of iq_capture is
	signal iq_inR       : std_logic_vector(31 downto 0); --Shift register for IQ data
	signal iq_inRR      : std_logic_vector(31 downto 0);
	signal adc_wr_shift  : std_logic_vector(1 downto 0); --Create dv every 2nd clock
	
	signal go 			: std_logic;  --Logic 1 alows ADC data through shift register
	signal goR			: std_logic;  --Registered version of go
	
	signal ch_id_tag	: std_logic;  --Flag, signifies when to insert channel id data tag
	signal pls_cnt_tag: std_logic;  --Flag, signifies when to insert pulse count data tag
	signal prt_tag		: std_logic;  --Flag, signifies when to insert prt data tag
	
	signal pulse_count: std_logic_vector(31 downto 0); -- Pulse Count Register

begin
		
    --assign shift register to output bus
	iq_data_out <= iq_inRR & iq_inR;
				
	--Moves ADC data through the shift register.  Handles start/stop of data capture
	iq_datacapture : process (clk, reset)
	begin
		if (reset = '1') 
		then
		   IQ_inR		 <= (others => '0');
			IQ_inRR		 <= (others => '0');
			go           <= '0';
			goR          <= '0';
			data_valid64 <= '0';
			adc_wr_shift <= "01";
			ch_id_tag	 <= '0';
			pls_cnt_tag	 <= '0';
			prt_tag	 	 <= '0';
			pulse_count  <= (others => '0');
		elsif rising_edge (clk) 
		then
			
			--Register go for edge detection
			goR <= go; 
			
			--Check to see if ADC enable has been set, if not return to starting condition
			if(enable = '0') then
				go <= '0';
			else  
				if(sync = '1') then --Were waiting on a sync and now go it
					go <= '1';
				else  --Go holds current state
					go <= go;
				end if;
			end if;
			
		
			--ADC Data Capture, create the write signal every 2nd clk
    		if(go = '1' and (gate_1 = '1' or gate_2 = '1')) 
			then
				data_valid64  <= adc_wr_shift(1);
				adc_wr_shift  <= adc_wr_shift(0) & adc_wr_shift(1);
			else
				data_valid64 <= '0';
				--adc_wr_shift <= "01";
				ch_id_tag    <= '0';
				prt_tag      <= '0';
				pls_cnt_tag  <= '0';
			end if;
			
			
			--Register incomming ADC data into a 64bit value to post to FIFO.  If sim mode is selected 
			--make the sim counter the input to the shift register
			if (ch_id_tag = '0' and (gate_1 = '1' or gate_2 = '1')) then
				iq_inR(31 downto 4) <= (others=>'0');
				iq_inR(3 downto 0) <= channel_id;
				ch_id_tag <= '1';
			elsif (prt_tag = '0' and gate_1 = '1') then
				iq_inR(31 downto 2) <= (others => '0');
				iq_inR(1 downto 0) <= "01";
				prt_tag <= '1';
			elsif (prt_tag = '0' and gate_2 = '1') then
				iq_inR(31 downto 2) <= (others => '0');
				iq_inR(1 downto 0) <= "10";
				prt_tag <= '1';
			elsif (pls_cnt_tag = '0' and gate_1 = '1') then
				iq_inR <= pulse_count;
				if (prt_mode = '0') then 
					pulse_count <= pulse_count + 1;
				end if;
				pls_cnt_tag <= '1';
			elsif (pls_cnt_tag = '0' and gate_2 = '1') then
				iq_inR <= pulse_count;
				pulse_count <= pulse_count + 1;
				pls_cnt_tag <= '1';
			elsif (gate_1 = '1' or gate_2 = '1') then
				iq_inR <= iq_data_in;	-- IQ Data
			else
				iq_inR <= iq_inR;
			end if;
			
			if (gate_1 = '1' or gate_2 = '1') then
				iq_inRR <= iq_inR; --Shift Data
			end if;
			
		end if;
	end process;
	
end behavioral;
