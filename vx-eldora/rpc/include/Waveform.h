/*
 *	$Id$
 *
 *	Module:		 Waveform.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1991/09/11  16:29:24  thor
 * Added wave_counts items.
 *
 * Revision 1.1  1991/08/30  18:39:42  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCWaveformh
#define INCWaveformh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct waveform_d {
    char  waveform_des[4];		/* Identifier for the waveform */
				/* descriptor (ascii characters "WAVE"). */
    short waveform_des_length;	/* Length of the waveform descriptor */
				/* in bytes. */
    short padding1;
    char  ps_file_name[16];		/* Pulsing scheme file name.*/
    short num_chips[6];		/* Number of chips in a repeat. */
				/* sequence for each frequency. */
    char  blank_chip[256];	/* Blanking RAM sequence. */
    float repeat_seq;		/* Number of milliseconds in a repeat */
				/* sequence in ms. */
    short repeat_seq_dwel;	/* Number of repeat sequences in a */
				/* dwell time. */
    short total_pcp;		/* Total Number of PCP in a repeat sequence. */
    short chip_offset[6];	/* Number of 60 Mhz clock cycles to */
				/* wait before starting a particular */
				/* chip in 60 MHz counts. */
    short chip_width[6];	/* Number of 60 Mhz clock cycles in */
				/* each chip in 60 MHz counts. */
    float ur_pcp;		/* Number of PCP that set the */
				/* unambiguous range, after real time */
				/* unfolding. */
    float uv_pcp;		/* Number of PCP that set the */
				/* unambiguous velocity, after real */
				/* time unfolding. */
    short num_gates[5];		/* Total number of gates sampled. */
    short padding2;    
    short uniform_gates;	/* Flag 0 => Non uniform gates, all */
				/* are listed, nonzero => uniform */
				/* gates. */
    short padding3;
    short gate_dist1[2];	/* Distance from radar to data cell #1 */
				/* in 60 MHz counts. */
    short gate_dist2[2];	/* Ditto. */
    short gate_dist3[2];	/* Ditto. */
    short gate_dist4[2];	/* Ditto. */
    short gate_dist5[2];	/* Ditto. */
};

#ifdef OK_RPC

typedef struct waveform_d waveform_d;
typedef struct waveform_d WAVEFORM;

bool_t xdr_waveform_d(XDR *, WAVEFORM *);

#endif /* OK_RPC */

#endif /* INCWaveformh */

