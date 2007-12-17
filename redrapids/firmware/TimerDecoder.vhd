--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    14:38:32 06/21/06
-- Design Name:    
-- Module Name:    TimerDecoder - Behavioral
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

entity D3_8E_MXILINX_TimerDecoder is
   port ( A0 : in    std_logic; 
          A1 : in    std_logic; 
          A2 : in    std_logic; 
          E  : in    std_logic; 
          D0 : out   std_logic; 
          D1 : out   std_logic; 
          D2 : out   std_logic; 
          D3 : out   std_logic; 
          D4 : out   std_logic; 
          D5 : out   std_logic; 
          D6 : out   std_logic; 
          D7 : out   std_logic);
end D3_8E_MXILINX_TimerDecoder;

architecture BEHAVIORAL of D3_8E_MXILINX_TimerDecoder is
   attribute BOX_TYPE   : string ;
   component AND4
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND4 : component is "BLACK_BOX";
   
   component AND4B1
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND4B1 : component is "BLACK_BOX";
   
   component AND4B2
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND4B2 : component is "BLACK_BOX";
   
   component AND4B3
      port ( I0 : in    std_logic; 
             I1 : in    std_logic; 
             I2 : in    std_logic; 
             I3 : in    std_logic; 
             O  : out   std_logic);
   end component;
   attribute BOX_TYPE of AND4B3 : component is "BLACK_BOX";
   
begin
   I_36_30 : AND4
      port map (I0=>A2,
                I1=>A1,
                I2=>A0,
                I3=>E,
                O=>D7);
   
   I_36_31 : AND4B1
      port map (I0=>A0,
                I1=>A2,
                I2=>A1,
                I3=>E,
                O=>D6);
   
   I_36_32 : AND4B1
      port map (I0=>A1,
                I1=>A2,
                I2=>A0,
                I3=>E,
                O=>D5);
   
   I_36_33 : AND4B2
      port map (I0=>A1,
                I1=>A0,
                I2=>A2,
                I3=>E,
                O=>D4);
   
   I_36_34 : AND4B1
      port map (I0=>A2,
                I1=>A0,
                I2=>A1,
                I3=>E,
                O=>D3);
   
   I_36_35 : AND4B2
      port map (I0=>A2,
                I1=>A0,
                I2=>A1,
                I3=>E,
                O=>D2);
   
   I_36_36 : AND4B2
      port map (I0=>A2,
                I1=>A1,
                I2=>A0,
                I3=>E,
                O=>D1);
   
   I_36_37 : AND4B3
      port map (I0=>A2,
                I1=>A1,
                I2=>A0,
                I3=>E,
                O=>D0);
   
end BEHAVIORAL;



library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
-- synopsys translate_off
library UNISIM;
use UNISIM.Vcomponents.ALL;
-- synopsys translate_on

entity TimerDecoder is
   port ( AddrIn    : in    std_logic_vector (2 downto 0); 
          Clk       : in    std_logic; 
          Enable    : in    std_logic; 
          Reset     : in    std_logic; 
          WR        : in    std_logic; 
          Cntrl     : out   std_logic_vector (15 downto 0); 
          Delay     : out   std_logic_vector (15 downto 0); 
          Mult      : out   std_logic_vector (15 downto 0); 
          Period    : out   std_logic_vector (15 downto 0); 
          Width     : out   std_logic_vector (15 downto 0); 
          DataInOut : inout std_logic_vector (31 downto 0));
end TimerDecoder;

architecture BEHAVIORAL of TimerDecoder is
   attribute HU_SET     : string ;
   signal En0       : std_logic;
   signal En1       : std_logic;
   signal En2       : std_logic;
   signal En3       : std_logic;
   signal En4       : std_logic;
   signal En5       : std_logic;
   signal En6       : std_logic;
   signal En7       : std_logic;
   component register_16bit
      port ( Clk     : in    std_logic; 
             DataIn  : inout std_logic_vector (15 downto 0); 
             DataOut : out   std_logic_vector (15 downto 0); 
             Reset   : in    std_logic; 
             Enable  : in    std_logic; 
             WR      : in    std_logic);
   end component;
   
   component D3_8E_MXILINX_TimerDecoder
      port ( A0 : in    std_logic; 
             A1 : in    std_logic; 
             A2 : in    std_logic; 
             E  : in    std_logic; 
             D0 : out   std_logic; 
             D1 : out   std_logic; 
             D2 : out   std_logic; 
             D3 : out   std_logic; 
             D4 : out   std_logic; 
             D5 : out   std_logic; 
             D6 : out   std_logic; 
             D7 : out   std_logic);
   end component;
   
   attribute HU_SET of XLXI_23 : label is "XLXI_23_0";
begin
   XLXI_1 : register_16bit
      port map (Clk=>Clk,
                Enable=>En0,
                Reset=>Reset,
                WR=>WR,
                DataOut(15 downto 0)=>Cntrl(15 downto 0),
                DataIn(15 downto 0)=>DataInOut(15 downto 0));
   
   XLXI_2 : register_16bit
      port map (Clk=>Clk,
                Enable=>En1,
                Reset=>Reset,
                WR=>WR,
                DataOut(15 downto 0)=>Delay(15 downto 0),
                DataIn(15 downto 0)=>DataInOut(15 downto 0));
   
   XLXI_9 : register_16bit
      port map (Clk=>Clk,
                Enable=>En2,
                Reset=>Reset,
                WR=>WR,
                DataOut(15 downto 0)=>Width(15 downto 0),
                DataIn(15 downto 0)=>DataInOut(15 downto 0));
   
   XLXI_10 : register_16bit
      port map (Clk=>Clk,
                Enable=>En3,
                Reset=>Reset,
                WR=>WR,
                DataOut(15 downto 0)=>Period(15 downto 0),
                DataIn(15 downto 0)=>DataInOut(15 downto 0));
   
   XLXI_19 : register_16bit
      port map (Clk=>Clk,
                Enable=>En4,
                Reset=>Reset,
                WR=>WR,
                DataOut(15 downto 0)=>Mult(15 downto 0),
                DataIn(15 downto 0)=>DataInOut(15 downto 0));
   
   XLXI_20 : register_16bit
      port map (Clk=>Clk,
                Enable=>En5,
                Reset=>Reset,
                WR=>WR,
                DataOut=>open,
                DataIn(15 downto 0)=>DataInOut(15 downto 0));
   
   XLXI_21 : register_16bit
      port map (Clk=>Clk,
                Enable=>En6,
                Reset=>Reset,
                WR=>WR,
                DataOut=>open,
                DataIn(15 downto 0)=>DataInOut(15 downto 0));
   
   XLXI_22 : register_16bit
      port map (Clk=>Clk,
                Enable=>En7,
                Reset=>Reset,
                WR=>WR,
                DataOut=>open,
                DataIn(15 downto 0)=>DataInOut(15 downto 0));
   
   XLXI_23 : D3_8E_MXILINX_TimerDecoder
      port map (A0=>AddrIn(0),
                A1=>AddrIn(1),
                A2=>AddrIn(2),
                E=>Enable,
                D0=>En0,
                D1=>En1,
                D2=>En2,
                D3=>En3,
                D4=>En4,
                D5=>En5,
                D6=>En6,
                D7=>En7);
   
end BEHAVIORAL;


