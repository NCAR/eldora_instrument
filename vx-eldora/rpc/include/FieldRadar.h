/*
 *	$Id$
 *
 *	Module:		 FieldRadar.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/08/30  18:39:23  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCFieldRadarh
#define INCFieldRadarh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct field_radar_i {
    char  field_radar_info[4];	/* Identifier for a field written */
				/* radar information block */
				/* (ascii characters FRIB). */
    long field_radar_info_len;	/* Length of this field written radar */
				/* information block in bytes. */
    short data_sys_id;		/* Data system identification. */
    short signal_source;	/* Signal source. */
    float freqs[5];		/* Transmitted frequencies for f0 */
				/* through f4 in Ghz. */
    float loss_out;		/* Losses between Transmitter and */
				/* antenna in db. */
    float loss_in;		/* Losses between antenna and Low */
				/* noise amplifier in db. */
    float ant_loss;		/* Losses in the antenna itself in db. */
    float sys_loss_0;		/* Other not yet defined system losses */
				/* #0 in db. */
    float sys_loss_1;		/* Other not yet defined system losses */
				/* #1 in db. */
    float sys_loss_2;		/* Other not yet defined system losses */
				/* #2 in db. */
    float sys_loss_3;		/* Other not yet defined system losses */
				/* #3 in db. */
    float ant_v_dim;		/* Antenna Vertical Dimension in m. */
    float ant_h_dim;		/* Antenna Horizontal Dimension in m. */
    float aperture_eff;		/* Aperture Efficiency. */
    float bandwidths[5];	/* Receiver bandwidths for frequencies */
				/* f0 through f4 in Mhz. */
    char  filter_num[8];	/* If signal processor filter number */
				/* being used. */
    float bessel_correct;	/* Bessel Filter Correction Factor. */
    float ant_noise_temp;	/* Antenna Noise Temperature in degrees K. */
    float r_noise_figure;	/* Receiver noise figure in ?. */
    short dly_tube_antenna;	/* Time delay from RF being applied to */
				/* tube and energy leaving antenna in ns. */
    short dly_antenna_dircplr;	/* Time delay from energy entering */
				/* antenna and it reaching the test */
				/* pulse directional coupler in ns. */
    short dly_dircplr_ad[5];	/* Time delay from the test pulse. */
				/* directional coupler to the A/D */
				/* converters in ns. */
    short padding1;
    short dly_timmod_testpulse; /* Time delay from timeing Module test */
				/* pulse edge and test pulse being */
				/* injected into directional coupler in ns. */
    short dly_modulator_on;	/* Modulator rise time (Time between */
				/* modulator on in timing module and */
				/* modulator full up in the high power */
				/* amplifier) in ns. */
    short dly_modulator_off;	/* Modulator fall time (Time between */
				/* end last chip in the timing module */
				/* and modulator full off) in ns. */
    short dly_rf_twt_on;	/* Time between a chip edge in timing */
				/* module and chip out of the TWT in ns. */
    short dly_rf_twt_off;	/* Time between a chip off edge in */
				/* timing module and chip actually off */
				/* out of the TWT in ns. */
    short padding4;
}; /* End of Structure */


#ifdef OK_RPC 

typedef struct field_radar_i field_radar_i;
typedef struct field_radar_i FIELDRADAR;

bool_t xdr_field_radar_i(XDR *, FIELDRADAR *);

#endif /* OK_RPC */

#endif /* INCFieldRadarh */
