/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 CDTask.hh
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        
 */
#ifndef INCCDTask_hh
#define INCCDTask_hh

#include "Task.hh"

extern "C" {
    void __do_global_ctors();
    void __do_global_dtors();
};

class CDTask : public Task
{
  public:
    CDTask(FUNCPTR entry, int *args, int argsize, int pri = 100,
	   int stacksize = 3000, int options = VX_FP_TASK | VX_STDIO,
	   int go = 1, char *name = "") : Task(entry,args,argsize,pri,
					       stacksize,options,go,name)
    { __do_global_ctors(); }

    ~CDTask() { __do_global_dtors(); }
};

#endif // INCCDTask_hh
