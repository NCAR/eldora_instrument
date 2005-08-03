#include <stream.h>
#include <unistd.h>
#define OK_RPC
#define CLIENT_SIDE
#include "RadarRpc.hh"

static char *menu[] = {
"1> Start",
"2> Stop",
"3> Restart",
"4> Reboot",
"5> Resync",
"6> DC Removal Freq 1 - High channel",
"7> DC Removal Freq 1 - Low channel",
"8> DC Removal Freq 2 - High channel",
"9> DC Removal Freq 2 - Low channel",
"10> DC Removal Freq 3 - High channel",
"11> DC Removal Freq 3 - Low channel",
"12> FFT Freq 1",
"13> FFT Freq 2",
"14> FFT Freq 3",
"15> Init",
"16> DC Removal - all frequencies",
"17> LOAD",
"18> Send command",
"19> Exit",
0
};

main(int argc, char **argv)
{
    char *t1 = NULL;
    char *t2 = NULL;

    if (argc == 1)
      {
	  cerr << "Must give at least one target name!" << endl;
	  exit(1);
      }
    else if (argc == 2)
      {
	  t1 = argv[1];
      } else {
	  t1 = argv[1];
	  t2 = argv[2];
      }

    CLIENT *c1;
    CLIENT *c2;

    if ((c1 = clnt_create(t1,RadarControl,RadarCtrlVers,"udp")) == NULL)
      {
	  clnt_pcreateerror(t1);
	  exit(1);
      }

    if (t2 != NULL)
      {
	  if ((c2 = clnt_create(t2,RadarControl,RadarCtrlVers,"udp")) == NULL)
	    {
		clnt_pcreateerror(t2);
		exit(1);
	    }
      }

    RadarCommand cmdBlk;

    cmdBlk.cmd = 0;

    for (;;)
      {
	  char **ptr = menu;

	  for (; *ptr; ptr++)
	    cout << *ptr << endl;

	  int ans;

	  cin >> ans;

	  switch(ans)
	    {
	      case 1:
		cmdBlk.cmd |= START;
		break;

	      case 2:
		cmdBlk.cmd |= STOP;
		break;

	      case 3:
		cmdBlk.cmd |= RELOAD;
		break;

	      case 4:
		cmdBlk.cmd |= REBOOT;
		break;

	      case 5:
		cmdBlk.cmd |= RESYNC;
		break;

	      case 6:
		cmdBlk.cmd &= DC_NULL;
		cmdBlk.cmd |= DC_REMOVAL | DC_1 | AD_CHAN_HI;
		break;

	      case 7:
		cmdBlk.cmd &= DC_NULL;
		cmdBlk.cmd |= DC_REMOVAL | DC_1 | AD_CHAN_LO;
		break;

	      case 8:
		cmdBlk.cmd &= DC_NULL;
		cmdBlk.cmd |= DC_REMOVAL | DC_2 | AD_CHAN_HI;
		break;

	      case 9:
		cmdBlk.cmd &= DC_NULL;
		cmdBlk.cmd |= DC_REMOVAL | DC_2 | AD_CHAN_LO;
		break;

	      case 10:
		cmdBlk.cmd &= DC_NULL;
		cmdBlk.cmd |= DC_REMOVAL | DC_3 | AD_CHAN_HI;
		break;

	      case 11:
		cmdBlk.cmd &= DC_NULL;
		cmdBlk.cmd |= DC_REMOVAL | DC_3 | AD_CHAN_LO;
		break;

	      case 12:
		cmdBlk.cmd &= FFT_NULL;
		cmdBlk.cmd |= FFT | FFT_1;
		break;

	      case 13:
		cmdBlk.cmd &= FFT_NULL;
		cmdBlk.cmd |= FFT | FFT_2;
		break;

	      case 14:
		cmdBlk.cmd &= FFT_NULL;
		cmdBlk.cmd |= FFT | FFT_3;
		break;

	      case 15:
		cmdBlk.cmd |= INIT;
		break;

	      case 16:
		cmdBlk.cmd &= DC_NULL;
		cmdBlk.cmd |= DC_REMOVAL;
		break;

	      case 17:
		cmdBlk.cmd |= LOAD;
		break;

	      case 18:
		(void)sendradarcommand_1(&cmdBlk,c1);
		if (t2 != NULL)
		  (void)sendradarcommand_1(&cmdBlk,c2);
		cmdBlk.cmd = 0;
		break;

	      default:
		exit(0);
		break;
	    }
	  cout << endl << endl << endl;
      }
}
