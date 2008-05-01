#include "GeGlobal.hh"
#include "GeDraw.hh"
#include "tp41Lib.h"
#include <sysLib.h>

extern "C" {
    void tstem(int wait);
    void __do_global_ctors();
};

static DispCommand ge_command;

void tstem(int wait)
{
    sysIntEnable(1);            // Enable interrupts.
    sysIntEnable(2);
    sysIntEnable(3);

    dport((char *)0x40200000,(void *)0x10000000,4); // Set up dual
                                                    // ported memory.

    wcio(0,"a",0xc8);           // Set VME page & attach VME ext space.

// Set up cmd block.
    GeCommand = &ge_command;
    GeCommand->max0 = 60.0;
    GeCommand->max1 = 0.0;
    GeCommand->max2 = 0.0;
    GeCommand->min0 = -60.0;
    GeCommand->min1 = 0.0;
    GeCommand->min2 = 0.0;
    GeCommand->top = 30000.0;
    GeCommand->bottom = 0.0;
    
    GeCommand->numParams = 2;
    GeCommand->param0 = ParamNames::REFLECT;
    GeCommand->param1 = ParamNames::VELOCITY;
    GeCommand->param2 = ParamNames::NO_PARAM;
    GeCommand->radius = 511;
    GeCommand->colorTable[0] = 0xffffffff;
    GeCommand->cmd = FORWARD_RADIAL;
    GeCommand->size = 30000.0;
    GeCommand->top = 30000.0;
    GeCommand->bottom = 0.0;
    GeCommand->side = LEFT;

    __do_global_ctors();

    GraphicController Agc((void *)AGC_ADDR,AGC_WIDTH,AGC_HEIGHT,AGC_MEM_WIDTH,
                          AGC_VECTOR);

    Radial rad(&Agc);
    Dual dual(&Agc);
    Horiz horiz(&Agc);
    Vert vert(&Agc);
    
    Display *disp = (Display *)&rad;

    // Now create the needed shared objects.
    ColorConverter *conv = new ColorConverter();

    ParamNames namer;

    disp->setColorConverter(*conv);

    disp->setParmNames(namer);

    disp = (Display *)&dual;
    
    disp->setColorConverter(*conv);

    disp->setParmNames(namer);

    disp = (Display *)&horiz;
    
    disp->setColorConverter(*conv);

    disp->setParmNames(namer);

    disp = (Display *)&vert;
    
    disp->setColorConverter(*conv);

    disp->setParmNames(namer);


    for (;;)
      {
	  disp = (Display *)&rad;
          GeCommand->cmd = FORWARD_RADIAL;
	  disp->reset(Hdr,GeCommand);

	  cout << "Waiting..." << endl;
	  taskDelay(wait);
	  cout << "done waiting" << endl;
    
	  disp->undisplay();

	  disp = (Display *)&dual;
          GeCommand->cmd = FORWARD_DUAL;
	  disp->reset(Hdr,GeCommand);

	  cout << "Waiting..." << endl;
	  taskDelay(wait);
	  cout << "done waiting" << endl;
    
	  disp->undisplay();

//           disp = (Display *)&horiz;
//           GeCommand->cmd = FORWARD_HORIZ;
// 	  disp->reset(Hdr,GeCommand);

// 	  cout << "Waiting..." << endl;
// 	  taskDelay(wait);
// 	  cout << "done waiting" << endl;
    
// 	  disp->undisplay();

//           disp = (Display *)&vert;
//           GeCommand->cmd = FORWARD_VERT;
// 	  disp->reset(Hdr,GeCommand);

// 	  cout << "Waiting..." << endl;
// 	  taskDelay(wait);
// 	  cout << "done waiting" << endl;
    
// 	  disp->undisplay();
      }
}
