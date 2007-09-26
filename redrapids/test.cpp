#include "RR314.h"
#include <iostream>

int
main(int argc, char** argv) 
{

  RR314 rr314(600,
	      100,
	      0,
	      100,
	      10,
	      50.0e6,
	      12,
	      std::string(""),
	      std::string(""),
	      std::string(""));
	      
  while(1) {
    std::cout << "sleep...\n";
  }
}

