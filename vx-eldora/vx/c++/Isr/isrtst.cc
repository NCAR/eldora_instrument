#include "Isr.hh"
#include <iostream.h>

class isr : private Isr {
  public:
    isr(int vector);

  private:
    void myIsr();
};

isr::isr(int vector) : Isr(vector,1,(VOIDFUNCPTR)&isr::myIsr)
{
    for (;;)
      {
	  IsrWait();

	  cout << "Caught an interrupt" << endl;
      }
}

void isr::myIsr()
{
    semGive(sem);
}
