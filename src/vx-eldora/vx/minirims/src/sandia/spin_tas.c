/*+ DEC/CMS REPLACEMENT HISTORY, Element SPIN_TASK.C+*/
/*+ *4    18-NOV-1988 08:49:33 JRFELLE "removed diagnostic timing calls in preparation for flight"+*/
/*+ *3    12-JUL-1988 09:11:29 JRFELLE "included declaration of tnum and snum"+*/
/*+ *2    12-JUL-1988 08:56:11 JRFELLE "encorporated changes necessary for target frame update"+*/
/*+ *1    23-MAY-1988 07:48:04 JRFELLE "spinning controller slow loop"+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element SPIN_TASK.C+*/
#include "vrtxvisi.h"
#include "portdefs.h"
/*+**********************************************************************

NAME: spin_task - the spinning controller slow loop task interface

SYNOPSIS: void spin_task()

INPUTS: global variable spin_mode

PROCESSING: pends on a 1 Hz mailbox post from NAV_TASK and
            calls the ac_slow function.


OUTPUTS: None

RETURNS: None.

    Rick Fellerhoff    May 12, 1988

**********************************************************************+*/

long int SPIN_MAIL_BOX;
extern void ac_slow();
int slow_calls=0;

void spin_task()
{
  long int msg,err;

  gatarout(1,"SPIN_TASK is alive !!!\n",23);

  FOREVER
  {
    msg = sc_pend(&SPIN_MAIL_BOX,0L,&err);
    if(err != RET_OK)
    {
      gatarout(1,"spin task pend error\n",21);
      continue;
    }
    slow_calls++;
    ac_slow();

  }   /* END OF FOREVER */
}




