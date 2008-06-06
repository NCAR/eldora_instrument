#include "EldoraRadarParams.h"

EldoraRadarParams::EldoraRadarParams()
{
    // fields from the header waveform block (WAVE)
    for (unsigned int i = 0; i < sizeof(wave_nchips); i++)
        wave_nchips[i] = 1;
    wave_nchips[4] = 0;

    wave_pcprep = 1;
/*
    wave_chpoff[0] = 17;
    wave_chpoff[1] = 62;
    wave_chpoff[2] = 107;
    wave_chpoff[3] = 152;
    wave_chpoff[4] = 0;
    wave_chpoff[5] = 0;
*/    

    wave_chpoff[0] = 17;
        wave_chpoff[1] = 77;
        wave_chpoff[2] = 137;
        wave_chpoff[3] = 197;
        wave_chpoff[4] = 0;
        wave_chpoff[5] = 0;

    for (unsigned int i = 0; i < sizeof(wave_chpwid); i++)
        //wave_chpwid[i] = 45;
        wave_chpwid[i] = 60;
    wave_chpwid[4] = 0;
    //wave_chpwid[5] = 197;
    wave_chpwid[5] = 257;

    for (unsigned int i = 0; i < sizeof(wave_ngates); i++)
        wave_ngates[i] = 400;
    wave_ngates[5] = 0;
    

/*
    wave_gate1[0] = 75;
    wave_gate1[1] = 45;

    wave_gate2[0] = 75;
    wave_gate2[1] = 45;

    wave_gate3[0] = 75;
    wave_gate3[1] = 45;

    wave_gate4[0] = 75;
    wave_gate4[1] = 45;

    wave_gate5[0] = 75;
    wave_gate5[1] = 45;
*/

    wave_gate1[0] = 90;
    wave_gate1[1] = 60;

    wave_gate2[0] = 90;
    wave_gate2[1] = 60;

    wave_gate3[0] = 90;
    wave_gate3[1] = 60;

    wave_gate4[0] = 90;
    wave_gate4[1] = 60;

    wave_gate5[0] = 90;
    wave_gate5[1] = 60;

    //wave_msrep = 0.33333333;
    wave_msrep = 1.125;
    wave_seqrep = 45;

    // fields from the header radar descriptor block (RADD)
    for (unsigned int i = 0; i < 4; i++)
        radd_name[i] = "FORE"[i];

    radd_const = -82.350868;
    radd_noipow = -120.0;
    radd_unvel = 23.168688;
    radd_unrng = 50.0;
    radd_nfreq = 4;
    //radd_nipp = 1;
    radd_nipp = 2;
        
    radd_freq1 = 9.709332;
    radd_freq2 = 9.700000;
    radd_freq3 = 9.690667;
    radd_freq4 = 9.718666;
    radd_freq5 = -999.0;
    
    //radd_ipp1 = 0.33333333;
    radd_ipp1 = 0.5;
    //radd_ipp2 = -999.0;
    radd_ipp2 = 0.625;
    radd_ipp3 = -999.0;
    radd_ipp4 = -999.0;
    radd_ipp5 = -999.0;

    // fields from the header field radar info block (FRIB)
    frib_rxgain[0] = 24.299999;
    frib_rxgain[1] = 24.400000;
    frib_rxgain[2] = 24.400000;
    frib_rxgain[3] = 24.799999;
    frib_rxgain[4] = 30.000000;
    frib_lnalos = 1.829850;
    frib_xgain = 22.1;
    frib_tsgat = 68;
    frib_frqgat= 10;
    
    // scale and bias for the DDS data compression
    parm_dbz_scale= 10.0; 
    parm_dbz_bias = 350.0;
    
    parm_sw_scale = 400.0;
    parm_sw_bias  = 0.0;
    
    parm_ncp_scale= 1024.0;
    parm_ncp_bias = 0.0;
    
    parm_vs_scale = 400.0;
    parm_vs_bias  = 0.0;
     
    parm_vl_scale = 400.0;
    parm_vl_bias  = 0.0;
    
    parm_vr_scale = 400.0;
    parm_vr_bias  = 0.0;

}
