/*
 *	$Id$
 *
 *	Module:		 TapeHeader.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        
 */
#ifndef INCTapeHeaderh
#define INCTapeHeaderh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#include "CellSpacing.h"
#include "FieldRadar.h"
#include "Parameter.h"
#include "RadarDesc.h"
#include "Volume.h"
#include "Waveform.h"

#endif /* OK_RPC */

struct sensorDesc {
    radar_d        Radar;
    field_radar_i  FieldInfo;
    cell_spacing_d CellSpacing;
#ifndef OK_RPC
    parameter_d    Params<>;
#else
    struct {
	u_int Params_len;
	PARAMETER *Params_val;
    } Params;
#endif /* OK_RPC */
};

struct tapeHeader {
    volume_d   Volume;
    waveform_d Wave;
    struct sensorDesc Fore;
    struct sensorDesc Aft;
};


#ifdef OK_RPC

typedef struct sensorDesc sensorDesc;
typedef struct sensorDesc SENSORDESC;

typedef struct tapeHeader tapeHeader;
typedef struct tapeHeader TAPEHEADER;

bool_t xdr_sensorDesc(XDR *, SENSORDESC *);
bool_t xdr_tapeHeader(XDR *, TAPEHEADER *);

#endif /* OK_RPC */

#endif /* INCTapeHeaderh */

