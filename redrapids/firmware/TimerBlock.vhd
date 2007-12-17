--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    14:50:26 06/21/06
-- Design Name:    
-- Module Name:    TimerBlock - Behavioral
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
library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
-- synopsys translate_off
library UNISIM;
use UNISIM.Vcomponents.ALL;
-- synopsys translate_on

entity TimerBlock is
   port ( AddrIn      : in    std_logic_vector (2 downto 0); 
          AddrTrigger : in    std_logic; 
          BusClk      : in    std_logic; 
          CS          : in    std_logic; 
          Enable      : in    std_logic; 
          GPSTrigger  : in    std_logic; 
          Reset       : in    std_logic; 
          TimerClk    : in    std_logic; 
          WR          : in    std_logic;
          --Timertest   : out   std_logic; 
          PulseOut    : out   std_logic; 
          DataInOut   : inout std_logic_vector (31 downto 0));
end TimerBlock;

architecture BEHAVIORAL of TimerBlock is
   attribute INIT       : string ;
   attribute BOX_TYPE   : string ;
   signal Cntrl        : std_logic_vector (15 downto 0);
   signal Delay        : std_logic_vector (15 downto 0);
   signal Mult         : std_logic_vector (15 downto 0);
   signal Period       : std_logic_vector (15 downto 0);
   signal TimerClkOut  : std_logic;
   signal TimerEnable  : std_logic;
   signal TimerTrigger : std_logic;
   signal Width        : std_logic_vector (15 downto 0);
   signal XLXN_1       : std_logic;
   component TimerDecoder
      port ( Enable    : in    std_logic; 
             AddrIn    : in    std_logic_vector (2 downto 0); 
             Clk       : in    std_logic; 
             DataInOut : inout std_logic_vector (31 downto 0); 
             Cntrl     : out   std_logic_vector (15 downto 0); 
             Delay     : out   std_logic_vector (15 downto 0); 
             Width     : out   std_logic_vector (15 downto 0); 
             Period    : out   std_logic_vector (15 downto 0); 
             Mult      : out   std_logic_vector (15 downto 0); 
             WR        : in    std_logic; 
             Reset     : in    std_logic);
   end component;
   
   component timermodule
      port ( Clk      : in    std_logic; 
             Enable   : in    std_logic; 
             Trigger  : in    std_logic; 
             Rst      : in    std_logic;
             Cntrl    : in    std_logic_vector (15 downto 0); 
             PulseOut : out   std_logic;
             --Timertest: out   std_logic; 
             Delay    : in    std_logic_vector (15 downto 0); 
             Width    : in    std_logic_vector (15 downto 0); 
             Period   : in    std_logic_vector (15 downto 0); 
             Mult     : in    std_logic_vector (15 downto 0));
   end component;
   
   component clockdivider
      port ( Clk    : in    std_logic; 
             Sel    : in    std_logic_vector (1 downto 0); 
             ClkOut : out   std_logic);
   end component;
   
   component FD
      -- synopsys translate_off
      generic( INIT : bit :=  '0');
      -- synopsys translate_on
      port ( C : in    std_logic; 
             D : in    std_logic; 
             Q : out   std_logic);
   end component;
   attribute INIT of FD : component is "0";
   attribute BOX_TYPE of FD : component is "BLACK_BOX";
   
   component OR2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of OR2 : component is "BLACK_BOX";
   
begin
   XLXI_1 : TimerDecoder
      port map (AddrIn(2 downto 0)=>AddrIn(2 downto 0),
                Clk=>BusClk,
                Enable=>CS,
                Reset=>Reset,
                WR=>WR,
                Cntrl(15 downto 0)=>Cntrl(15 downto 0),
                Delay(15 downto 0)=>Delay(15 downto 0),
                Mult(15 downto 0)=>Mult(15 downto 0),
                Period(15 downto 0)=>Period(15 downto 0),
                Width(15 downto 0)=>Width(15 downto 0),
                DataInOut(31 downto 0)=>DataInOut(31 downto 0));
   
   XLXI_2 : timermodule
      port map (Clk=>TimerClkOut,
                Rst=>Reset,
                Cntrl(15 downto 0)=>Cntrl(15 downto 0),
                Delay(15 downto 0)=>Delay(15 downto 0),
                Enable=>TimerEnable,
                Mult(15 downto 0)=>Mult(15 downto 0),
                Period(15 downto 0)=>Period(15 downto 0),
                Trigger=>TimerTrigger,
                Width(15 downto 0)=>Width(15 downto 0),
                --Timertest=>Timertest,
                PulseOut=>PulseOut);
   
   XLXI_3 : clockdivider
      port map (Clk=>TimerClk,
                Sel(1 downto 0)=>Cntrl(4 downto 3),
                ClkOut=>TimerClkOut);
   
   XLXI_4 : FD
      port map (C=>TimerClkOut,
                D=>Enable,
                Q=>TimerEnable);
   
   XLXI_5 : FD
      port map (C=>TimerClkOut,
                D=>XLXN_1,
                Q=>TimerTrigger);
   
   XLXI_6 : OR2
      port map (I0=>AddrTrigger,
                I1=>GPSTrigger,
                O=>XLXN_1);
   
end BEHAVIORAL;


