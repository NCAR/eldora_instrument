#include "RR314.h"
#include <iostream>
#include <iomanip>
#include <ios>

using namespace RedRapids;

int
main(int argc, char** argv) 
{

  // create an RR314 card
  try {
    RR314 rr314(0,     // device number 
		1000,  // gates
		100,   // samples (nci)
		0,     // dual prt (0 or 1)
		0,     // starting IQ gate
		10,    // number of IQ gates
		12,    // decimation factor for the gaussian filter
		std::string(""), // path to xsvf bitstream file
		std::string(""), // path to kaiser filter coeeficients file
		std::string("")  // path to gaussian filter coeeficients file
		);
	      
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
  catch (std::string e) {
    std::cout << e << std::endl;
    exit(1);
  }

}

