-- Disclaimer : These designs are provided "as is" with
--              no warranty whatsoever, and xilinx specifically
--              disclaims any implied warranties of merchantability,
--              fitness for a particular purpose, or against infringement.
--
-- 32 bit shifter_virtex2 for Virtex2 using SRLC16E elements
--
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

--pragma translate_off
library unisim ;
use unisim.vcomponents.all ;
--pragma translate_on

entity shifter_virtex2 is port (
	d 	: in std_logic;
	clk 	: in std_logic;
	ce 	: in std_logic;
	a0 	: in std_logic;
	a1 	: in std_logic;
	a2 	: in std_logic;
	a3 	: in std_logic;
	a4 	: in std_logic;
	rst 	: in std_logic; 
	q	: out std_logic); 

end shifter_virtex2;

architecture arch_shifter_virtex2 of shifter_virtex2 is

component srlc16e port (
	d 	: in std_logic;
	clk 	: in std_logic;
	ce 	: in std_logic;
	a0 	: in std_logic;
	a1 	: in std_logic;
	a2 	: in std_logic;
	a3 	: in std_logic;
	q 	: out std_logic; 
	q15 	: out std_logic); 
end component;

component fdc port (
	d 	: in std_logic;
	c 	: in std_logic;
	clr 	: in std_logic; 
	q	: out std_logic); 
end component;

component muxf5 port (
	i0    	: in std_logic;
	i1    	: in std_logic;
	s    	: in std_logic;
	o    	: out std_logic);
end component ;

signal high : std_logic ;
signal low : std_logic ;
signal qc : std_logic ;
signal ql : std_logic ;
signal qh : std_logic ;
signal qm : std_logic ;

begin

high <= '1' ;
low  <= '0' ;

txsrl0 : srlc16e port map(
	d 	=> d,
	clk 	=> clk,
	ce 	=> ce,
	a0 	=> a0,
	a1 	=> a1,
	a2 	=> a2,
	a3 	=> a3,
	q15 	=> qc,
	q 	=> ql );
	
txsrl1 : srlc16e port map(
	d 	=> qc,
	clk 	=> clk,
	ce 	=> ce,
	a0 	=> a0,
	a1 	=> a1,
	a2 	=> a2,
	a3 	=> a3,
	q15 	=> open,	
	q 	=> qh );
	
muxdata   : muxf5 
	port map(i0 => ql, i1 => qh, s => a4, o => qm ) ;
	
txdff : fdc port map (
	d 	=> qm,
	c 	=> clk,
	clr 	=> rst, 
	q	=> q) ; 

end arch_shifter_virtex2 ;

