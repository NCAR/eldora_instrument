/****************************************************************
* navtask.c							*
*****************************************************************

Description:  This is the main routine which is used to control the
	      strapdown navigation software.  It is analagous to
	      the navtask routine used in Thorny Merit.

inputs:	      variables stored in nav data structure put there by proc e.
outputs:      variables stored in nav data structure.

notes:	      These navigation calculation routines are intended to execute
	      in processor f.  They are executed after the i/o processor
	      has received a rims interrupt and read the sensor data.
	      When the nav calculations are completed, the task is suspended
	      and waits in a qpend state.

	      This routine is called at the rate irate as defined in the
	      routine initial.c.   Rtime increments are 1/irate sec.

	      This routine activates the telemetry task in the i/o processor
              at a rate of 16 Hz.


*****************************************************************/

#include "navvars.h"
#include "rims.h"
#include "vrtxvisi.h"
#include "align_v.h"
#include "diag.h"		/* !!! For testing only !!!	*/
#include "fltseqda.h"

#define RIMS_INTRQ	0	/* rims interrupt read queue */
#define RIMSTO		0	/* rims qpend timeout value */

int incmd;
long int max_loop_time, end_nav_loop, nav_loop_time, modetimer, tm_counter;
long int xg_cnts, yg_cnts, zg_cnts, xrg_cnts, xa_cnts, ya_cnts, za_cnts;
long int xg_pcnts, xg_ncnts, yg_pcnts, yg_ncnts;
long int zg_pcnts, zg_ncnts, xrg_pcnts, xrg_ncnts;
short int rims_id, soft_reset, timertest;
long int countdown = -1;
long int ready;
double dph_x_tot,dph_y_tot,dph_z_tot;

char not_done,pend_err,rqpost_err,rims_err,fr1ptr;
extern unsigned vrtx_errf;

