--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    17:16:52 06/21/06
-- Design Name:    
-- Module Name:    MultiTimer - Behavioral
-- Project Name:   
-- Target Device:  
-- Tool versions:  
-- Description:
--
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
--------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity MultiTimer is
    Generic (NTIMERS : integer := 2);
    Port ( BUSCLK       : in  std_logic;
           RST          : in  std_logic;
           ADDRWR       : in  std_logic;
           ADDRIN       : in  std_logic_vector(31 downto 0);
           DATAWR       : in  std_logic;
			  TIMERCLK     : in  std_logic;
           ONEPPS       : in  std_logic;         -- not needed for now??
           DATAIN       : in  std_logic_vector(31 downto 0);
           DATAOUT      : inout std_logic_vector(31 downto 0);
           PULSEOUT     : out std_logic_vector(NTIMERS-1 downto 0);
           --Timertest    : out std_logic_vector(NTIMERS-1 downto 0);
           ADDROUT      : out std_logic_vector(31 downto 0)
			  
           );
end MultiTimer;

architecture Behavioral of MultiTimer is

component TimerInterface is
    generic(NTIMERS : integer);
    Port ( Clk          : in    std_logic;
           Reset        : in    std_logic;
           AddrIn       : in    std_logic_vector(31 downto 0);
           DataIn       : in    std_logic_vector(31 downto 0);
           TimerAddrOut : out   std_logic_vector(2 downto 0);
           DataInOut    : inout std_logic_vector(31 downto 0);
			  AddrOut      : out   std_logic_vector(31 downto 0);
           WR           : out   std_logic;
			  GPSEnable    : out   std_logic;
			  GPSReset     : out   std_logic;
           TimerSelect  : out   std_logic_vector(NTIMERS-1 downto 0);
			  AddrTrigger  : out   std_logic;
			  AddrWr       : in    std_logic;
			  DataWr       : in    std_logic);
end component TimerInterface;

component TimerBlock is
   port ( AddrIn        : in    std_logic_vector (2 downto 0); 
          AddrTrigger   : in    std_logic; 
          BusClk        : in    std_logic; 
          CS            : in    std_logic; 
          Enable        : in    std_logic; 
          GPSTrigger    : in    std_logic; 
          Reset         : in    std_logic; 
          TimerClk      : in    std_logic; 
          WR            : in    std_logic;
          --Timertest     : out   std_logic; 
          PulseOut      : out   std_logic; 
          DataInOut     : inout std_logic_vector (31 downto 0));
end component TimerBlock;

signal timerAddr      : std_logic_vector( 2 downto 0);
--signal timerDataInOut : std_logic_vector(31 downto 0);
signal timerWr        : std_logic;
signal timerSelect    : std_logic_vector(NTIMERS-1 downto 0);
signal addrTrigger    : std_logic;
signal globalEnable   : std_logic;
signal gpsTrigger     : std_logic;


begin
gpsTrigger <= '0';
globalEnable <= '1';  -- SUGANTH


TIMER_INTERFACE: TimerInterface
   generic map (NTIMERS => NTIMERS)
   port map (
      Clk          => BUSCLK,
      Reset        => RST ,
      AddrIn       => ADDRIN,
      DataIn       => DATAIN,
      TimerAddrOut => timerAddr,
      DataInOut    => DATAOUT,
		AddrOut      => ADDROUT,
      Wr           => timerWr,
		GPSEnable    => open,
		GPSReset     => open,
      TimerSelect  => timerSelect,
		AddrTrigger  => addrTrigger,
		AddrWr       => ADDRWR,
		DataWr       => DATAWR
   );

GEN_TIMERS: for X in 0 to NTIMERS-1 GENERATE
   TIMER_BLOCK_X: TimerBlock 
      port map (
         AddrIn       => timerAddr, 
         AddrTrigger  => addrTrigger, 
         BusClk       => BUSCLK, 
         CS           => timerSelect(X), 
         Enable       => globalEnable, 
         GPSTrigger   => Gpstrigger, 
         Reset        => RST,  
         TimerClk     => TIMERCLK, 
         WR           => timerWr, 
         PulseOut     => PULSEOUT(X),
         DataInOut    => DATAOUT
         --Timertest    => Timertest(X)
      );
    END GENERATE GEN_TIMERS;

end Behavioral;
