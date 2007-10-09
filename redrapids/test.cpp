#include "RR314.h"
#include <iostream>
#include <iomanip>
#include <ios>

using namespace RedRapids;

// a task which just consumes the data from the
// rr314 as it becomes available
void *
dataTask(void* threadArg) {
  RR314* pRR314 = (RR314*) threadArg;
  
  while (1) {
    // loop while waiting for new buffers
    int* pBuf = pRR314->nextBuffer();
    pRR314->returnBuffer(pBuf);
  }
}

// The main routine creates the card, starts it, and then just
// monitors the card activity.
int
main(int argc, char** argv) 
{

  bool simulate = false;
  if (argc > 1) {
    if (!strcmp(argv[1], "-s")) {
	  simulate = true;
	}
  }
  // create an RR314 card
  try {
    RR314 rr314(0,     // device number 
		1000,  // gates
		10,    // samples (nci)
		0,     // dual prt (0 or 1)
		0,     // starting IQ gate
		10,    // number of IQ gates
		12,    // decimation factor for the gaussian filter
		std::string(""), // path to xsvf bitstream file
		std::string(""), // path to kaiser filter coeeficients file
		std::string(""), // path to gaussian filter coeeficients file
	        simulate  // simulation mode?
		);
	      
    // create the data reading thread
    pthread_t dataThread;
    pthread_create(&dataThread, NULL, dataTask, (void*) &rr314);

    // start the processing
    rr314.start();

    // peridically display the card activity.
    while(1) {
      std::vector<unsigned long> bytes = rr314.bytes();

      std::cout << std::setw(4);
      std::cout << rr314.numFreeBuffers() << " ";
      unsigned long sum = 0;
      for (int c = 0; c < bytes.size(); c++) {
	std::cout << std::setw(6);
	std::cout << bytes[c] << " ";
	sum += bytes[c];
      }
      std::cout << sum;
      std::cout << "\n";
      
      sleep(10);
    }
  }
  catch (std::string e) {
    std::cout << e << std::endl;
    exit(1);
  }

}

