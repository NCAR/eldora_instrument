/*+ DEC/CMS REPLACEMENT HISTORY, Element ALIGN.H+*/
/*+ *6    18-OCT-1988 16:37:24 ECREEL "eec: adding declares for op_az and old_op_az and op_az_flag"+*/
/*+ *5    18-OCT-1988 16:10:43 ECREEL "eec: adding external declarations for saved_tdxgb, ...ygb, ...zgb"+*/
/*+ *4    28-JUL-1988 09:38:10 JRFELLE "no changes"+*/
/*+ *3    23-MAY-1988 12:19:24 ECREEL "rwed prot set by eec"+*/
/*+ *2    20-MAY-1988 15:12:37 ECREEL "no mods -- used only for compiling purposes by eec"+*/
/*+ *1     7-MAR-1988 15:45:30 JRFELLE ""+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element ALIGN.H+*/
/* 10/30/87  10:50
 * NAME:  align.h
 *
 * NAV alignment variable definitions common to:
 *	f_align.c
 *	i_align.c
 *	s_align.c
 */


#define COURSE	1
#define FINE	2
#define NN	9
#define T_ALIGN_SW 30.0
#define DT_UPDT 1.0


#define M_NOISEN2 9.00e-04
#define M_NOISEE2 9.00e-04
#define S_GBX2 1.46e-12
#define S_GBY2 1.46e-12
#define S_GBZ2 1.46e-12
#define Q_GBX 6.52e-17
#define Q_GBY 6.52e-17
#define Q_GBZ 6.52e-17
#define Q_VN 4.00e-06
#define Q_VE 4.00e-06
#define Q_EN 1.6e-13
#define Q_EE 1.6e-13
#define Q_ED 1.6e-13

#define D_XGK_2 1.0e-08
#define D_YGK_2 1.0e-08
#define D_ZGK_2 1.0e-08
#define D_XGY_2 4.0e-08
#define D_XGZ_2 4.0e-08
#define D_YGX_2 4.0e-08
#define D_YGZ_2 4.0e-08
#define D_ZGX_2 4.0e-08
#define D_ZGY_2 4.0e-08

extern double	f[NN][NN];
extern double	kal_gn[NN][2];
extern double	p[NN][NN];
extern double	q[NN][NN];
extern double	r[NN][NN];
extern double   dph_x2, dph_y2, dph_z2;
extern double   dph_x_acc, dph_y_acc, dph_z_acc;
extern double   t_align;
extern double	t_up;
extern double	t_up_1;
extern double   dvn, dve;

extern double   op_az;

extern int	a_mode;
extern int	propagate;
extern int	update;
