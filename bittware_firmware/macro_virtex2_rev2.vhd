-- Disclaimer : These designs are provided "as is" with
--              no warranty whatsoever, and xilinx specifically
--              disclaims any implied warranties of merchantability,
--              fitness for a particular purpose, or against infringement.
--
-- Tx/Rx macro_virtex2_rev2 for tiger sharc link
--
-- Assumptions
--
-- 1. The links are running at the same speed as the DSP, ie phase difference only
-- 2. Receiver data will be pulled out as soon as available - no handshake. Valid data
--    is indicated by rxdatavalid_int going high
-- 3. No tx handshaking - just a back pressure signal. Data for transmission can
--    be written anytime the signal txbp is low.

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;

--pragma translate_off
library unisim ;
use unisim.vcomponents.all ;
--pragma translate_on

entity macro_virtex2_rev2 is port (
	clk		: in std_logic ;				-- system clock = twice link frequency 
	lxclkin 	: in std_logic ;				-- lxclkin
	lxdatain 	: in std_logic_vector(7 downto 0) ; -- lxdatain
	lxdataout : out std_logic_vector(7 downto 0) ; -- lxdout
	rst 		: in std_logic ;				-- reset active high
	verein 	: in std_logic ;				-- 1 enables tx_vere function
	psize 	: in std_logic ;				-- 0 = 64 QW Tx, 1 = indefinite Tx
	lxclkout	: out std_logic ;				-- lxclkout
	lxdir	: out std_logic ;				-- lxdir_int
	txdata	: in std_logic_vector(7 downto 0) ; -- data for transmission
	txwrite 	: in std_logic ;				-- write transmit buffer
	rxbp	 	: in std_logic ;				-- receiver back pressure, halt reception when high
	txbp	 	: out std_logic ;				-- transmitter back pressure, do not write when high
	rxdata	 	: out std_logic_vector(7 downto 0) ; -- received data
	rx_vere_bad	: out std_logic ;			-- received data had a tx_vere error = [RER(0)]
	rx_timeout	: out std_logic ;			-- <16 bytes received in 256 cycles = [RER(1)]
	tx_ack_bad 	: out std_logic ;			-- No Acknowledge received during transmit = [TER(0)]
	tx_timeout 	: out std_logic ;			-- LxCLKin low for > 2048 cycles = [TER(1)]
	rst_timeout 	: in std_logic ;			-- reset the timeout bits
	rxdatavalid 	: out std_logic) ;			-- rx data is valid

end macro_virtex2_rev2;

architecture arch_macro_virtex2_rev2 of macro_virtex2_rev2 is
-- Shifter using SRLC16E, do not use with Virtex2 Enginnering Silicon
component shifter_virtex2 port (
	d 	: in std_logic;
	clk 	: in std_logic;
	ce 	: in std_logic;
	a0 	: in std_logic;
	a1 	: in std_logic;
	a2 	: in std_logic;
	a3 	: in std_logic;
	a4 	: in std_logic;
	rst 	: in std_logic;
	q 	: out std_logic);
end component;
-- Shifter using RAM32x1D, do not use with Virtex2 Production Silicon
component ram_shifter port (
	d 	: in std_logic;
	clk 	: in std_logic;
	ce 	: in std_logic;
	a0 	: in std_logic;
	a1 	: in std_logic;
	a2 	: in std_logic;
	a3 	: in std_logic;
	a4 	: in std_logic;
	rst 	: in std_logic;
	q 	: out std_logic);
end component;
-- fdc primitive
component fdc port (d, c, clr : in std_logic; q : out std_logic); end component;
-- fdc_1 primitive
component fdc_1 port (d, c, clr : in std_logic; q : out std_logic); end component;
-- fdp primitive
component fdp port (d, c, pre : in std_logic; q : out std_logic); end component;
-- fdp_1 primitive
component fdp_1 port (d, c, pre : in std_logic; q : out std_logic); end component;
-- RAM16X1D primitive
component ram16x1d port (d, we, wclk, a0, a1, a2, a3, dpra0, dpra1, dpra2, dpra3 : in std_logic ;
			 spo, dpo : out std_logic) ; end component;

