/*****************************************************************************
******************************************************************************
*                                                                            *
*     NNNNNN         N                A             TTTTTTT TTTTTTTTTT       *
*     N NNNNN        N               AAA                  T TTTTT            *
*     NN NNNNN       N              AAAAA                 T TTTTT            *
*     N N NNNNN      N             A AAAAA                T TTTTT            *
*     N  N NNNNN     N            A A AAAAA               T TTTTT            *
*     N   N NNNNN    N           A   A AAAAA              T TTTTT            *
*     N    N NNNNN   N          A     A AAAAA             T TTTTT            *
*     N     N NNNNN  N         AAAAAAAAA AAAAA            T TTTTT            *
*     N      N NNNNN N        A         A AAAAA           T TTTTT            *
*     N       N NNNNNN       A           A AAAAA          T TTTTT            *
*     N        N NNNNN  OO  A             A AAAAA  OO     T TTTTT     OO     *
*                       OO                         OO                 OO     *
*                                                                            *
*     Gesellschaft fuer Netzwerk- und Automatisierungstechnologie m.b.H      *
*        Kamillenweg 22, D-53757 Sankt Augustin, Tel.: ++49-2241/3989-0      *
*                                                                            *
******************************************************************************
******************************************************************************
*
* Module      : nif_c.c
*
* Description : Masterboard Network Interface Routines
*
* Author      : H.Koerte
*
******************************************************************************
******************************************************************************
*
*                    Copyright (c) by N.A.T. GmbH
*
*       All rights reserved. Copying, compilation, modification,
*       distribution or any other use whatsoever of this material
*       is strictly prohibited except in accordance with a Software
*       License Agreement with N.A.T. GmbH.
*
******************************************************************************
******************************************************************************
*
* $ProjectRevision$
* $Source$
* $State$
* $Revision$ $Name$
* $Date$
* $Locker$
*
* Module's revision history:
* ==========================
*
* --------- $Log$
* 030328 hk Corrected bug with nif_mget(): it now returns an mbuf or NULL and
*		does no longer take an mbuf's address as parameter.
* 030327 hk Added NAT header.
*	    Corrected known bug in action(): nif_setACTH2S() was called
*		with wrong parameter!
*	    Corrected known bug in nif_getActS2H() with passed parameter.
*		Instead of modifying passed parameter nif_getActS2H() now
*		returns a valid mbuf or NULL else.
* 961219 hk Initial version for CUSTOMIZE, adapted from nif_a.a
******************************************************************************
******************************************************************************/

#include <natdefs.h>
#include <nif.h>
#include <mbuf.h>
#include <hwconfig.h>
#include <customize.h>			/* CUSTOMIZE here */

#define MAX_WAIT 100

extern u_char *V_Nif;
extern u_char *V_Port;			/* address of module on bus */
extern u_char *V_Mbox;			/* address of module's mbox cell */


/*-------------------------------------------------------------------------*/
FUNCTION static void no_op()		     /* does what is ought to do   */
/*-------------------------------------------------------------------------*/
{
}

/*--------------------------------------------------------------------------*/
FUNCTION void nif_wait()
/*--------------------------------------------------------------------------*/
{
	u_long i1;

	for (i1 = 0; i1 < 1000; i1++)
		 no_op();
}

/*--------------------------------------------------------------------------*/
FUNCTION struct mbuf *get_mbuf()
/*--------------------------------------------------------------------------*/
{
	register struct ls_softc *ls;
	register u_long i1;
	register struct mbuf *m;
	u_short irq_mask;

	i1 = 0;
	mask_irq(&irq_mask);

	for(;;) {
		m = nif_mget();
		if (m) {
			enab_irq(&irq_mask);
			return (m);
		} else {
			if (i1 == MAX_WAIT) {
					enab_irq(&irq_mask);
					return((struct mbuf *)0);
			}
			nif_wait();
			i1++;
		}
	}
}

/*--------------------------------------------------------------------------*/
FUNCTION char *get_page()
/*--------------------------------------------------------------------------*/
{
	register u_long i1;
	register u_char *page;
	u_short irq_mask;

	i1 = 0;
	mask_irq(&irq_mask);

	for(;;) {
		nif_pget(page);
		if (page) {
			enab_irq(&irq_mask);
			return (page);
		} else {
			if (i1 == MAX_WAIT) {
					enab_irq(&irq_mask);
					return((u_char*)0);
			}
			nif_wait();
			i1++;
		}
	}
}

/*--------------------------------------------------------------------------*/
FUNCTION int put_mbuf(m)
/*--------------------------------------------------------------------------*/
	register struct mbuf *m;
{
	register u_long i1;
	u_short irq_mask;
	u_long error;

	i1 = 0;
	error = 0;

	mask_irq(&irq_mask);

	for(;;) {
		error = nif_mput(m);
		if (error == 0) {
			enab_irq(&irq_mask);
			return(error);
		} else {
			if (i1 == MAX_WAIT) {
					enab_irq(&irq_mask);
					return(error);
			}
			nif_wait();
			i1++;
		}
	}
}

