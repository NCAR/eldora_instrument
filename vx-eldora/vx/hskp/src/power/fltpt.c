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
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to 
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <pwrDef.h>
#include <pwrFunc.h>

void flt_pt()
{
    double exp;

    if(xmit_isr_done==1)
      {
	  
/*************  CONVERT DATA TO FLOATING POINT AND DISPLAY  *************/
	  
	  fore_xmit_pwr=(xmit_array[4]-'0');
	  if(xmit_array[5]=='.')
	    {
		fore_xmit_pwr+=((xmit_array[6]-'0')*0.1)+((xmit_array[7]-'0')*0.01)+((xmit_array[8]-'0')*0.001);
	    }
	  else if(xmit_array[6]=='.')
	    {
		fore_xmit_pwr*=10;
		fore_xmit_pwr+=(xmit_array[5]-'0')+((xmit_array[7]-'0')*0.1)+((xmit_array[8]-'0')*0.01);
	    }
	  else if(xmit_array[7]=='.')
	    {
		fore_xmit_pwr*=100;
		fore_xmit_pwr+=((xmit_array[5]-'0')*10)+(xmit_array[6]-'0')+((xmit_array[8]-'0')*0.1);
	    }
  	  if(xmit_array[10]=='-')
	    {
		exp=xmit_array[12]-'0';
		fore_xmit_pwr*=pow((double)10.0,-exp);
	    }
	  else
	    {
		exp=xmit_array[12]-'0';
		fore_xmit_pwr*=pow((double)10.0,exp);
	    }
	  aft_xmit_pwr=(xmit_array[18]-'0');
	  if(xmit_array[19]=='.')
	    {
		aft_xmit_pwr+=((xmit_array[20]-'0')*0.1)+((xmit_array[21]-'0')*0.01)+((xmit_array[22]-'0')*0.001);
	    }
	  else if(xmit_array[20]=='.')
	    {
		aft_xmit_pwr*=10;
		aft_xmit_pwr+=(xmit_array[19]-'0')+((xmit_array[21]-'0')*0.1)+((xmit_array[22]-'0')*0.01);
	    }
	  else if(xmit_array[21]=='.')
	    {
		aft_xmit_pwr*=100;
		aft_xmit_pwr+=((xmit_array[19]-'0')*10)+(xmit_array[20]-'0')+((xmit_array[22]-'0')*0.1);
	    }
	  if(xmit_array[24]=='-')
	    {
		exp=(xmit_array[26]-'0');
		aft_xmit_pwr*=pow((double)10.0,-exp);
	    }
	  else
	    {
		exp=xmit_array[26]-'0';
		aft_xmit_pwr*=pow((double)10.0,exp);
	    }
      }
    if(testp_isr_done==1)
      {
/************  CONVERT DATA TO FLOATING POINT AND DISPLAY  *************/

	  fore_testp_pwr=((testp_array[5]-'0')*100)+((testp_array[6]-'0')*10)+(testp_array[7]-'0')+((testp_array[9]-'0')*0.1)+((testp_array[10]-'0')*0.01);
	  if(testp_array[4]=='-')
	    fore_testp_pwr*=-1;
	  aft_testp_pwr=((testp_array[17]-'0')*100)+((testp_array[18]-'0')*10)+(testp_array[19]-'0')+((testp_array[21]-'0')*0.1)+((testp_array[22]-'0')*0.01);
	  if(testp_array[16]=='-')
	    aft_testp_pwr*=-1;
      } 
}







