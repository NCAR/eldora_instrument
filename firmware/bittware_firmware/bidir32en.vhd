library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use unisim.vcomponents.all;

entity bidir32en is
    Port ( Tristate : in std_logic_vector(31 downto 0);
           DIN : in std_logic_vector(31 downto 0);
           DOUT : out std_logic_vector(31 downto 0);
           DIO : inout std_logic_vector(31 downto 0));
end bidir32en;

architecture Behavioral of bidir32en is

-- INOUT_PORT : inout STD_LOGIC;
 
--**Insert the following between the
--  'architecture' and 'begin' keywords**
  
signal IN_SIG, OUT_SIG, T_ENABLE:  std_logic;
component IOBUF
      port (I, T: in std_logic; 
            O: out std_logic; 
            IO: inout std_logic);
end component; 

begin

--**Insert the following after the 'begin' keyword**
	IOBUFGen: for cnt in (DIN'Length - 1) downto 0 generate
		IOBUFInst: IOBUF port map (I => DIN(cnt), T => Tristate(cnt),O => DOUT(cnt), IO => DIO(cnt));
	end generate;
 

end Behavioral;
