#include <iostream>
#include "EldoraRadarParams.h"

EldoraRadarParams::EldoraRadarParams()
{
    // fields from the header waveform block (WAVE)
    for (unsigned int i = 0; i < sizeof(wave_nchips); i++)
        wave_nchips[i] = 1;
    wave_nchips[4] = 0;
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
    
    // fields from the header cell spacing descriptor
    cspd_width[0] = 150.0;
    cspd_width[1] = 150.0;
    cspd_width[2] = 150.0;
    cspd_width[3] = 150.0;
    cspd_width[4] = -999.0;
    cspd_width[5] = -999.0;

    // scale and bias for the DDS data compression
    parm_dbz_scale= 10.0;
    parm_dbz_bias = 350.0;

    parm_sw_scale = 400.0;
    parm_sw_bias = 0.0;

    parm_ncp_scale= 1024.0;
    parm_ncp_bias = 0.0;

    parm_vs_scale = 400.0;
    parm_vs_bias = 0.0;

    parm_vl_scale = 400.0;
    parm_vl_bias = 0.0;

    parm_vr_scale = 400.0;
    parm_vr_bias = 0.0;

}

/////////////////////////////////////////////////////////////////////////
void EldoraRadarParams::dump()
{

    std::cout << "wave_nchips: ";
    for (int i = 0; i < 6; i++)
        std::cout << wave_nchips[i] << " ";
    std::cout << "\n";

    std::cout << "wave_chpoff: ";
    for (int i = 0; i < 6; i++)
        std::cout << wave_chpoff[i] << " ";
    std::cout << "\n";

    std::cout << "wave_chpwid: ";
    for (int i = 0; i < 6; i++)
        std::cout << wave_chpwid[i] << " ";
    std::cout << "\n";

    std::cout << "wave_ngates: ";
    for (int i = 0; i < 5; i++)
        std::cout << wave_ngates[i] << " ";
    std::cout << "\n";

    std::cout << "wave_gate1: ";
    for (int i = 0; i < 2; i++)
        std::cout << wave_gate1[i] << " ";
    std::cout << "\n";

    std::cout << "wave_gate2: ";
    for (int i = 0; i < 2; i++)
        std::cout << wave_gate2[i] << " ";
    std::cout << "\n";

    std::cout << "wave_gate3: ";
    for (int i = 0; i < 2; i++)
        std::cout << wave_gate3[i] << " ";
    std::cout << "\n";

    std::cout << "wave_gate4: ";
    for (int i = 0; i < 2; i++)
        std::cout << wave_gate4[i] << " ";
    std::cout << "\n";

    std::cout << "wave_gate5: ";
    for (int i = 0; i < 2; i++)
        std::cout << wave_gate5[i] << " ";
    std::cout << "\n";

    std::cout << "wave_msrep: " << wave_msrep << "\n";
    std::cout << "wave_seqrep: "<< wave_seqrep << "\n";

    std::cout << "radd_name:";
    for (int i = 0; i < 4; i++)
        std::cout << radd_name[i];
    std::cout << "\n";

    std::cout << "radd_const: " << radd_const << "\n";
    std::cout << "radd_noipow: " << radd_noipow << "\n";
    std::cout << "radd_unvel: " << radd_unvel << "\n";
    std::cout << "radd_unrng: " << radd_unrng << "\n";
    std::cout << "radd_nfreq: " << radd_nfreq << "\n";
    std::cout << "radd_nipp: " << radd_nipp << "\n";
    std::cout << "radd_freq1: " << radd_freq1 << "\n";
    std::cout << "radd_freq2: " << radd_freq2 << "\n";
    std::cout << "radd_freq3: " << radd_freq3 << "\n";
    std::cout << "radd_freq4: " << radd_freq4 << "\n";
    std::cout << "radd_freq5: " << radd_freq5 << "\n";
    std::cout << "radd_ipp1: " << radd_ipp1 << "\n";
    std::cout << "radd_ipp2: " << radd_ipp2 << "\n";
    std::cout << "radd_ipp3: " << radd_ipp3 << "\n";
    std::cout << "radd_ipp4: " << radd_ipp4 << "\n";
    std::cout << "radd_ipp5: " << radd_ipp5 << "\n";

    std::cout << "frib_rxgain: ";
    for (int i = 0; i < 5; i++)
        std::cout <<frib_rxgain[i] << " ";
    std::cout << "\n";

    std::cout << "frib_xgain: " << frib_xgain << "\n";
    std::cout << "frib_lnalos: " << frib_lnalos << "\n";
    std::cout << "frib_tsgat: " << frib_tsgat << "\n";
    std::cout << "frib_frqgat: " << frib_frqgat << "\n";

    std::cout << "cspd_width: ";
    for (int i = 0; i < 6; i++)
        std::cout << cspd_width[i] << " ";
    std::cout << "\n";

    std::cout << "parm_dbz_scale: " << parm_dbz_scale << "\n";
    std::cout << "parm_dbz_bias: " << parm_dbz_bias << "\n";
    std::cout << "parm_sw_scale: " << parm_sw_scale << "\n";
    std::cout << "parm_sw_bias: " << parm_sw_bias << "\n";
    std::cout << "parm_ncp_scale: " << parm_ncp_scale << "\n";
    std::cout << "parm_ncp_bias: " << parm_ncp_bias << "\n";
    std::cout << "parm_vs_scale: " << parm_vs_scale << "\n";
    std::cout << "parm_vs_bias: " << parm_vs_bias << "\n";
    std::cout << "parm_vl_scale: " << parm_vl_scale << "\n";
    std::cout << "parm_vl_bias: " << parm_vl_bias << "\n";
    std::cout << "parm_vr_scale: " << parm_vr_scale << "\n";
    std::cout << "parm_vr_bias: " << parm_vr_bias << "\n";

}
