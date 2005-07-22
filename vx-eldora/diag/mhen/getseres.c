static  int     AI=0,OI=0;
static  float   OFLOW[1000];  /* could crash if HITS > 500 */
/* AI is the remembered count of the building array (a[]) */
/* and OI is the remembered count of the overflow array */

/* builds a[na] out of several b[nb] 's */
/* where na is the size of a, and nb is the size of b */
int     getseries(float  a[], int na, float b[], int nb)
   {
   int          i;
   
   /* make sure target is never less than source size */
   if(na < nb) return(0);
   
   /* start new array with any leftover from last call */
   /* note: there is overflow only if there is a new array starting */
   /*       AI is guaranteed to be zero in this case */
/*   for(; OI && AI < na; OI--,AI++)  a[AI] = OFLOW[AI]; */
   
   /* fill up array with samples from each call */
   for(i=0; i<nb && AI<na; i++,AI++)   a[AI] = b[i];
      
   /* test to see if array has been filled */
   if(AI >= na)  {AI=0; OI=0;}
   
   /* after array is full, save any leftover from dwell in oflow */
/*   for(; i<nb; i++)   OFLOW[OI++] = b[i]; */
   
   return(!AI);      /* return status of array */
   }
