/*
 *	$Id$
 *
 *	Module:	FSETCAL	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/11/09  16:57:03  eric
 * Initial revision
 *
 * Revision 1.1  1991/12/04  16:09:05  eric
 * Initial revision
 *
 *
 * description:
 *        
 * This module programs the ADC Card.
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/* Program to set the calibration function in the A/D CONVERTER MODULE */
#include "vxWorks.h"
#include "stdioLib.h"
#include "math.h"
#include "ELDRP7.h"

/* define constants used in this program */
/* #define FALSE  (0) */
/* #define TRUE   (1) */
 
/* define calibration curve storage arrays */
   int ad_val[50];  /* A/D converter values array */
   float flt_val[50]; /* Floating point values corresponding to ad_val */
/* Q memory H converter stored in 0 - 9 */
/* Q memory L converter stored in 10 - 19 */
/* I memory H converter stored in 20 - 29 */
/* I memory L converter stored in 30 - 39 */
/* Z memory stored in 40 - 49 */
   int threshold;
   short *daisychain,*modesel;
 
 
/***************************************/
/* start the A/D setup program here    */
/***************************************/
 
void fsetcal(frq,bd)
unsigned long frq, bd;
{
 
static   void cal_change();
static   void save_atod();
static   void prog_atod();
static   void retr_atod();
static   void atod_hexd();
unsigned long i3e2dsp();	
 
   long  ltemp,k;
   int temp;
   int good_val;
   short dcsave;
 

   frq = (frq << 24) + EXTD_BASE;          /* freq # * 10**6 + EXTD_BASE */
   bd = bd << 20;                         /* board # * 10**5 */
   bd += frq;                             /* full base addr of ADC */

   /*----------------------------------------------------------------*/
   for(k=0;k<41;k=k+10) {
      ad_val[k] = -2048;
      flt_val[k] = -2048.0;
      ad_val[k + 1] = 2047;
      flt_val[k+1] = 2047.0;
   }

   threshold = 2895;  /* 2895 = sqrt(2047^2 + 2047^2) longest possible vectr */
   daisychain = (short *) (bd + ATOD_DC);
   dcsave = 0;
   *daisychain = dcsave;
   modesel = (short *) (bd + ATOD_M);
 
menu: ;
   printf("Select your desired choice\n");
   printf("1) In-Phase, high dynamic range, A/D calibration\n");
   printf("2) In-Phase, low dynamic range, A/D  calibration\n");
   printf("3) Quaduarature, high dynamic range, A/D calibration\n");
   printf("4) Quadurature, low dynamic range, A/D calibration\n");
   printf("5) Z space A/D calibration\n");
   printf("6) Change the choice threshold: %d\n",threshold);
   printf("7) Change the Daisy Chain number: %d\n",dcsave);
   printf("8) Change the Mode Register \n");
   printf("9) Save the current A/D converter card setup in a file\n");
   printf("10) Program the A/D converter card with current setup (fp)\n");
   printf("11) Program the A/D converter card with current setup (int)\n");
   printf("12) Retrieve a setup from a disk file\n");
   printf("13) Hex dump A/D converter card memory\n");
   printf("14) End routine\n");
 
   scanf("%d",&temp);
 
   switch ( temp ) {
      case 1:                  /* inphase memory high dynamic range */
         cal_change(1,0);
         break;
 
      case 2:                 /* inphase memory low dynamic range */
         cal_change(1,1);
         break;
 
      case 3:                /* quadrature memory high dynamic range */
         cal_change(0,0);
         break;
 
      case 4:
         cal_change(0,1);    /* quadrature memory low dynamic range */
         break;
 
      case 5:
         cal_change(1,2);    /* Log reflectivity memory*/
         break;
 
      case 6:
         for(good_val = FALSE; good_val == FALSE;) {
            printf("Enter threshold vector length (0 <= thresh >= 2895)\n");
            scanf("%d",&temp);
            if((temp<0) || (temp>2895))
               printf("Illegal threshold entered please re-enter\n");
            else {
               threshold = temp;
               good_val = TRUE;
            }
         }
         break;
 
      case 7:
         for(good_val = FALSE; good_val == FALSE;) {
            printf("Enter number of gates to pass to each DSP before\n");
            printf("setting the 'pass the daisy chain' line true\n");
            scanf("%d",&temp);
            if((temp<0) || (temp>255))
              printf("Illegal value entered (0 <= V > 255) please re-enter\n");            else {
               *daisychain = (short) temp;
	       dcsave = (short) temp;
               good_val = TRUE;
            }
         }
         break;
 
      case 8:
         for(good_val = FALSE; good_val == FALSE;) {
            printf("Enter Mode Select Number...\n");
            printf("0= Output I, Q, and Z, choose every hit.\n");
            printf("1= Output I and Q only, choose every hit.)\n");
            printf("2= Output I, Q, and Z, use first choice.\n");
            printf("3= Output I and Q only, use first choice.)\n"); 
	    scanf("%d",&temp);
            if((temp<0) || (temp>3))
              printf("Illegal value entered (0 <= V < 3) please re-enter\n");
            else {
               *modesel = temp;
               good_val = TRUE;
            }
         }
         break;

      case 9:   /* save current setup in a file */
         save_atod();
         break;
 
      case 10:   /* program the A/D card with the current setup (fp)*/
         prog_atod(1,bd);
         break;
 
      case 11:   /* program the A/D card with the current setup (int)*/
         prog_atod(0,bd);
         break;
 
      case 12:  /* retrieve a setup from a disk file */
         retr_atod();
         break;
 
      case 13: /* Hex Dump the memory */
         atod_hexd(bd);
         break;
 
      case 14:
         return;
         break;
 
      default:
         printf("The input given is not implemented\n");
         break;
   }
   goto menu;
}
 