main()
{
  char *sc_qpend(),*sc_qaccept();          /* vrtx linkage */

  extern crc_task();
  extern kill_gatar(),gold_tm();
  extern char res_flagf;
  extern void ac_soft(),init_eci(),hard_init(),soft_init(),
              spin_task(),s_align(),ac_hard();

  register struct nav_vars *n;	/* pointer to the nav data structure	*/
  struct plat_control *rc,*tc;	/* pointer to control word in memory	*/
  struct indiscrete *pindis;	/* pointer to input discrete word	*/

  double *matptr;		/* DCM pointer used for calling ortho	*/
  char *mesg;			/* VRTX message word			*/
  long int err;			/* VRTX error status word		*/

  long *ptimer;
  struct diag_vars *d;

  short int modein,incmd;

  extern short int rims_id,soft_reset,bitest,timertest;
  extern long int max_loop_time, end_nav_loop, nav_loop_time, tm_counter;
  extern char fr1ptr;		/* values used in 30 sec accum test	*/
  extern long SPIN_MAIL_BOX;
  extern short int euler_flag;
  struct rims *r;

/*	    <<<<---------- INITIALIZATION ------------->>>>		*/


  n = (struct nav_vars *)NAV_start;	/* initialize structure pointers */

  r = (struct rims *)RIMS_START;

  d = (struct diag_vars *)DIAG_start; 	/* diagnostic data structure address */

  rc = (struct plat_control *)&n->rimscmd;
  tc = (struct plat_control *)&n->rmstat;

  pindis = (struct indiscrete *)&n->indis;

  ptimer = (long *)0x000e0000L;		/* pointer to hardware timer */

  tm_counter = 1;

  fr1ptr = 0;		
  not_done=(char)0;
  pend_err=(char)0;
  rqpost_err=(char)0;
  rims_err=(char)0;
  rims_id = RIMS_ID;

/* ------------------ start up indication message --------------------- */

  gatarout(1,"The navigator is alive!\n",24);

/* ------------------- start the crc task -----------------------------*/
  sc_tcreate(crc_task,20,200,&err);
  if(err != RET_OK)
    gatarout(1,"CRC_TASK create error\n",22);

/* ------------------- start the spin_slow task -----------------------------*/
  sc_tcreate(spin_task,40,100,&err);
  if(err != RET_OK)
    gatarout(1,"SPIN_TASK create error\n",23);

/* ------------------- start the s_align task --------------------------*/
  sc_tcreate(s_align,60,100,&err);
  if(err != RET_OK)
    gatarout(1,"S_ALIGN_TASK create error\n",26);

/* -------------- navigator variable initialization --------------------*/

/* check if this is a recovery attempt before initialization */
  if(res_flagf)
    goto recover;

  hard_init();
  ac_hard();

  n->launch = 0;	/* set launch ready flag to not ready state */
  max_loop_time=0L;	/* max navg execution time */

/* --------------- MINI RIMS hardware initialization ------------------ */

  mesg = sc_qpend(RIMS_INTRQ,RIMSTO,&err);	/* wait for 1st interrupt */
						/* before talking to rims */

  modein = pindis->ndis;	/* read input mode direct from interface */
  n->rimsstr = 0x00;		/* reset startup status code		*/
  if (modein == 1)		/* test for correct statrup mode	*/
  {
    n->mode = modein;
    n->rimsstr = 0x02;
  }

  rc->data_sel = XY_GYRO_TMP;	/* set temp to gyro in insure start	*/
  rc->rims_oper = RUN_INERT;	/* start rims - set in run mode		*/

/* test if the rims system is in the inertial mode yet, if not wait */

  gatarout(1,"waiting for gyro spinup\n",24);
  while (n->rimsup != 0)	/* test if rims in inertial mode yet	*/
  {
    mesg = sc_qpend(RIMS_INTRQ,RIMSTO,&err);

    while (err != RET_OK)	/* verify that interrupts are ok	*/
    {
      gatarout(1,"qpend error on rims_intrq\n",26);
      mesg = sc_qpend(RIMS_INTRQ,RIMSTO,&err);
    }

    incmd = tc->rims_oper;	/* test if rims in run mode yet		*/

    if (incmd == RUN_INERT)   /* if RUN_INERT,keep going; else stay in loop */
    {
      n->rimsup = 0;
      gatarout(1,"ready\n",6);
    }
  }				/* software loop operation.		 */

  kill_gatar();			/* disable further commnuications */

  rc->rims_cont = CAGE_0;		/* cage on start to 0 deg	*/
  rc->rims_oper = RUN_CAGE_SLEW;		/* set up rims for operation	*/

  rmrdcalc();

/* 	<<<< -------- Normal Operation Navigator loop --------- >>>>	*/

/* clear input queue on startup */
  do
  {	
    mesg = sc_qaccept(RIMS_INTRQ,&err);
  }
  while(err == RET_OK);

  while (n->rimsup == 0)
  {

    /* ---- EXECUTION ENTRY POINT ----- */
    /* ---- Recovery Entry Point  ----- */

recover:
    mesg = sc_qaccept(RIMS_INTRQ,&err);
    if(err == RET_OK)		/* Test if interrupt occurred before */
    {				/* end of previous loop.	     */
      not_done++;		/* try and catch up if necessay      */
    }
    else
    {
      mesg = sc_qpend(RIMS_INTRQ,RIMSTO,&err);/* wait for rims interrupt */
      if (err != RET_OK)			/* print if error occurs   */
      {
        pend_err++;
      }
    }
    modein = pindis->ndis;	/* read input mode direct from interface */

    if((bitest == 1) && (timertest == 1))
    {
      d->toddst = *ptimer;	/* get timer value at start of loop	*/
      d->tevst = d->toddst;
    }

    rmrdcalc();

    /* Test if initial cage detected & if rims operating properly */

    if (n->rimsstr == 0x04)
    {
      if (!(tc->rims_oper && 0x0040))
        rims_err++;
    }
    else
      if (tc->rims_cont == CAGE_180)
        n->rimsstr = 0x04;		/* initial cage detected */


/* test time if even or odd and execute pos or att routines */

    if (n->rtime %2 ==0)
    {                           /* position branch calculations */
      n->ixrgp1 = n->ixrgp;       /* save pos gyro counts for att calc */
      n->ixgp1 = n->ixgp;
      n->iygp1 = n->iygp;
      n->izgp1 = n->izgp;

      n->ixrgn1 = n->ixrgn;       /* save neg gyro counts for att calc */
      n->ixgn1 = n->ixgn;
      n->iygn1 = n->iygn;
      n->izgn1 = n->izgn;

      n->ixan2 = n->ixan;     /* save accel counts for accumulation */
      n->iyan2 = n->iyan;	/* in gcomp  */
      n->izan2 = n->izan;

      acomp();
      gravity();
      switch(n->mode) {
        default:
        case 0:                 /* nav position */
	/* post to spin_slow task at 1 Hz */
	  if(n->rtime % 128 == 0)
          {
            sc_post(&SPIN_MAIL_BOX,100L,&err);
	    if(err != RET_OK)
	      vrtx_errf++;
	  }
          nealt();
	  gravity();
	  ecinav_pos();  /* ECI velocity, position, sf calculations */
	  fltseq();
          if (n->rtime % n->irate == 0)
          {
	    matptr = &n->a11;
	    ortho(matptr);
	  }
          else
            if (n->rtime % n->irate == 64)
            {
	      matptr = &n->t11;
	      ortho(matptr);
	    }
	  if (ready == 0)	        /* if on internal power - no mode */
	  {				/* changes - disable commo & brks */
	    modechg();
	    n->launch = 0;		/* set flag to not ready */
	    if(n->intpwron != 0)
	    {		/* enable breaks, keyboard commo, and nav display */
	      n->intpwron = 0x0;
	    }
	  }
	  else
          {
            if((modein == 2)&&(n->snum == LAUNCH_NOT_DETECTED))
            {
              if(modetimer <= 0)
                modechg();
              else
                modetimer--;
            }
            n->intpwron = 0xff;     /* disable breaks and umbilical commo */
            n->launch = 0xff;       /* set flag to ready */
          }
          break;
        case 1:                 /* init position */
          if (soft_reset)
          {
            soft_reset=0;
            soft_init();      /*  initialize parameters  */
            ac_soft();
            gravity();  /* force earth related values to defined values  */
            erthnav();  /* force rltpai to defined value,other will reset  */
            init_eci();
          }
          init_nav();
          break;
        case 2:                 /* hold position */
          if (n->launch)
          {
            modetimer = 0;
            n->launch = 0;
          }
          break;
        case 3:                 /* test position */
          tstrims();
          break;
	case 4:			/* Optical Align Mode*/
	  nealt();
	  break;
	case 5:			/* Course align mode */
	  break;
        case 6:                 /* Quick align mode */
          nealt();
          break;
        case 8:                 /* Align Position - Rslvr caged to -90 deg */
          nealt();
          break;
        case 9:                 /* Align Position - Rslvr caged to 0 deg */
          nealt();
          break;
      }	/* end of position calc loop switch */

      gravity();
      if (n->mode != 0)
	modechg();          /* check if mode changed if not in nav mode */

      if ((bitest == 1) && (timertest == 1))
      {
        d->tevend = *ptimer;
        d->index = n->rtime % 128;
        d->dtime[d->index] = d->tevend - d->tevst;

        if(d->dtime[d->index] > d->maxeven)     /* save max even time */
          d->maxeven = d->dtime[d->index];
      }
    }
    else
    {                           /* attitude branch calculations */

      gcomp();

      n->ixan1 = n->ixan;     /* save accel counts for pos calc */
      n->iyan1 = n->iyan;
      n->izan1 = n->izan;

      switch (n->mode) {
	default:
        case 0:                     /* nav attitude */
          dircos();
	  if(n->snum == (short int ) LAUNCH_NOT_DETECTED)
	  {    /* only do before launch */
	    teci_init();	    /* init T, platform->eci xform */
	  }
	  dcmpi();		/* propagate T matrix */
	  erthnav();
	  navrot();
	  ecinav_att();	/* ECI Frame attitude calculations */
          break;
        case 1:                     /* hold attitude */
          break;
        case 2:                     /* init attitude */
          break;
        case 3:                     /* test attitude */
          break;
	case 4:			    /* Optical Align Mode */
          dircos();
          erthnav();
          align();
          navrot();
          if (n->rtime % n->irate ==1)
	  {
	    matptr = &n->a11;
	    ortho(matptr);
	  }
          break;
	case 5:		/* Coarse Align Mode */
	  c_align();
          break;
        case 6:         /* Quick Align Mode */
          dircos();
          erthnav();
          align();
          navrot();
          if (n->rtime % n->irate ==1)
	  {
	    matptr = &n->a11;
	    ortho(matptr);
	  }
          break;
        case 8:		/* Align Position - Rslvr caged to -90 deg */
        case 9:		/*  "     "          "     "    "    0 deg */
          dircos();
          if (a_mode == COURSE)
            erthal();
          else
            erthnav();
          align();
          navrot();
          if (n->rtime % n->irate == 1)
	  {
	    matptr = &n->a11;
	    ortho(matptr);
	  }
          break;
      }	/* end of attitude calcs loop switch */

      if((n->snum==(short int) LAUNCH_NOT_DETECTED)&&(euler_flag==0))
                                 /* no Launch yet */
	euler_flag = 0;
      else			/* use Flight euler angles after launch */
	euler_flag = 1;

      euler();                /* Caluclate pre-launch or flight Euler */
                                /* Angles depending on launch detect flag */
      if((bitest == 1) && (timertest == 1))
      {
        d->toddend = *ptimer;
        d->index = n->rtime % 128;
        d->dtime[d->index] = d->toddend - d->toddst;

        if(d->dtime[d->index] > d->maxodd)         /* save max odd time */
          d->maxodd = d->dtime[d->index];
      }

      if (n->rtime % n->irate == 1)
	if ((n->mode != 8) && (n->mode != 9)
         && (n->mode != 4) && (n->mode != 6))
            n->nsec++;
    }  /* End of Attitude branch calculations */

    /* ---- Accumulate Counts at a 16 Hz rate for TM ---- */

    n->ixrg_16 += n->ixrgp - n->ixrgn;
    n->ixg_16 += n->ixgp - n->ixgn;
    n->iyg_16 += n->iygp - n->iygn;
    n->izg_16 += n->izgp - n->iygn;
    n->ixa_16 += n->ixan;
    n->iya_16 += n->iyan;
    n->iza_16 += n->izan;

    /* ---- VRTX INTERFACE for TELEMETRY ------------*/
    /* load_tm set at 16 Hz rate */

    if (tm_counter++ % 8 == 0)
    {
      tm_counter = 1;
      n->load_tm = 0;
      eris_tm();
      mesg = (char *)0x1L;		/* 'TELM' message to i/o proc	     */
      sc_rqpost(0x0e,mesg,&err);	/* post to I/O processor to start TM */
      if (err == ER_QFL)
        rqpost_err++;
      n->ixrg_16 = 0;
      n->ixg_16 = 0;
      n->iyg_16 = 0;
      n->izg_16 = 0;
      n->ixa_16 = 0;
      n->iya_16 = 0;
      n->iza_16 = 0;
    }

    n->rtime++;
    if((bitest == 1) && (timertest == 1))
    {
      end_nav_loop = *ptimer;
      nav_loop_time = end_nav_loop - d->toddst;
      if(nav_loop_time > max_loop_time)
        max_loop_time = nav_loop_time;
    }
  }
/* -------------------- End Of Navigator Loop -------------------------	*/

}
