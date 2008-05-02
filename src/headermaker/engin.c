#define scope extern
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

/**************************************************************************/
/*          SUBROUTINE INITIALIZE ENGIN STRUCTURE                   */
/*************************************************************************/

void initialize_engin()
{
    engin_win.schemenum = -999;
    engin_win.numsamples = -999;
    engin_win.prf = -999;
    engin_win.numgates = -999;
    engin_win.chipdur = -999;
    engin_win.first_gate_dis = -999;
    engin_win.forehpa_snum = -999;
    engin_win.afthpa_snum = -999;
    engin_win.rot_spacing = -999;
    engin_win.avg_freq[0] = -999;
    engin_win.avg_freq[1] = -999;
    engin_win.avg_freq_index[0] = -999;
    engin_win.avg_freq_index[1] = -999;

}/*end initialize_engin_structure*/


/************************************************************************/
/*          SUBROUTINE INITIALIZE ENGIN STATE                           */
/*************************************************************************/

void initialize_engin_state()
{
    engin_st.schemenum = 0;
    engin_st.numsamples = 0;
    engin_st.prf = 0;
    engin_st.numgates = 0;
    engin_st.chipdur = 0;
    engin_st.first_gate_dis = 0;
    engin_st.forehpa_snum = 0;
    engin_st.afthpa_snum = 0;
    engin_st.rot_spacing = 0;
    engin_st.avg_freq[0] = 0;
    engin_st.avg_freq[1] = 0;
    engin_st.avg_freq_index[0] = 0;
    engin_st.avg_freq_index[1] = 0;

}/*end initialize_engin_state*/

/**************************************************************************/
/*          SUBROUTINE CALCULATE ENGIN STRUCTURE                         */
/*************************************************************************/

int calculate_engin()
{

double temp;
int number_to_calculate;
number_to_calculate = 2;

if(engin_st.avg_freq[0] > 0)
  number_to_calculate--;
else if(radar_st.num_freq_trans > 0  && radar_st.freq1 > 0)
  {

      temp = 0;
      if(1 <= radar[FORE].num_freq_trans)
	temp += radar[FORE].freq1;
      if(2 <= radar[FORE].num_freq_trans)
	temp += radar[FORE].freq2;
      if(3 <= radar[FORE].num_freq_trans)
	temp += radar[FORE].freq3;
      if(4 <= radar[FORE].num_freq_trans)
	temp += radar[FORE].freq4;
      if(5 <= radar[FORE].num_freq_trans)
	temp += radar[FORE].freq5;

      engin_win.avg_freq[FORE] = temp/radar[FORE].num_freq_trans;

      temp = 0;
      if(1 <= radar[AFT].num_freq_trans)
	temp += radar[AFT].freq1;
      if(2 <= radar[AFT].num_freq_trans)
	temp += radar[AFT].freq2;
      if(3 <= radar[AFT].num_freq_trans)
	temp += radar[AFT].freq3;
      if(4 <= radar[AFT].num_freq_trans)
	temp += radar[AFT].freq4;
      if(5 <= radar[AFT].num_freq_trans)
	temp += radar[AFT].freq5;

      engin_win.avg_freq[AFT] = temp/radar[AFT].num_freq_trans;

      engin_st.avg_freq[0] = 3;
      number_to_calculate--;
  }


if(engin_st.avg_freq_index[0] > 0)
  number_to_calculate--;
else if(engin_st.avg_freq[0] > 0)
  {
      engin_win.avg_freq_index[FORE] = (engin_win.avg_freq[FORE] - 9.25) * 10.0;
      engin_win.avg_freq_index[AFT] = (engin_win.avg_freq[AFT] - 9.25) * 10.0;

      engin_st.avg_freq_index[0] = 3;
      number_to_calculate--;
  }

return(number_to_calculate);
     
}/*end calculate_engin */