/***********************************************/
/*  subroutine to change calibration curves   */
/**********************************************/

static void cal_change(iorq,horl)
  int iorq,horl; /* iorq => i or q memory, horl => high or low converter */
{
   int k;
   int start,end,flg,good_val;
 
/* Display the current set of calibration curves */
 
   printf("     starting point                    ending point\n");
   printf(" a/d val   calibrated val          a/d val     calibrated val\n");
   start = iorq * 20 + horl * 10;
   end = start + 9;
   for(k = start,flg = FALSE;(k < end) && (flg == FALSE);k++) {
      printf("  %4d       %8.3f              %4d       %8.3f\n"
                 ,ad_val[k],flt_val[k],ad_val[k+1],flt_val[k+1]);
      if(ad_val[k+1] == 2047 ) flg = TRUE;
   }
 
/* see if user wishes to change these calibration curves */

   printf("Do you want to change this calibration ? (y = 0, n = 1)\n");
   scanf("%d",&k);
   if(k != 0) return;

/* allow the user to enter a new set of calibration curves */

   end = end + 1;
   for(k = start,flg = FALSE; k < end,flg == FALSE;k++) {
      if((k > start) && (k < end)) {
         for(good_val = FALSE; good_val == FALSE;) {
            printf("Enter A/D converter value for start of next\n");
            printf("calibration line (2047 = last point)\n");
            scanf("%d",&ad_val[k]);
            if((ad_val[k] < ad_val[k-1]) || (ad_val[k] > 2047))
               printf("Value must be > %d and < 2048, please re-enter\n",
                       ad_val[k-1]);
            else good_val = TRUE;
         }
      }
      else if (k == end) ad_val[k] = 2047; /* maximum # of curves = 10 */
      printf("Enter floating point value to represent A/D value %d\n",
               ad_val[k]);
      scanf("%f",&flt_val[k]);
      if(ad_val[k] == 2047) flg = TRUE;
   }
   return;
 
}
 
/***************************************************/
/* subroutine to save current setup as a disk file */
/***************************************************/


static void save_atod()
{
   FILE *fp;
   char fname[11];
   int k;

/* get filename from the user */

   printf("Enter the filename to save the setup in\n");
   scanf("%s",fname);

/* open file and write out the current setup */

   fp = fopen(fname,"w");

   for(k=0;k<40;k++)
      fprintf(fp,"%d,%f\n",ad_val[k],flt_val[k]);
   fprintf(fp,"%d,%d,%d\n",threshold,*daisychain,*modesel);

   fclose(fp);

   return;
}

/*************************************************************/
/* subroutine to program the A/D card with the current setup */
/*************************************************************/


