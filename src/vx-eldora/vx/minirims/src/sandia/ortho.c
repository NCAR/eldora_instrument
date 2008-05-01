/*+ DEC/CMS REPLACEMENT HISTORY, Element ORTHO.C+*/
/*+ *1     7-MAR-1988 14:43:54 JRFELLE "orthonarmalization routine"+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element ORTHO.C+*/
/****************************************************************
* ortho.c							*
*****************************************************************

Description:  This routine is used to force direction cosine matricies
	      to be both orthogonal and normal.  It is a general
	      which is passed the pointer to the first element in
	      a 3 X 3 double precision matrix and then operates on
	      the matrix.

inputs:	      The A or T DCM first element address.
outputs:      ortho-normalized matrix elements in their original location.

notes:	      none

called routines:  none

*****************************************************************/

/* Last Modified:  4/20/87    BY:  E.J. Nava, 9131		*/

#define A11	0	/* 0 - offsets from start address - dbl precision */
#define A12	1	/* 8 - compiler multiplies values by 8 for offsets */
#define A13	2	/* 16			*/
#define A21	3	/* 24			*/
#define A22	4	/* 32			*/
#define A23	5	/* 40			*/
#define A31	6	/* 48			*/
#define A32	7	/* 56			*/
#define A33	8	/* 64			*/

ortho(matptr)
double *matptr;
{
  double ata11,ata12,ata13,ata21,ata22,ata23,ata31,ata32,ata33;
  double a11up,a12up,a13up,a21up,a22up,a23up,a31up,a32up,a33up;

  register double *mp;

  mp = matptr;		/* use register pointer for efficient addressing  */

  /* compute a`a-I */

  ata11 = (*(mp + A11) * *(mp + A11)) + (*(mp + A21) * *(mp + A21))
        + (*(mp + A31) * *(mp + A31)) - 1.0;

  ata12 = (*(mp + A11) * *(mp + A12)) + (*(mp + A21) * *(mp + A22))
        + (*(mp + A31) * *(mp + A32));

  ata13 = (*(mp + A11) * *(mp + A13)) + (*(mp + A21) * *(mp + A23))
        + (*(mp + A31) * *(mp + A33));

  ata21 = (*(mp + A12) * *(mp + A11)) + (*(mp + A22) * *(mp + A21))
        + (*(mp + A32) * *(mp + A31));

  ata22 = (*(mp + A12) * *(mp + A12)) + (*(mp + A22) * *(mp + A22))
	+ (*(mp + A32) * *(mp + A32)) - 1.0;

  ata23 = (*(mp + A12) * *(mp + A13)) + (*(mp + A22) * *(mp + A23))
	+ (*(mp + A32) * *(mp + A33));

  ata31 = (*(mp + A13) * *(mp + A11)) + (*(mp + A23) * *(mp + A21))
	+ (*(mp + A33) * *(mp + A31));

  ata32 = (*(mp + A13) * *(mp + A12)) + (*(mp + A23) * *(mp + A22))
	+ (*(mp + A33) * *(mp + A32));

  ata33 = (*(mp + A13) * *(mp + A13)) + (*(mp + A23) * *(mp + A23))
	+ (*(mp + A33) * *(mp + A33)) - 1.0;

/* compute a - 0.5 * a(a'a-I)  */

  a11up =  *(mp + A11) - 0.5 * ((*(mp + A11) * ata11)
	+ (*(mp + A12) * ata21) + (*(mp + A13) * ata31));

  a12up =  *(mp + A12) - 0.5 * ((*(mp + A11) * ata12)
	+ (*(mp + A12) * ata22) + (*(mp + A13) * ata32));

  a13up =  *(mp + A13) - 0.5 * ((*(mp + A11) * ata13)
	+ (*(mp + A12) * ata23) + (*(mp + A13) * ata33));

  a21up =  *(mp + A21) - 0.5 * ((*(mp + A21) * ata11)
	+ (*(mp + A22) * ata21) + (*(mp + A23) * ata31));

  a22up =  *(mp + A22) - 0.5 * ((*(mp + A21) * ata12)
	+ (*(mp + A22) * ata22) + (*(mp + A23) * ata32));

  a23up =  *(mp + A23) - 0.5 * ((*(mp + A21) * ata13)
	+ (*(mp + A22) * ata23) + (*(mp + A23) * ata33));

  a31up =  *(mp + A31) - 0.5 * ((*(mp + A31) * ata11)
	+ (*(mp + A32) * ata21) + (*(mp + A33) * ata31));

  a32up =  *(mp + A32) - 0.5 * ((*(mp + A31) * ata12)
	+ (*(mp + A32) * ata22) + (*(mp + A33) * ata32));

  a33up =  *(mp + A33) - 0.5 * ((*(mp + A31) * ata13)
	+ (*(mp + A32) * ata23) + (*(mp + A33) * ata33));

  *(mp + A11) = a11up;
  *(mp + A12) = a12up;
  *(mp + A13) = a13up;
  *(mp + A21) = a21up;
  *(mp + A22) = a22up;
  *(mp + A23) = a23up;
  *(mp + A31) = a31up;
  *(mp + A32) = a32up;
  *(mp + A33) = a33up;

}