signal high : std_logic ;
signal low : std_logic ;
signal enablewr : std_logic ;
signal lxclkout_d : std_logic ;
signal lxclkout_dd : std_logic ;
signal disablewr : std_logic ;
signal ramoea : std_logic ;
signal ramoe : std_logic ;
signal txreq : std_logic ;
signal notlxclkin : std_logic ;
signal lxclkint : std_logic ;
signal lxclkintp : std_logic ;
signal lxclkintpp : std_logic ;
signal lxclkintn : std_logic ;
signal wa : std_logic_vector(3 downto 0) ;
signal ra : std_logic_vector(4 downto 0) ;
signal lxdataint : std_logic_vector(15 downto 0) ;
signal state : integer range 0 to 255 ;
signal shift_tx : std_logic ;
signal shift_txd : std_logic ;
signal gcnt : std_logic_vector(3 downto 0) ;
signal txhadd : std_logic_vector(4 downto 0) ;
signal txhq : std_logic_vector(7 downto 0) ;
signal rxdatavalid_inta : std_logic ;
signal txreqd : std_logic ;
signal tx_vere_reg : std_logic_vector(8 downto 0) ;
signal rx_vere_reg : std_logic_vector(8 downto 0) ;
signal lxdataout_int : std_logic_vector(7 downto 0) ;
signal lxdataout_int_d : std_logic_vector(7 downto 0) ;
signal pcount : std_logic_vector(5 downto 0) ;
signal txtimeout : std_logic ;
signal txtocounter : std_logic_vector(11 downto 0) ;
signal rxtocounter : std_logic_vector(8 downto 0) ;
signal rxtoen : std_logic ;
signal rxdata_int : std_logic_vector(7 downto 0) ;
signal rxdatavalid_int : std_logic ;
signal mux : std_logic ;
signal lxcounth : std_logic_vector(3 downto 0) ;
signal lxcounthflag : std_logic ;
signal localenablewr : std_logic ;
signal localenablewr_rt : std_logic ;
signal ramflag : std_logic ;
signal ackflag : std_logic ;
signal ramflag_rt : std_logic ;
signal ackflag_rt : std_logic ;
signal lxdir_int : std_logic ;
signal rxvtflag : std_logic ;

attribute rloc : string ;
attribute iob : string ;

attribute iob	of fo0: label is "true" ;
attribute iob	of fo1: label is "true" ;
attribute iob	of fo2: label is "true" ;
attribute iob	of fo3: label is "true" ;
attribute iob	of fo4: label is "true" ;
attribute iob	of fo5: label is "true" ;
attribute iob	of fo6: label is "true" ;
attribute iob	of fo7: label is "true" ;
attribute iob	of fo8: label is "true" ;

begin

high <= '1' ;
low  <= '0' ;
lxdataout <= lxdataout_int ;
tx_timeout <= txtimeout ;
notlxclkin <= not lxclkin ;
rxdata <= rxdata_int ;
rxdatavalid <= rxdatavalid_inta ;

-- retime lxclkin to clk
lxcff0 : fdp_1 port map (d => lxclkin,   c => clk, pre => rst, q => lxclkintn );
lxcff1 : fdp   port map (d => lxclkin,   c => clk, pre => rst, q => lxclkintp );
lxcff2 : fdp   port map (d => lxclkintn, c => clk, pre => rst, q => lxclkintpp );
-- retime localenablewr to clk
lxcff4 : fdc   port map (d => localenablewr, c => clk, clr => rst, q => localenablewr_rt );
-- retime ramflag to clk
lxcff5 : fdc   port map (d => ramflag, c => clk, clr => rst, q => ramflag_rt );
-- retime ackflag to clk
lxcff6 : fdc   port map (d => ackflag, c => clk, clr => rst, q => ackflag_rt );

loop0 : for i in 0 to 7 generate		-- tx holding regs

