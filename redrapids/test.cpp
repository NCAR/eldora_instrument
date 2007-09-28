#include "RR314.h"
#include <iostream>
#include <iomanip>
#include <ios>

using namespace RedRapids;

int
main(int argc, char** argv) 
{

  // create an RR314 card
  RR314 rr314(1000,
	      100,
	      0,
	      0,
	      10,
	      50.0e6,
	      12,
	      std::string(""),
	      std::string(""),
	      std::string(""));
	      
  // start the processing
  rr314.start();

  while(1) {
    sleep(10);
    int bytes[16];
    std::cout << std::setw(8);
    std::cout << rr314.bytes() << " ";
    for (int c = 0; c < 16; c++) {
      std::cout << std::setw(8);
      std::cout << rr314.bytes(c) << " ";
    }
    std::cout << "\n";
  }
}

