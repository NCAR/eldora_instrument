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
 * Revision 1.6  1994/08/30  15:19:48  thor
 * Dropped old rpc stuff.
 *
 * Revision 1.5  1992/07/28  17:33:44  thor
 * Added nav & insitu items.
 *
 * Revision 1.4  1991/10/22  17:07:48  thor
 * Changed to use 10 fixed PARAMETERs.
 *
 * Revision 1.4  1991/10/22  17:07:48  thor
 * Changed to use 10 fixed PARAMETERs.
 *
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

#include "CellSpacing.h"
#include "FieldRadar.h"
#include "Parameter.h"
#include "RadarDesc.h"
#include "Volume.h"
#include "Waveform.h"
#include "InSitu.h"
#include "NavDesc.h"

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
    struct nav_descript Nav;
    struct insitu_descript Insitu;
};


typedef struct sensorDesc sensorDesc;
typedef struct sensorDesc SENSORDESC;

typedef struct tapeHeader tapeHeader;
typedef struct tapeHeader TAPEHEADER;

#endif /* INCTapeHeaderh */

