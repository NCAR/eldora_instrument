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
 * Revision 1.3  1991/10/09  15:22:49  thor
 * Removed unneeded wave cunt items.
 *
 * Revision 1.2  1991/09/11  16:30:22  thor
 * Added items to handle wave_counts.
 *
 * Revision 1.1  1991/08/30  18:39:44  thor
 * Initial revision
 *
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
    parameter_d Params[10];
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

#define HeaderRPC ((u_long)0x30000100)
#define HeaderVers ((u_long)1)
#define SendHeader ((u_long)1)
#define SendCounts ((u_long)2)

#ifdef CLIENT_SIDE
extern void *sendheader_1(TAPEHEADER *, CLIENT *);
#else
extern void startHeader(void);
extern void *sendheader_1(TAPEHEADER *, struct svc_req *);
extern void headerrpc_1(struct svc_req *, register SVCXPRT *);
#endif /* CLIENT_SIDE */

#else

program HeaderRPC {
    version HeaderVers {
	void SendHeader(struct tapeHeader) = 1;
    } = 1;
} = 0x30000100;

#endif /* OK_RPC */

#endif /* INCTapeHeaderh */

