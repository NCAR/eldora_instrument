/*
 *	$Id$
 *
 *	Module:	init_iru	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/19  15:40:59  craig
 * Initial revision
 *
 * description: This module initializes the Inertial Reference Unit (IRU)
 *              interface and it pointers etc.  This interface is a ARINC429
 *              serial bus interface designed at NCAR.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern

/* Include fifty million vx-works .h files */

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"

/* include fifty million .h files to deal with the header formats */

#include "Volume.h"
#include "Waveform.h"
#include "RadarDesc.h"
#include "FieldRadar.h"
#include "CellSpacing.h"
#include "Parameter.h"
#include "NavDesc.h"
#include "InSitu.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "IndFreq.h"
#include "TimeSeries.h"
#include "NavInfo.h"
#include "Ins.h"
#include "MiniRIMS.h"
#include "Gps.h"
#include "InSituData.h"

#include "Header.h"
extern HeaderPtr inHeader;

/* include the .h files that are housekeeper code specific */

#include "hskpInt.h"
#include "hskpGbl.h"
#include "tp41vAdr.h"
#include "vmevmeAdr.h"
#include "vmevmeDef.h"
#include "vmevmeGbl.h"
#include "vmevmeFunc.h"
#include "iruDef.h"
#include "iruGbl.h"
#include "iruFunc.h"
#include "ecbMaster.h"

void init_iru(short interrupt)
{
char *char_pntr;

iru_rpntr = (struct iru_rcontrol *)(IRU_BASE + STANDARD_BASE + IRU_RXA1); 
iru_hi8282a = (short *)(IRU_BASE + STANDARD_BASE + IRU_HI8282A);

/* Set up interface to interrupt on either 300 words or the longitude label */

iru_rpntr->max_words = 300;
iru_rpntr->term_label = LONGITUDE_LAB;
*iru_hi8282a = 0x20;

/*  Setup for interrupts, (if requested) */

iru_rpntr->int_flag = 0;
if(interrupt)
  {
      iru_rpntr->int_flag = 1;
      char_pntr = (unsigned char *)(IRU_BASE + STANDARD_BASE + IRU_BIM
                   + BIMCR0);
      *char_pntr = (char)(ARINC_IRQ + 0x10);
      char_pntr = (unsigned char *)(IRU_BASE + STANDARD_BASE + IRU_BIM
                   + BIMVR0);
      *char_pntr = ARINC_VEC;

      /* Will have to attached iru_isr to the interrupt here before
         actually using the interrupt feature */
  }

}





