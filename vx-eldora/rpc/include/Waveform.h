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
#ifndef OK_RPC
    short gate_dist1<4096>;	/* Distance from radar to data cell #1 */
				/* in 60 MHz counts. */
    short gate_dist2<4096>;	/* Ditto. */
    short gate_dist3<4096>;	/* Ditto. */
    short gate_dist4<4096>;	/* Ditto. */
    short gate_dist5<4096>;	/* Ditto. */
#else
    struct {
	u_int gate_dist1_len;	/* This MUST equal the value in */
				/* num_gates[n]. */
	short *gate_dist1_val;
    } gate_dist1;
    struct {
	u_int gate_dist2_len;
	short *gate_dist2_val;
    } gate_dist2;
    struct {
	u_int gate_dist3_len;
	short *gate_dist3_val;
    } gate_dist3;
    struct {
	u_int gate_dist4_len;
	short *gate_dist4_val;
    } gate_dist4;
    struct {
	u_int gate_dist5_len;
	short *gate_dist5_val;
    } gate_dist5;
#endif /* OK_RPC */
}; /* End of Structure */


struct wave_counts {
    int index;
    int offset;
    int count;
    short counts[512];
};


#ifdef OK_RPC

typedef struct waveform_d waveform_d;
typedef struct waveform_d WAVEFORM;

typedef struct wave_counts wave_counts;
typedef struct wave_counts WAVECOUNTS;

bool_t xdr_waveform_d(XDR *, WAVEFORM *);
bool_t xdr_wave_counts(XDR *, WAVECOUNTS *);

#endif /* OK_RPC */

#endif /* INCWaveformh */

