#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/uio.h>

#define OK_RPC
#include "Volume.h"
#include "RadarDesc.h"
#include "Parameter.h"
#include "Platform.h"
#include "CellSpacing.h"
#include "Ray.h"
#include "Waveform.h"
#include "FieldRadar.h"
#include "NavDesc.h"
#include "InSitu.h"

#define MAX_RADAR   2
#define MAX_PARAM   10
#define MAX_HPA     5
#define MAX_RCVR    2
#define MAX_ANTENNA 3
#define MAX_PSCHEME 16
#define FORE   0
#define AFT    1

struct hpa_s{
    char serial_number[8];    /* Serial number of HPA plus QW or ASE to
                                 determine the manufacturer */
    float peak_power[6];      /* Nominal peak transmitted power in kw by
                                 transmitted frequency (i.e. 9.3, 9.4, 9.5,
                                 9.6, 9.7, 9.8 GHz) */
    float x_band_gain[6];           /* Gain from waveguide to output SMA of the
                                 amplifier, by frequency - should have the
                                 testpulse coupling removed (dB) */
    float turn_on_time;       /* Turn on time in ns */
    float turn_off_time;      /* Turn off time in ns */
    float peak_power_coupling; /* Coupling to peak power meter head (dB) */
    float test_pulse_coupling; /* Coupling from the testpulse input SMA to
				 the waveguide (dB) */
    float r_noise_figure;     /* receiver noise figure */
    float tr_tube_loss;       /* Insertion loss of the TR tube in dB */
};

struct rcvr_s{
    float cntr_freq;           /* Center transmitted frequency in GHz */
    float receiver_gain[5];   /* Gain from the HPA output to the output of the
				 receiver exciter chassis NOTE: this should
                                 include the cable from the HPA to the receiver
                                 exciter chassis (by channel in dB) Note: this
				 gain is frequency dependent and must be
				 measured to correspond with the frequencies
                                 given in */
    float if_gain[5];         /* Gain from the receiver exciter output SMA to
                                 the IF output (the anti-aliasing filtered 
				 4MHz signal into the digitizers) in dB */
    float delay_chip_rcvrs;   /* delay from TTL chip on in timing modules to
				 chips into the A/D converters with the RF
				 input attached to forward #1 in nanosecs */
    float testpulse_delay;    /* Delay (ns) between testpulse being turned
				 on in the timing module and the testpulse
				 actually appearing at the A/D converter */
    int freq_indexs[5];       /* Index into arrays that are set up based on
			         the transmitted frequency of the
                                 given channel */
};

struct attenuator{
    float loss_rawtp_hpa; /* Loss through the attenuator chassis from the
                               raw testpulse input to the HPA input NOTE:
                               this should include the cable from the
                               attenuator chassis to the HPA and the
			       attenuator must be set to zero (in dB) */
   float loss_rawtp_powermeter;  /* Loss through the attenuator chassis 
				       from the raw test pulse input to the
				       rear connector where the power meter
				       is attached */
};

struct antenna_s{
    float gain[6];       /*As measured by LCTAR, by transmitted freq (in dB)*/
    float hbeam_width[6]; /*As measured by LCTAR, by transmitted freq (in dB)*/
    float vbeam_width[6]; /*As measured by LCTAR, by transmitted freq (in dB)*/
    float E_plane_angle[6]; /*As measured by LCTAR, by transmitted freq (deg)*/
    float H_plane_angle[6];/*As measured by LCTAR, by transmitted freq (deg)*/
    float waveguide_length; /* As measured by NCAR staff (meters) */
    float encoder_antenna_up; /* Encoder reading minus roll angle when antenna
                                 is horizontal pointing up */
    float pitch_antenna_up;   /* Measure pitch angle minus pitch angle when
				 antenna is horizontal pointing up */
};

struct param_units
{
    char name[8];
    char units[8];
    char description[40];
};

struct pscheme_s
{
    char  name[16];	        /* Pulsing scheme name.*/
    short num_chips[6];		/* Number of chips in a repeat
				   sequence for each frequency. */
    char  blank_chip[256];	/* Blanking RAM sequence. */
    short total_pcp;		/* Total Number of PCP in a repeat sequence. */
    float ur_pcp;		/* Number of PCP that set the
				   unambiguous range, after real time
				   unfolding. */
    float uv_pcp;		/* Number of PCP that set the
				   unambiguous velocity, after real
				   time unfolding. */
    float samples_in_repeat;    /* Number of samples per repeat sequence */
    int pcp_sets_prf;           /* Number of pcp to set the prf */
    int num_params;             /* Number of parameters with this scheme */
    struct param_units param[MAX_PARAM];  /* Names and units of parameters */
    float chip_percent[5];      /* percent of pulse that has occured when
                                   chip starts */
    int chips_in_pulse;         /* Number of chips that make up the pulse */
    int numipps;                /* Number of interpulse periods (ipps) in use*/
    int pcp_per_ipp[5];         /* Number of Pulse Clock Periods (PCP) in each
				   ipp that is being used */
};

/* Engineering window parameters */

struct engineer{

    int schemenum;    /* Pulsing/sampling scheme number */
    int numsamples;   /* Number of samples (per frequency) in
			 repeat sequence */
    float prf;        /* User requested PRF in hertz */
    int numgates;     /* Number of gates to be sampled */
    float chipdur;    /* Duration of the chips in micro-seconds */
    float first_gate_dis;  /* Distance to the first gate in meters */
    int forehpa_snum;      /* Fore HPA serial number */
    int afthpa_snum;       /* Aft HPA serial number */
    float rot_spacing;     /* Rotational spacing of the beams in degrees */
    float avg_freq[2];     /* Average transmitted frequency (GHZ)*/ 
    int avg_freq_index[2]; /* Index into arrays that are set up based on
			      the transmitted frequency of the given channel
			      The is for avg_freq */
};


