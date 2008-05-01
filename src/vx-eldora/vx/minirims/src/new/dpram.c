#include        <stdio.h>
#include        <vxWorks.h>
#include        <taskLib.h>

/* #include        "..\include\globals.h" */

/* integer, short, and char values */
#define IVALUE(a) (((a) & 0xFF) + (((int)(((a) & 0xFF))+1)<<8) + (((int)(((a) & 0xFF))+2)<<16) + (((int)(((a) & 0xFF))+3)<<24))
#define SVALUE(a) (unsigned short)((unsigned short)((a) & 0xFF) + (((unsigned short)(((a) & 0xFF))+1)<<8))
#define CVALUE(a) ((unsigned char)((a) & 0xFF))

dpram(int membase,char type)
   {

   if(type > 2 || type < 0)
       {
         /* default to integer test */
           printf("Use: DPRAM ADDRESS [TYPE] (type 0,1,2  -> char short int\n"); exit(0);
       } 
   switch(type)
      {
      case 0:   cdpram(membase);  break;
      case 1:   sdpram(membase);  break;
      case 2:   idpram(membase);  break;
      default:  printf("Use: DPRAM ADDRESS [TYPE] (type 0,1,2  -> char short int\n"); 
      }
   }

/* do chars through memory but just delay on error */
qdpram(int membase)
   {
   int i,j,n,success,count=0,ecount=0;
   volatile     int     k;
   volatile unsigned char *base;   
   unsigned char temp;

   base = (unsigned char *)membase;
   
   success = 1;

   printf("Dual port RAM test (CHARS)..... ");    fflush(stdout);

   for(j=0; !kbhit(); j++)   
      {
      for(i=0; i<65536; i++)
         {
         *(base + i) = CVALUE((j + i) & 0xff);
         for(k=0; k<50; k++);
         temp = *(base + i); 
         if( temp != CVALUE((j + i) & 0xff)) 
            {for(k=0; k<500; k++); success = 0;}
         }
      }
   getch();
   printf("%s\n",success?"OK":"BAD");
   }

/* repeatedly test just one location very fast */
odpram(int membase)
   {
   int j,n,success,count=0,ecount=0;
   volatile int i;
   volatile unsigned short *base;   

   base = (volatile unsigned short *)(membase);
   
   success = 1;

   printf("writing and reading forever until a key is hit...");
   fflush(stdout);
   while(!kbhit())
      {
      for(i=0; i<100000; i++);
      count++;
      *base = IVALUE(count & 0xff);
      for(i=0; i<100000; i++);
      i = *base;
      if(i != count) ecount++; /*     printf("%04X\n",i^0x5555);  */
      }
   getch();
   printf("\n");
   }

cdpram(int membase)
   {
   int i,j,n,success,count=0,ecount=0;
   unsigned char *base;   

   base = (unsigned char *)(membase);
   
   success = 1;

   printf("Dual port RAM test (CHARS)..... ");    fflush(stdout);

   for(j=0; j<256; j++)   
      {
      for(i=0; i<65536; i++)  /* 64K chars */
         {
         *(base + i) = CVALUE((j + i) & 0xff);
         }

      for(i=0; i<65536; i++)
         {
         if( *(base + i) != CVALUE((j + i) & 0xff)) 
           {
           if(success==1) printf("\n");
           printf("address %02X = %02X should be %02X\n",membase + i * sizeof(char),*(base + i),CVALUE((j + i) & 0xff));
           success = 0;
           }
         }
      }
   
   printf("%s\n",success?"OK":"BAD");
   }


sdpram(int membase)
   {
   int i,j,n,success,count=0,ecount=0;
   unsigned short *base;   

   base = (unsigned short *)(membase);
   
   success = 1;

   printf("Dual port RAM test (SHORTS)..... ");    fflush(stdout);

   for(j=0; j<256; j++)   
      {
      for(i=0; i<32768; i++)  /* 16K words */
         {
         *(base + i) = SVALUE((j + i) & 0xff);
         }

      for(i=0; i<32768; i++)
         {
         if( *(base + i) != SVALUE((j + i) & 0xff)) 
           {
           if(success==1) printf("\n");
           printf("address %04X = %04X should be %04X\n",membase + i * sizeof(short),*(base + i),SVALUE((j + i) & 0xff));
           success = 0;
           }
         }
      }
   
   printf("%s\n",success?"OK":"BAD");
   }

idpram(int membase)
   {
   int i,j,n,success,count=0,ecount=0,*base;

   base = (int *)(membase);
   
   success = 1;

   printf("Dual port RAM (LONG) ..... ");    fflush(stdout);

   for(j=0; j<256; j++)   
      {
      for(i=0; i<16384; i++)  /* 16K words */
         {
         *(base + i) = IVALUE((j + i) & 0xff);
         }

      for(i=0; i<16384; i++)
         {
         if( *(base + i) != IVALUE((j + i) & 0xff)) 
           {
           if(success==1) printf("\n");
           printf("address %08X = %08X should be %08X\n",membase + i * sizeof(int),*(base + i),IVALUE((j + i) & 0xff));
           success = 0;
           }
         }
      }
   
   printf("%s\n",success?"OK":"BAD");
   }

kbhit(void)
{
    return(1);

}
getch(void)
{
    return;
}
