
struct sweepinfo_d {
    char sweep_des[4];	      /* Comment descriptor identifier: ASCII */
			      /* characters "COMM" stand for Comment */
			      /* Descriptor. */
    long  sweep_des_length;   /* Comment descriptor length in bytes. */
    char  radar_name[8];      /* comment*/
    long  sweep_num;          /*Sweep number from the beginning of the volume*/
    long  num_rays;            /*number of rays recorded in this sweep*/
    float start_angle;         /*true start angle [deg]*/
    float stop_angle;          /*true stop angle  [deg]*/

}; /* End of Structure */



typedef struct sweepinfo_d swepinfo_d;
typedef struct sweepinfo_d SWEEPINFO;