static void prog_atod(typ,bd)
unsigned char typ;
unsigned long bd;
{

/* assign pointers to the various A/D card registers */

   long *atod_qh;
   long *atod_ql;
   long *atod_ih;
   long *atod_il;
   long *atod_z;
   unsigned char *atod_c;
   unsigned char *atod_dc;

/* declare and set masks for creating DSP32 floating point */

   long maskei = 0x7f800000;
   long masks = 0x80000000;
   long maskfi = 0x7fffff;

/* declare needed variables */
   float dtemp1,dtemp2;
   float fnum,slope;
   long qhi, ihi;
   unsigned long magnitude,threshSquared;
   long ltemp,k,j,i,m;
   unsigned long dsp_lwd;
   long dsp_exp,dsp_sgn,dsp_frc;
   long start,end,off;
   long flg,val1,val2,temp;
   long *inum; 
   atod_qh = (long *) (bd + ATOD_QH);
   atod_ql = (long *) (bd + ATOD_QL);
   atod_ih = (long *) (bd + ATOD_IH);
   atod_il = (long *) (bd + ATOD_IL);
   atod_z  = (long *) (bd + ATOD_Z);
   atod_c  = (unsigned char *) (bd + ATOD_CH);
   atod_dc = (unsigned char *) (bd + ATOD_DC);

   inum = (long *) &fnum;

/* write out the calibrations to the various memory locations */

   for(m = 0; m < 5; m++) {              /* m is the memory type counter */
      start = m * 10;
      end = start + 9;
      printf("first for loop m = %ld start = %ld end = %ld\n",m,start,end);
      for(j = start, flg = FALSE;j < end, flg == FALSE; j++) {
        slope = (flt_val[j+1] - flt_val[j]) / (float)(ad_val[j+1] - ad_val[j]);
        printf("second for loop slope = %f ad_val[j] = %d ad_val[j+1] = %d\n",
        slope,ad_val[j],ad_val[j+1]);
        for(i = ad_val[j]; i <= ad_val[j+1] ;i++) {
            fnum = flt_val[j] + slope * (float)(i - ad_val[j]);
            off = i & 0xfff;
	    if (typ==1)
	      dsp_lwd = i3e2dsp(fnum);
	    else
	      dsp_lwd = (long)fnum;

            /* store the number in the correct place */
            switch ( m ) {

               case 0:   /* quadrature memory high converter */
                  *(atod_qh + off) = dsp_lwd;
                  break;

               case 1:   /* quadrature memory low converter */
                  *(atod_ql + off) = dsp_lwd;
                  break;  

               case 2:   /* in-phase memory high converter */
                  *(atod_ih + off) = dsp_lwd;
                  break;

               case 3:   /* in-phase memory low converter */
                  *(atod_il + off) = dsp_lwd;
                  break;

               case 4:   /* log reflectivity memory */
                  *(atod_z + off) = dsp_lwd;
                  break;
            }
         }
         if(ad_val[j+1] == 2047) flg = TRUE;
      }
   }

/* set the choice  memory based on the threshold */

   threshSquared = threshold * threshold;
   for(k = 1 ; k < 0x20000 ; k = k + 2) {/* k is offset to the memory */
     ltemp = k / 2;
     ltemp ^= 0x8000;           /* invert the m.s. bit to correspond */
                                /* to the VME address decode error */

     /* compute the corresponding value of Q High (upper 8 bits only) */
     qhi = (ltemp & 0xff) <<4;  /* AND off upper eight bits & shift up 4 */
     if (qhi & 0x800)           /* if the sign bit is set, get magnitude */
       qhi = (~qhi & 0xfff) + 1;

     /* compute the corresponding value of I High (upper 8 bits only) */
     ihi = (ltemp & 0xff00) >>4;  /* AND off lower eight bits & shift down 4 */
     if (ihi & 0x800)           /* if the sign bit is set, get magnitude */
       ihi = (~ihi & 0xfff) + 1;

     magnitude = qhi*qhi + ihi*ihi;
     if (magnitude >= threshSquared)
       *(atod_c + k) = 0;
     else
       *(atod_c + k) = 1;
   }
   return;
}

/*************************************************************/
/* subroutine to retrieve an A/D card setup from a disk file */
/*************************************************************/

static void retr_atod()
{
   FILE *fp;
   char fname[11];
   long k;

/* get filename from the user */

   printf("Enter the filename to retrieve the setup from\n");
   scanf("%s",fname);

/* open file and read in the setup */

   fp = fopen(fname,"r");

   for(k=0;k<40;k++)
      fscanf(fp,"%d,%f\n",&ad_val[k],&flt_val[k]);
   fscanf(fp,"%d,%d,%d\n",&threshold,daisychain,modesel);

   fclose(fp);

   return;
}
 
#ifdef NOTDEF

/*
                Module: i3e2dsp.c
                Author: Grant Gray
                Date:   4/2/89
 
Subroutine to convert ieee floating point to DSP32 floating point.
Single-precision only.
Argument is an IEEE float to be converted to DSP32/32C format. 
 
*/

unsigned long i3e2dsp(fn)
float fn;
{
   union fpn
	{
	float f;
	long  l;
	}pn;
   unsigned long  mant = 0, sgn = 0, ex = 0;
   unsigned long ans;
 
   pn.f = fn;
   sgn = pn.l & 0x80000000;
   mant = pn.l & 0x7FFFFF;
   mant = mant << 8;
   ex = pn.l & 0x7F800000;
   ex = ex >> 23;
   ex += 1;
   if(ex>0xff)ex = 0xff;

   if((pn.l & 0x7fffffff) == 0)return(0);
 
   if(ex == 0xFF)
   {
      if(sgn == 0) ans = 0x7FFFFFFF;
      else ans = 0x800000FF;
   }
   else if(ex == 0) ans = 0;
   else
   {
      if(sgn != 0)
      {
         mant = ((mant ^ 0x7fffff00) + 0x100);
         if((mant & 0x80000000) != 0)ex = ex -1;
         mant = mant & 0x7fffff00;
      }
        
      ans = sgn | mant | ex;
   }   
   return(ans);
}

#endif
