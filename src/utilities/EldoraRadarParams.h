#ifndef ELDORARADARPARAMS_H_
#define ELDORARADARPARAMS_H_

/// Eldora radar operating parameters. These are the 
/// parameters necessary to configure and run the 
/// Eldora hardware. They can be extracted from the old
/// Eldora header. The naming convention is taken from 
/// headermaker, which assigns keys to each header parameter.
struct EldoraRadarParams
{
    EldoraRadarParams();
        // fields from the header waveform block (WAVE)
        int    wave_nchips[6];///< waveform number of chips
        int    wave_chpoff[6];///< waveform chip offset (60 Mhz counts)
        int    wave_chpwid[6];///< waveform chip width (60 Mhz counts)
        int    wave_ngates[5];///< waveform number of sampled gates
        int    wave_gate1[2]; ///< waveform gate distance 1 (60 Mhz counts)
        int    wave_gate2[2]; ///< waveform gate distance 2 (60 Mhz counts)
        int    wave_gate3[2]; ///< waveform gate distance 3 (60 Mhz counts)
        int    wave_gate4[2]; ///< waveform gate distance 4 (60 Mhz counts)
        int    wave_gate5[2]; ///< waveform gate distance 5 (60 Mhz counts)
        double wave_msrep;    ///< waveform number of milliseconds per repeat
        int    wave_seqrep;   ///< waveform number of repeat sequences in a dwell time
        
        // fields from the header radar descriptor block (RADD)
        char   radd_name[4];  ///< radar name
        double radd_const;    ///< radar constant (db)
        double radd_noipow;   ///< noise power (dbm)
        double radd_unvel;    ///< radar effective unambiguous velocity
        double radd_unrng;    ///< radar effective unambiguous range
        int    radd_nfreq;    ///< radar number of tx frequencies
        int    radd_nipp;     ///< radar number of diff ipps
        double radd_freq1;    ///< radar feequency 1 (GHz)
        double radd_freq2;    ///< radar feequency 2 (GHz)
        double radd_freq3;    ///< radar feequency 3 (GHz)
        double radd_freq4;    ///< radar feequency 4 (GHz)
        double radd_freq5;    ///< radar feequency 5 (GHz)   
        double radd_ipp1;     ///< radar interpulse period 1 (ms)
        double radd_ipp2;     ///< radar interpulse period 2 (ms)
        double radd_ipp3;     ///< radar interpulse period 3 (ms)
        double radd_ipp4;     ///< radar interpulse period 4 (ms)
        double radd_ipp5;     ///< radar interpulse period 5 (ms)
        
        // fields from the header field radar info block (FRIB)
        double frib_rxgain[5];///< field radar receiver gain by channel (db)
        double frib_xgain;    ///< field radar x band gain (db)
        double frib_lnalos;   ///< field radar losses between antenna and lna (db)
        int    frib_tsgat;    ///< field radar time series gate
        int    frib_frqgat;   ///< field radar number of time series gate (this field hijacked from independent frequency gate parameter) 
        
        double parm_dbz_scale;///< scale for dbz
        double parm_dbz_bias; ///< bias for dbz
        
        double parm_sw_scale; ///< scale for sw
        double parm_sw_bias;  ///< bias for sw
        
        double parm_ncp_scale;///< scale for ncp
        double parm_ncp_bias; ///< bias for ncp
        
        double parm_vs_scale; ///< scale for vs
        double parm_vs_bias;  ///< bias for vs
        
        double parm_vl_scale; ///< scale for vl
        double parm_vl_bias;  ///< bias for vl
        
        double parm_vr_scale; ///< scale for vr
        double parm_vr_bias;  ///< bias for vr

};

#endif /*RADARPARAMS_H_*/
