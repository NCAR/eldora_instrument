/*
 *	$Id$
 *
 *	Module:	fltpt.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1997/11/12  19:36:03  eric
 * modified to use Graph Mode for xmit pulse.
 *
 * Revision 1.1  1993/09/20  17:35:40  reif
 * Initial revision
 *
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to 
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "math.h"
#include "pwrDef.h"
#include "pwrFunc.h"
#include "tp41vAdr.h"

void flt_pt(int mode)
{
    double exp;
    volatile unsigned char *xmit_data, *testp_data;
    if(xmit_isr_done==1)
      {
	  
/*************  CONVERT DATA TO FLOATING POINT AND DISPLAY  *************/
	  
	  xmit_data = xmit_array + STANDARD_BASE;
	  fore_xmit_pwr=(xmit_data[4]-'0');
	  if(xmit_data[5]=='.')
	    {
		fore_xmit_pwr+=((xmit_data[6]-'0')*0.1)+((xmit_data[7]-'0')*0.01)+((xmit_data[8]-'0')*0.001);
	    }
	  else if(xmit_data[6]=='.')
	    {
		fore_xmit_pwr*=10;
		fore_xmit_pwr+=(xmit_data[5]-'0')+((xmit_data[7]-'0')*0.1)+((xmit_data[8]-'0')*0.01);
	    }
	  else if(xmit_data[7]=='.')
	    {
		fore_xmit_pwr*=100;
		fore_xmit_pwr+=((xmit_data[5]-'0')*10)+(xmit_data[6]-'0')+((xmit_data[8]-'0')*0.1);
	    }
  	  if(xmit_data[10]=='-')
	    {
		exp=xmit_data[12]-'0';
		fore_xmit_pwr*=pow((double)10.0,-exp);
	    }
	  else
	    {
		exp=xmit_data[12]-'0';
		fore_xmit_pwr*=pow((double)10.0,exp);
	    }

	  if(mode == 0) /* Continuous Update */
	    {
	      aft_xmit_pwr=(xmit_data[18]-'0');
	      if(xmit_data[19]=='.')
		{
		  aft_xmit_pwr+=((xmit_data[20]-'0')*0.1)+((xmit_data[21]-'0')*0.01)+((xmit_data[22]-'0')*0.001);
		}
	      else if(xmit_data[20]=='.')
		{
		  aft_xmit_pwr*=10;
		  aft_xmit_pwr+=(xmit_data[19]-'0')+((xmit_data[21]-'0')*0.1)+((xmit_data[22]-'0')*0.01);
		}
	      else if(xmit_data[21]=='.')
		{
		  aft_xmit_pwr*=100;
		  aft_xmit_pwr+=((xmit_data[19]-'0')*10)+(xmit_data[20]-'0')+((xmit_data[22]-'0')*0.1);
		}
	      if(xmit_data[24]=='-')
		{
		  exp=(xmit_data[26]-'0');
		  aft_xmit_pwr*=pow((double)10.0,-exp);
		}
	      else
		{
		  exp=xmit_data[26]-'0';
		  aft_xmit_pwr*=pow((double)10.0,exp);
		}
	    }

	  if(mode == 1) /* Update Now */
	    {
	      aft_xmit_pwr=(xmit_data[19]-'0');
	      if(xmit_data[20]=='.')
		{
		  aft_xmit_pwr+=((xmit_data[21]-'0')*0.1)+((xmit_data[22]-'0')*0.01)+((xmit_data[23]-'0')*0.001);
		}
	      else if(xmit_data[21]=='.')
		{
		  aft_xmit_pwr*=10;
		  aft_xmit_pwr+=(xmit_data[20]-'0')+((xmit_data[22]-'0')*0.1)+((xmit_data[23]-'0')*0.01);
		}
	      else if(xmit_data[22]=='.')
		{
		  aft_xmit_pwr*=100;
		  aft_xmit_pwr+=((xmit_data[20]-'0')*10)+(xmit_data[21]-'0')+((xmit_data[23]-'0')*0.1);
		}
	      if(xmit_data[25]=='-')
		{
		  exp=(xmit_data[27]-'0');
		  aft_xmit_pwr*=pow((double)10.0,-exp);
		}
	      else
		{
		  exp=xmit_data[27]-'0';
		  aft_xmit_pwr*=pow((double)10.0,exp);
		}
	    }

      }
    if(testp_isr_done==1)
      {
/************  CONVERT DATA TO FLOATING POINT AND DISPLAY  *************/

	  testp_data = testp_array + STANDARD_BASE;
	  fore_testp_pwr=((testp_data[5]-'0')*100)+((testp_data[6]-'0')*10)+(testp_data[7]-'0')+((testp_data[9]-'0')*0.1)+((testp_data[10]-'0')*0.01);
	  if(testp_data[4]=='-')
	    fore_testp_pwr*=-1;
	  aft_testp_pwr=((testp_data[17]-'0')*100)+((testp_data[18]-'0')*10)+(testp_data[19]-'0')+((testp_data[21]-'0')*0.1)+((testp_data[22]-'0')*0.01);
	  if(testp_data[16]=='-')
	    aft_testp_pwr*=-1;
      } 
}