/*--------------------------------------------------------------------------*/
FUNCTION int put_page(page)
/*--------------------------------------------------------------------------*/
	register char *page;
{
	register u_long i1;
	u_short irq_mask;
	u_long error;

	i1 = 0;
	error = 0;

	mask_irq(&irq_mask);

	for(;;) {
		error = nif_pput(page);
		if (error == 0) {
			enab_irq(&irq_mask);
			return (error);
		} else {
			if (i1 == MAX_WAIT) {
					enab_irq(&irq_mask);
					return(error);
			}
			nif_wait();
			i1++;
		}
	}
}

/*--------------------------------------------------------------------------*/
FUNCTION int action(m)
/*--------------------------------------------------------------------------*/
	register struct mbuf *m;
{
	register u_long i1;
	u_short irq_mask;
	u_long error;

	i1 = 0;
	error = 0;

	mask_irq(&irq_mask);

	for(;;) {
		error = nif_setACTH2S(m, IRQ_ACTH2S);
		if (error == 0) {
			enab_irq(&irq_mask);
			return(0);
		} else {
			if (i1 == MAX_WAIT) {
					enab_irq(&irq_mask);
					return(-1);
			}
			nif_wait();
			i1++;
		}
	}
}

/*--------------------------------------------------------------------------*/
FUNCTION struct mbuf* nif_mget()
/*--------------------------------------------------------------------------*/
{
	NIF *nif;
	struct mbuf *m;

	nif = (NIF *)V_Nif;
	m = nif->mget;

	if (m) {
		nif->mget = (struct mbuf *)0;
		m = (struct mbuf*)(((u_long)m & (MAX_MPRAM - 1)) + V_Port);
		if (test_mbox())
			return((struct mbuf *)NULL);
		do_mbox(IRQ_MGET);
		return(m);
	}
	return((struct mbuf *)NULL);
}

/*--------------------------------------------------------------------------*/
FUNCTION int nif_mput(m)
/*--------------------------------------------------------------------------*/
	struct mbuf *m;
{
	NIF *nif;

	nif = (NIF *)V_Nif;

	if (nif->mput)
		return(-1);

	if (test_mbox())
		return(-1);

	nif->mput = m;
	do_mbox(IRQ_MPUT);

	return(0);
}

/*--------------------------------------------------------------------------*/
FUNCTION int nif_pget(p)
/*--------------------------------------------------------------------------*/
        char *p;
{
        NIF *nif;

        nif = (NIF *)V_Nif;
        p = nif->pget;

        if (p) {
                nif->pget = (char *)0;
                p = (char *)(((u_long)p & (MAX_MPRAM - 1)) + V_Port);
                if (test_mbox())
                        return(-1);
                do_mbox(IRQ_PGET);
                return(0);
        }
        return(-1);
}

/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
FUNCTION int nif_pput(p)
/*--------------------------------------------------------------------------*/
	char *p;
{
	NIF *nif;

	nif = (NIF *)V_Nif;

	if (nif->pput)
		return(-1);

	if (test_mbox())
		return(-1);

	nif->pput = p;

	do_mbox(IRQ_PPUT);

	return(0);
}

/*--------------------------------------------------------------------------*/
FUNCTION struct mbuf *nif_getACTS2H()
/*--------------------------------------------------------------------------*/
{
	struct mbuf *m;
	NIF *nif;

	nif = (NIF *)V_Nif;
	m = nif->ActS2H;

	if (m) {
		nif->ActS2H = (struct mbuf *)0;
		m = (struct mbuf*)(((u_long)m & (MAX_MPRAM - 1)) + V_Port);
		return(m);
	}
	return((struct mbuf *)NULL);
}

/*--------------------------------------------------------------------------*/
FUNCTION int nif_setACTH2S(m, irq_code)
/*--------------------------------------------------------------------------*/
	struct mbuf *m;
	u_char irq_code;
{
	NIF *nif;

	nif = (NIF *)V_Nif;
	if (m) {
		if (nif->ActH2S)
			return(-1);
		if (test_mbox())
			return(-1);
		nif->ActH2S = m;
		do_mbox(irq_code);
		return(0);
	}
	return(-1);
}

/*--------------------------------------------------------------------------*/
FUNCTION int test_mbox()
/*--------------------------------------------------------------------------*/
{
	if (*V_Mbox)
		return(-1);
	return(0);
}

/*--------------------------------------------------------------------------*/
FUNCTION int do_mbox(irq_code)
/*--------------------------------------------------------------------------*/
	u_char irq_code;
{
	*V_Mbox = irq_code;
	return(0);
}

