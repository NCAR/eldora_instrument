#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/uio.h>

#define OK_RPC
#define UNIX
#include "/home/thor/sherrie/perusal/readtape/read_include/Volume.h"
#include "/home/thor/sherrie/perusal/readtape/read_include/RadarDesc.h"
#include "/home/thor/sherrie/perusal/readtape/read_include/Parameter.h"
#include "/home/thor/sherrie/perusal/readtape/read_include/Platform.h"
#include "/home/thor/sherrie/perusal/readtape/read_include/CellSpacing.h"
#include "/home/thor/sherrie/perusal/readtape/read_include/Ray.h"
#include "/home/thor/sherrie/perusal/readtape/read_include/Waveform.h"
#include "/home/thor/sherrie/perusal/readtape/read_include/FieldRadar.h"
#include "/home/thor/sherrie/perusal/readtape/read_include/NavDesc.h"
#include "/home/thor/sherrie/perusal/readtape/read_include/InSitu.h"
#include "/home/thor/sherrie/perusal/readtape/read_include/InSituData.h"
#include "/home/thor/sherrie/perusal/readtape/read_include/MiniRIMS.h"

#define BUFSIZE  10000
#define MAXPARAMETERS 10


struct volume_header{
       VOLUME   volume;
       WAVEFORM waveform;
   };

struct sensorDes{
       RADARDESC    radar;
       FIELDRADAR   fradar;
       CELLSPACING  cell;
       PARAMETER    parameter[10];
   };

struct navDesc{
      NAVDESC      navagation;
  };

struct inSituDataDesc{
     INSITUDESC    insitudata;
 };


 
/*declare external structures*/
  struct volume_d volume;
  struct radar_d  radar;
  struct parameter_d parameter[10];
  struct nav_descript navagation;
  struct insitu_descript insitudata;
  struct cell_spacing_d cell;
  struct waveform_d waveform;
  struct field_radar_i  fradar;
  struct volume_header vheader;
  struct sensorDes     foresensordescriptor;  
  struct sensorDes     aftsensordescriptor;  
  struct navDesc       navagationdesc;
  struct inSituDataDesc insitudescriptor;



  int prf; /*PULSE REPITITION FREQUENCY*/
  int cells;
  int cellspacing;




