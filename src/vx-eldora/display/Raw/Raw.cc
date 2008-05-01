//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          Raw.cc
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.2  1996/06/21  19:27:26  thor
// First truely working version.
//
// Revision 1.1  1996/03/25  21:57:30  thor
// Initial revision
//
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#pragma implementation

#include "Raw.hh"
#ifndef OK_RPC
#define OK_RPC
#endif
#include "TimeSeries.h"
#include "IndFreq.h"

Raw::Raw(GraphicController *gbd) : Display(gbd)
{
  Wdw[0].init(gbd,0,0,0,Raw::MAX_WIDTH,Display::FULL_HEIGHT,0,0);

  Wdw[0].setPriority(0);
  
  numOfWdws = 1;
  numOfParams = 1;

  makeStr->precision(1);
  makeStr->setf(ios::fixed);
}

void Raw::reset(Header *hdr, DispCommand *cmd)
{
  CELLSPACING *cs = hdr->CellSpacing();

  FAST int seg = cs->num_segments;
  FAST int ngates = 0;
  FAST short *nc = &cs->num_cells[0];

  for (FAST int i = 0; i < seg; i++) // Calculate maximum distance & # gates.
    {
      FAST int c = *nc++;

      ngates += c;
    }
  FAST RADARDESC *rd = hdr->Radar(1);

  FAST int np = rd->num_parameter_des; // How are there total?

  FAST int data_len = (ngates * np * sizeof(unsigned short)) +
    sizeof(DataBeam);
  FAST int indep_freq_len;
  FAST int time_series_len;

  FAST FIELDRADAR *fldrdr = hdr->FieldRadar();
  
  if(fldrdr->indepf_times_flg > 0)
    indep_freq_len = (rd->num_freq_trans * 8 * rd->num_ipps_trans) +
      sizeof(INDEP_FREQ);
  else
    indep_freq_len = 0;

  if(fldrdr->indepf_times_flg == 3)
    {
      WAVEFORM *wvfm = hdr->Waveform();
      time_series_len = (rd->num_freq_trans * 2 * 4 * wvfm->repeat_seq_dwel *
                         wvfm->num_chips[0]) + sizeof(TIME_SERIES);
    }
  else
    time_series_len = 0;
  
  bytesPerBeam =  data_len + indep_freq_len + time_series_len;

  if (cmd->cmd == AFT_RAW)
      dispType = Display::RAW_AFT;
    else
      dispType = Display::RAW_FORE;

  agc->setMask(0);
  agc->clear();
  drawTitle(0,0);

  refreshClock();

  Point a;

  a.x = Raw::MAX_WIDTH - Clock::WIDTH - 2;
  a.y = Display::FULL_HEIGHT - Clock::HEIGHT - 2;

  moveClock(a);
  
  agc->setMask(0x80);

  setcolor(9);                  // Default to a blue color.
  
  setPriority(1);

  Wdw[0].setTextScale(2,2);
  Wdw[0].setTextBackGround(BLACK);
  
  displaySet(0);

  memset((void *)&previous,0,sizeof(RAW_DATA));
}

void Raw::drawTitle(int set, int radar)
{
  Point a;
  Point b;

  a.x = 0;
  a.y = 0;

  b.x = 0;
  b.y = Raw::HEIGHT;

  FAST int inc = Raw::WIDTH / 10;
  
  while (a.x < Raw::WIDTH)
    {
      line(0,a,b,WHITE);

      a.x += inc;
      b.x += inc;
    }

  a.x = Raw::WIDTH - 1;
  b.x = a.x;
  line(0,a,b,WHITE);
  
  a.x = 0;
  a.y = 0;

  b.x = Raw::WIDTH;
  b.y = 0;

  inc = Raw::HEIGHT / 10;
  
  while (a.y < Raw::HEIGHT)
    {
      line(0,a,b,WHITE);

      a.y += inc;
      b.y += inc;
    }
  a.y = Raw::HEIGHT - 1;
  b.y = a.y;

  line(0,a,b,WHITE);
}

void Raw::drawLabels(FAST RAW_DATA *header)
{
  char xlabel[9];
  strncpy(xlabel,&header->x_label[0],8);

  char ylabel[9];
  strncpy(ylabel,&header->y_label[0],8);

  xlabel[8] = 0;
  ylabel[8] = 0;
  
  Point a;

  a.x = Raw::WIDTH + 15;
  a.y = (Raw::HEIGHT / 2) - 5;

  Wdw[0].horText(a,ylabel,WHITE1);

  resetString();
  *makeStr << header->ymax;
  a.y = 15;
  Wdw[0].horText(a,outputStr,WHITE1);

  resetString();
  *makeStr << header->ymin;
  a.y = Raw::HEIGHT - 15;
  Wdw[0].horText(a,outputStr,WHITE1);
  
  a.y = Raw::HEIGHT + 7;
  a.x = Raw::WIDTH / 2;

  Wdw[0].horText(a,xlabel,WHITE1);

  resetString();
  *makeStr << header->xmin;
  a.x = 10;
  Wdw[0].horText(a,outputStr,WHITE1);

  constant = (double)Raw::HEIGHT / (header->ymax - header->ymin);
}

void Raw::displaySet(int set)
{
  Wdw[0].display();
  showClock();
}

void Raw::undisplay()
{
  Wdw[0].undisplay();
  hideClock();
}

void Raw::switchSets()
{
}