--txs0 : ram_shifter port map(			-- use this line with VirtexII ES silicon
txs0 : shifter_virtex2 port map(		-- use this line with VirtexII production silicon
	d 	=> txdata(i),
	clk 	=> clk,
	ce 	=> txwrite,
	a0 	=> txhadd(0),
	a1 	=> txhadd(1),
	a2 	=> txhadd(2),
	a3 	=> txhadd(3),
	a4 	=> txhadd(4),
	rst 	=> rst,
	q 	=> txhq(i) );

end generate ;

process (clk, rst)				-- pointer for tx holding regs
begin
if rst = '1' then
	txhadd <= "11111" ;
	txreq <= '0' ;
	txreqd <= '0' ;
	txbp <= '0' ;
elsif clk'event and clk = '1' then
	txreqd <= txreq ;
	if txwrite = '1' and shift_tx = '0' then
		txhadd <= txhadd + 1 ;
	elsif shift_tx = '1' and txwrite = '0' then
		txhadd <= txhadd - 1 ;
	end if ;
	if txhadd > 2 and txhadd < 31 then
		txreq <= '1' ;
	else
		txreq <= '0' ;
	end if ;
	if txhadd > 26 and txhadd < 31 then
		txbp <= '1' ;
	else
		txbp <= '0' ;
	end if ;
end if ;
end process ;

process (clk, rst)			-- timeout process
begin
if rst = '1' then
	txtocounter <= (others => '0') ;
	rxtocounter <= (others => '0') ;
	txtimeout <= '0' ;
	rx_timeout <= '0' ;
elsif clk'event and clk = '1' then
	if rxtoen = '1' then
		rxtocounter <= rxtocounter + 1 ;
	else
		rxtocounter <= (others => '0' ) ;
	end if ;
	if rxtocounter = "111111111" then
		rx_timeout <= '1' ;
	end if ;
	if lxclkintp = '0' then
		txtocounter <= txtocounter + 1 ;
	else
		txtocounter <= (others => '0') ;
	end if ;
	if txtocounter = "111111111111" then
		txtimeout <= '1' ;
	end if ;
	if rst_timeout = '1' then
		txtimeout <= '0' ;
		rx_timeout <= '0' ;
		txtocounter <= (others => '0') ;
		rxtocounter <= (others => '0') ;
	end if ;

end if ;
end process ;

process (clk, rst) 				-- Tx VERE function
begin
if rst = '1' then
	lxdataout_int_d <= "00000000" ;
	tx_vere_reg <= "000000000" ;
	shift_txd <= '0' ;
elsif clk'event and clk = '1' then
	if mux = '1' then
		lxdataout_int_d <= tx_vere_reg(7 downto 0) ;
	else
		lxdataout_int_d <= txhq ;
	end if ;
	shift_txd <= shift_tx ;
	if shift_txd = '1' then
		tx_vere_reg <= ('0' & tx_vere_reg(7 downto 0)) + ('0' & txhq) + ("0000000" & tx_vere_reg(8)) ;
	else
		tx_vere_reg <= "000000000" ;
	end if ;
end if ;
end process ;

process (clk, rst) 				-- Rx VERE function
begin
if rst = '1' then
	rx_vere_reg <= "000000000" ;
	rx_vere_bad <= '0' ;
elsif clk'event and clk = '1' then
	if rxdatavalid_inta = '1' and verein = '1' then
		rx_vere_reg <= ('0' & rx_vere_reg(7 downto 0)) + ('0' & rxdata_int) + ("0000000" & rx_vere_reg(8)) ;
	else
		rx_vere_reg <= "000000000" ;
	end if ;
	if rxvtflag = '1' and verein = '1' then
		if rx_vere_reg(7 downto 0) = rxdata_int then
			rx_vere_bad <= '0' ;
		else
			rx_vere_bad <= '1' ;
		end if ;
	else
		rx_vere_bad <= '0' ;
	end if ;
end if ;
end process ;

-- output data changes on -ve edge of clk

fo0 : fdc_1 port map (c => clk, d => lxdataout_int_d(0), clr => rst, q => lxdataout_int(0) );
fo1 : fdc_1 port map (c => clk, d => lxdataout_int_d(1), clr => rst, q => lxdataout_int(1) );
fo2 : fdc_1 port map (c => clk, d => lxdataout_int_d(2), clr => rst, q => lxdataout_int(2) );
fo3 : fdc_1 port map (c => clk, d => lxdataout_int_d(3), clr => rst, q => lxdataout_int(3) );
fo4 : fdc_1 port map (c => clk, d => lxdataout_int_d(4), clr => rst, q => lxdataout_int(4) );
fo5 : fdc_1 port map (c => clk, d => lxdataout_int_d(5), clr => rst, q => lxdataout_int(5) );
fo6 : fdc_1 port map (c => clk, d => lxdataout_int_d(6), clr => rst, q => lxdataout_int(6) );
fo7 : fdc_1 port map (c => clk, d => lxdataout_int_d(7), clr => rst, q => lxdataout_int(7) );

fo8 : fdp port map (c => clk, d => lxclkout_dd, pre => rst, q => lxclkout );
lxclkout_dd <= lxclkout_d and not ackflag_rt ;

process (notlxclkin, rst, disablewr)			-- process for writing rx fifo
begin
if rst = '1' or disablewr = '1' then
	wa <= "0000" ;
	localenablewr <= '0' ;
	ramflag <= '0' ;
	ackflag <= '0' ;
elsif notlxclkin'event and notlxclkin = '1' then
	if enablewr = '1' or localenablewr = '1' then
		if (wa = "0000" or wa = "0001" or wa = "0010") then
			ramflag <= '1' ;
		else
			ramflag <= '0' ;
		end if ;
		if wa = "0001" or wa = "0000" then
			ackflag <= '1' ;
		elsif wa = "0010" then
			ackflag <= '0' ;
		end if ;
		if verein = '0' and wa = "0111" then
			wa <= "0000" ;
		elsif verein = '1' and wa = "1000" then
			wa <= "0000" ;
		else
			wa <= wa + 1 ;
		end if ;
	end if ;
	if enablewr = '1' then
		localenablewr <= '1' ;
	end if ;
end if ;
end process ;

process (clk, rst)				-- process for reading rx fifo
begin
if rst = '1' then
	ra <= "00000" ;
	ramoe <= '0' ;
	rxvtflag <= '0' ;
	rxdatavalid_int <= '0' ;
	rxdatavalid_inta <= '0' ;
elsif clk'event and clk = '1' then
	rxdatavalid_inta <= rxdatavalid_int ;
	if ramoe = '0' and ramflag_rt = '1' then
		ramoe <= '1' ;
		rxdatavalid_int <= '1' ;
		rxvtflag <= '0' ;
	elsif ramoe = '1' and verein = '0' and ra = "01111" then
		rxdatavalid_int <= ramflag_rt ;
		ra <= "00000" ;
		ramoe <= ramflag_rt ;
		rxvtflag <= '0' ;
	elsif ramoe = '1' and verein = '1' and ra = "01111" then
		rxdatavalid_int <= '0' ;
		ra <= ra + 1 ;
		rxvtflag <= '0' ;
	elsif ramoe = '1' and verein = '1' and ra = "10000" then
		rxvtflag <= '1' ;
		ra <= "00000" ;
		ramoe <= ramflag_rt ;
		rxdatavalid_int <= ramflag_rt ;
	elsif ramoe = '1' then
		ra <= ra + 1 ;
		rxvtflag <= '0' ;
	else
		rxvtflag <= '0' ;
	end if ;
end if ;
end process ;

-- register +ve edge data coming in with lxclkin

rp0 : ram16x1d port map(d => lxdatain(0), we => high, wclk => lxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(0)) ;
rp1 : ram16x1d port map(d => lxdatain(1), we => high, wclk => lxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(1)) ;
rp2 : ram16x1d port map(d => lxdatain(2), we => high, wclk => lxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(2)) ;
rp3 : ram16x1d port map(d => lxdatain(3), we => high, wclk => lxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(3)) ;
rp4 : ram16x1d port map(d => lxdatain(4), we => high, wclk => lxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(4)) ;
rp5 : ram16x1d port map(d => lxdatain(5), we => high, wclk => lxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(5)) ;
rp6 : ram16x1d port map(d => lxdatain(6), we => high, wclk => lxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(6)) ;
rp7 : ram16x1d port map(d => lxdatain(7), we => high, wclk => lxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(7)) ;

-- register -ve edge data coming in with lxclkin

rn0 : ram16x1d port map(d => lxdatain(0), we => high, wclk => notlxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(8)) ;
rn1 : ram16x1d port map(d => lxdatain(1), we => high, wclk => notlxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(9)) ;
rn2 : ram16x1d port map(d => lxdatain(2), we => high, wclk => notlxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(10)) ;
rn3 : ram16x1d port map(d => lxdatain(3), we => high, wclk => notlxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(11)) ;
rn4 : ram16x1d port map(d => lxdatain(4), we => high, wclk => notlxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(12)) ;
rn5 : ram16x1d port map(d => lxdatain(5), we => high, wclk => notlxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(13)) ;
rn6 : ram16x1d port map(d => lxdatain(6), we => high, wclk => notlxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(14)) ;
rn7 : ram16x1d port map(d => lxdatain(7), we => high, wclk => notlxclkin, a0 => wa(0), a1 => wa(1),
		        a2 => wa(2), a3 => wa(3), dpra0 => ra(1), dpra1 => ra(2),
		        dpra2 => ra(3), dpra3 => ra(4), spo => open, dpo => lxdataint(15)) ;

process (clk, rst)		-- register received data from DP rams
begin
if rst = '1' then
	rxdata_int <= "00000000" ;
elsif clk'event and clk = '1' then
	if ra(0) = '1' then
		rxdata_int <= lxdataint(15 downto 8) ;
	else
		rxdata_int <= lxdataint(7 downto 0) ;
	end if ;
end if ;
end process ;

process (clk, rst)		-- count lxclkin high cycles
begin
if rst = '1' then
	lxcounth <= "0000" ;
	lxcounthflag <= '0' ;
elsif clk'event and clk = '1' then
	if lxclkintpp = '0' or lxclkintp = '0' then
		lxcounth <= "0000" ;
	else
		lxcounth <= lxcounth + 1 ;
	end if ;
	if lxcounth > "0010" then
		lxcounthflag <= '1' ;
	else
		lxcounthflag <= '0' ;
	end if ;
end if ;
end process ;

process (clk, rst)	-- huge state machine
begin
if rst = '1' then
	state <= 255 ;
	lxclkout_d <= '1' ;
	lxdir <= '0' ;
	lxdir_int <= '0' ;
	shift_tx <= '0' ;
	gcnt <= "0000" ;
	tx_ack_bad <= '0' ;
	rxtoen <= '0' ;
	enablewr <= '0' ;
	disablewr <= '0' ;
	mux <= '0' ;
	pcount <= "000000" ;
elsif clk'event and clk = '1' then
	lxdir <= lxdir_int ;
	gcnt <= gcnt + 1 ;
	case state is
		when 255 => lxdir_int <= '0' ; lxclkout_d <= not rxbp ; pcount <= "000000" ;
			    gcnt <= "0000" ; shift_tx <= '0' ; 	-- reset state
			    if lxclkintp = '0' and rxbp = '0' then 	  -- goto receiver stuff
			      state <= 100 ; enablewr <= '1' ;
			    elsif txreq = '1' then state <= 2 ;		-- goto transmitter stuff
			    else state <= 255 ;
			    end if ; 				-- else hang around here

		when 2 => if lxclkintp = '0' then
			    state <= 2 ; lxclkout_d <= '1' ;	-- tx requested, wait for lxclkin high
		          elsif gcnt < "1011" then
		            state <= 2 ; lxclkout_d <= '0' ;	-- hold lxclkout low for 6 cycles
		          else state <= 3 ; shift_tx <= '1' ;
		          end if ;

		when 3  => state <= 13 ; lxclkout_d <= '0' ; shift_tx <= '1' ;
			   lxdir_int <= '1' ; gcnt <= "0000" ;

		when 13 => if gcnt = "0111" then
			     state <= 13 ; tx_ack_bad <= lxclkintp ;
			     if psize = '0' then
			     	pcount <= pcount + 1 ;
			     end if ;
			     lxclkout_d <= not gcnt(0) ; shift_tx <= '1' ;
			   elsif gcnt < "1101" then
		  	     state <= 13 ; lxclkout_d <= not gcnt(0) ;
		  	     shift_tx <= '1' ;
			   elsif verein = '1' and gcnt = "1101" then
		  	     state <= 13 ; lxclkout_d <= not gcnt(0) ;
		  	     shift_tx <= '1' ;
			   elsif verein = '1' and gcnt = "1110" then
		  	     state <= 13 ; lxclkout_d <= not gcnt(0) ;
		  	     shift_tx <= '0' ;
			   elsif verein = '0' and gcnt = "1101" then
			     state <= 32 ; lxclkout_d <= not gcnt(0) ;
			     shift_tx <= '1' ;
			   else
			     state <= 32 ; lxclkout_d <= not gcnt(0) ;
			     shift_tx <= '0' ; mux <= '1' ;
			   end if ;

		when 32 => lxclkout_d <= '1' ; mux <= '0' ;
			   if lxclkintp = '0' then
		             state <= 34 ; shift_tx <= '0' ; 	-- wait for receiver ready
			   elsif txreq = '1' and psize = '1' then
			     state <= 33 ;  shift_tx <= '1' ;
			   elsif txreq = '1' and psize = '0' and pcount > "000000" then
			     state <= 33 ;  shift_tx <= '1' ;
		           else
			     state <= 33 ;  shift_tx <= '0' ;
			   end if ;

		when 33 => gcnt <= "0000" ; lxclkout_d <= '0' ;
		           if txreqd = '1' and psize = '1' then
		             state <= 13 ; shift_tx <= '1' ; 	-- transmit next word
		           elsif txreqd = '1' and psize = '0' and pcount > "000000" then
		             state <= 13 ; shift_tx <= '1' ; 	-- transmit next word
			   else
			     state <= 45 ; shift_tx <= '0' ;
			   end if ;

		when 34 => gcnt <= "0000" ; lxclkout_d <= '0' ;
		           if lxclkintp = '0' then state <= 34 ;
		           else state <= 3 ; shift_tx <= '1' ;
		           end if ;

		when 45 => lxclkout_d <= '1' ; state <= 46 ;
		           lxdir_int <= '0' ; gcnt <= "0000" ; 	-- no tx request, try to pass token

		when 46 => if gcnt < "1000" then state <= 46 ;
			   elsif gcnt = "1000" and txreq = '1' 	-- tx req arrived, service it
			   and (psize = '1' or (psize = '0' and pcount > "000000")) then
			    state <= 47 ; lxclkout_d <= '0' ; gcnt <= "0000" ;
		           elsif gcnt = "1100" then
		            state <= 255 ; enablewr <= '0' ;
		           elsif lxclkintp = '0' then		-- request from other end
		            state <= 100 ; enablewr <= '1' ;
		           else
		            state <= 46 ;
		           end if ;

		when 47 => if gcnt < "0011" then state <= 47 ;
			   elsif lxclkintp = '0' then state <= 47 ;
		           else state <= 3 ; lxclkout_d <= '0' ; shift_tx <= '1' ;		-- tx req, regret token
		           end if ;

		when 100 => gcnt <= "0000" ; pcount <= "000000" ;
			    if localenablewr_rt = '1' then state <= 101 ;
		               lxclkout_d <= '0' ; rxtoen <= '1' ;
			    else state <= 100 ;
			    end if ;

		when 101 => lxclkout_d <= '0' ;
			    if gcnt = "0010" then state <= 105 ; enablewr <= '0' ;
			    else state <= 101 ;
			    end if ;

		when 105 => if ackflag_rt = '1' and rxbp = '1' then
		              state <= 106 ; lxclkout_d <= '0' ;
			    elsif lxcounthflag = '1' then state <= 200 ; disablewr <= '1' ;
			    elsif localenablewr_rt = '1' then
--			      state <= 105 ; lxclkout_d <= not(rxbp or ackflag_rt) ;
			      state <= 105 ; lxclkout_d <= '1' ; --not ackflag_rt ;
			    else state <= 255 ;
			    end if ;

		when 106 => if lxcounthflag = '1' then state <= 200 ; disablewr <= '1' ;
			    elsif rxbp = '1' then state <= 106 ; lxclkout_d <= '0' ;
			    else state <= 105 ; lxclkout_d <= '1' ;
			    end if ;

		when 200 => gcnt <= "0000" ; disablewr <= '0' ; rxtoen <= '0' ;
			    if txreq = '0' and lxclkintp = '1' then
			    state <= 255 ; -- no txreq
			    elsif txreq = '0' and lxclkintp = '0' then
			    state <= 100 ; enablewr <= '1' ; 		-- then go and receive
			    else lxclkout_d <= '0' ; state <= 201 ;	-- potential token pass
			    end if ;

		when 201 => if lxclkintp = '0' then state <= 100 ;
			       lxclkout_d <= '1' ; enablewr <= '1' ; 	-- regret from other end
		            elsif gcnt < "1000" then state <= 201 ; 	-- wait
		            else state <= 3 ;  shift_tx <= '1' ;	-- OK, become transmitter
		        end if ;

		when others => state <= 255 ;
	end case ;
end if ;
end process ;
end arch_macro_virtex2_rev2 ;

