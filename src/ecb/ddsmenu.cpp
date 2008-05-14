//DDS Menu Class
//Written by Drew and Charlie


#include "ddsmenu.h"

DDSMenu::DDSMenu(DDS* dds[], int numdds):

    _dds(dds),
    _numdds(numdds)
{

    
  //Initialize NRflag and freq array to 0
  for(int i=0;i<_numdds;i++)
  {
    _NRflag[i]=0;
    _freq[i]=0;
  }

  //Initialize Inbuf array to 0
  for(int i=0;i<14;i++)
  {
    _inbuf[i]=0;
  }

  OpenMenu();
}

///////////////////////////////////////////////////
DDSMenu::~DDSMenu()
{}
//////////////////////////////////////////////////
void
DDSMenu::OpenMenu()
{

  int uin;
  int nrin;
  int numarg;
  int unit;
  double freqin;
  int err_flag=0;
  int N;
  int B;
  int T;
  double f=0;

  uin=1;
  freqin=9.55e9;
  nrin=-1;

  //Initialize All DDS Units
  for(unit=0;unit<_numdds;unit++)
  {
      _dds[unit]->Rfinit();    
  }
    
  //Load DDS Text File
  LoadFile();

  //Load Previously Programmed Values onto DDS
  for(unit=0;unit<_numdds;unit++)
  {
      _dds[unit]->Rfputval(_freq[unit],_NRflag[unit]);
  }  

  while(1)
  {
    printf("\n----------DDS Frequency Generator Menu-----------------\n\n");
    if(LoadFile())
    {
        
      printf("Last values programmed were: \n");
      printf("DDS  Frequency          Noise Reduction   \n");
      for(_idx=0;_idx<_numdds;_idx++)
      {
        //print out what is in DDStext
        printf("  %d  %lf	 %d               \n",_idx+1,_freq[_idx],_NRflag[_idx]);
      }

    }
    else
    {
      printf("There are no previously programmed values!\n");
      printf("Creating DDS_Data.txt File.\n");
      if (InitFile())
      {
        printf("DDS_Data.txt Created and Initialized.\n");
        for(_idx=0;_idx<_numdds;_idx++)
        {
          printf("  %d  %lf	 %d               \n",_idx+1,_freq[_idx],_NRflag[_idx]);
        }
      }
      else
      {
        printf("DDS_Data.txt Could Not Be Created.\n");
      }
    }
    printf("\n1. Enter DDS unit number (1-%d)\n",_numdds);
    printf("2. Desired frequency (Hz) (9.3-9.8Ghz); \n");
    printf("3. Optional NR flag (0 or 1)\n");
    printf("Or enter (0) to exit the menu\n");
    printf("\nEnter Information: ");

    mygetline();
    numarg = sscanf(_inbuf,"%d %lf %d",&uin,&freqin,&nrin);

    //Change uin from 1 based to 0 based
    uin = uin -1 ;

    if(uin == -1)
    {
      break;
    }
    if((uin < -1) || (uin > (_numdds-1)))
    {
      printf("ONLY 0-%d ARE VALID OPTIONS\n",_numdds);
      printf("You entered %d. \n",(int) (uin+1));
      err_flag = 1;
      break;
    }

    if((freqin < 9.0e9) || (freqin > 10.0e9))
    {
      printf("Frequency must be between 9 and 10 GHz.\n");
      printf("You entered %.6f.\n",freqin);
      err_flag = 1;
    }

    if(numarg !=3)
    {
      nrin = -1;
    }

    else
    {
      if((nrin != 0) && (nrin !=1))
      {
        printf("Noise reduction flag must be 0 or 1.\n");
        printf("You entered %d.\n",nrin);
        err_flag=1;
      }
    }
    printf("\n");

    if(!err_flag)
    {

      B = 361 - (int)(freqin/30e6);
      N = B + 1;
      T = (int)(pow(2,24)/(B+1.0)*(256.0*181/3 - freqin/703125.0) + 0.5);

      _NRflag[uin]=nrin;
      _freq[uin] = 6e7*(181-T/pow(2,16)*(B+1)*3/pow(2,16));

      AppendFile();
      if(!AppendFile())
      {
        printf("\nError Appending file!\n");
      }

      //Program DDS units with user input values
      if(uin >= 0 && uin <= (_numdds-1))
      {
        _dds[uin]-> Rfputval(freqin, nrin);
      }
    }
    else
    {
      err_flag=0;
    }

  }

}
//////////////////////////////////////////////////////////////////////////////
int
DDSMenu::LoadFile()
{

  FILE *fin;

  fin = fopen("DDS_Data.txt","r");

  if(!fin)
  {
    return 0;
  }
  else
  {
    for(_idx=0;_idx<_numdds;_idx++)
    {
      fscanf(fin,"%lf %i", &_freq[_idx], &_NRflag[_idx]);
    }


    fclose(fin);
    return 1;
  }
}
//////////////////////////////////////////////////////////////////////////////
int
DDSMenu::InitFile()
{

  FILE *fout;

  fout = fopen("DDS_Data.txt","w");

  if(!fout)
  {
    return 0;
  }
  else
  {
    for(_idx=0;_idx<_numdds;_idx++)
    {
      fprintf(fout, "0 0\n");
    }
    fclose(fout);
    return 1;
  }
}
//////////////////////////////////////////////////////////////////////////////
int
DDSMenu::AppendFile()
{
  FILE *fout;

  fout = fopen("DDS_Data.txt","w");

  if(!fout)
  {
    return 0;
  }
  else
  {
    for(_idx=0;_idx<_numdds;_idx++)
    {
      fprintf(fout,"%lf %i\n",_freq[_idx],_NRflag[_idx]);
    }
    fclose(fout);
    return 1;
  }
}
//////////////////////////////////////////////////////////////////////////////
short
DDSMenu::mygetline()
{
  short idx;
  char c1;

  idx = 0;

  while(1)
  {
    c1 = getchar();
    if ((c1 == '\n') || (c1 == '\r'))
    {
      _inbuf[idx] = '\0';
      return(idx);
    }
    if((c1== 0x08) || (c1 == 0x7f))
    {
      if(idx == 0)
      {
        continue;
      }
      idx--;
      putchar(0x08);
      putchar(0x20);
      putchar(0x08);
      continue;
    }
    _inbuf[idx] = c1;
    idx++;
  }
}
