/*
 *	$Id$
 *
 *	Module:		 tapeCtrlXdr.c
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1994/09/02  18:20:32  thor
 * Major rewrite.
 *
 * Revision 1.2  1992/01/14  15:22:07  thor
 * Added code for data reduction.
 *
 * Revision 1.1  1991/08/28  19:48:46  thor
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define UNIX
#include "tapeControl.h"

bool_t xdr_TapeCommand(XDR *xdrs,TapeCommand *objp)
{
    if (!xdr_u_long(xdrs,&objp->cmd))
      {
          return (FALSE);
      }
    return (TRUE);
}

bool_t xdr_TapeStatus(XDR *xdrs,TapeStatus *objp)
{
    register long *buf;

    int i;

    if (xdrs->x_op == XDR_ENCODE)
        {
            buf = XDR_INLINE(xdrs,(2 + (2 + 2 + 2 + 2 + 2 + 2)) *
                             BYTES_PER_XDR_UNIT);
            if (buf == NULL)
                {
                    if (!xdr_vector(xdrs,(char *)objp->drives0,2,
                                    sizeof (u_long),(xdrproc_t)xdr_u_long))
                      return (FALSE);
                    if (!xdr_vector(xdrs,(char *)objp->drives1,2,
                                    sizeof (u_long),(xdrproc_t)xdr_u_long))
                      return (FALSE);
                    if (!xdr_vector(xdrs,(char *)objp->number_of_drives,2,
                                    sizeof (u_long),(xdrproc_t)xdr_u_long))
                      return (FALSE);
                    if (!xdr_vector(xdrs,(char *)objp->status,2,
                                    sizeof (u_long),(xdrproc_t)xdr_u_long))
                      return (FALSE);
                    if (!xdr_vector(xdrs,(char *)objp->failures,2,
                                    sizeof (u_long),(xdrproc_t)xdr_u_long))
                      return (FALSE);
                    if (!xdr_vector(xdrs,(char *)objp->attempts,2,
                                    sizeof (u_long),(xdrproc_t)xdr_u_long))
                      return (FALSE);
                    if (!xdr_int(xdrs,&objp->eot_warning))
                      return (FALSE);
                    if (!xdr_int(xdrs,&objp->unit))
                      return (FALSE);
                }
            else
                {
                    {
                        register u_long *genp;

                        for (i = 0,genp = objp->drives0;
                             i < 2; i++)
                            {
                                IXDR_PUT_U_LONG(buf,*genp++);
                            }
                    }
                    {
                        register u_long *genp;

                        for (i = 0, genp = objp->drives1;
                             i < 2; i++)
                            {
                                IXDR_PUT_U_LONG(buf,*genp++);
                            }
                    }
                    {
                        register u_long *genp;

                        for (i = 0, genp = objp->number_of_drives;
                             i < 2; i++)
                            {
                                IXDR_PUT_U_LONG(buf,*genp++);
                            }
                    }
                    {
                        register u_long *genp;

                        for (i = 0, genp = objp->status;
                             i < 2; i++)
                            {
                                IXDR_PUT_U_LONG(buf,*genp++);
                            }
                    }
                    {
                        register u_long *genp;

                        for (i = 0, genp = objp->failures;
                             i < 2; i++)
                            {
                                IXDR_PUT_U_LONG(buf,*genp++);
                            }
                    }
                    {
                        register u_long *genp;

                        for (i = 0, genp = objp->attempts;
                             i < 2; i++)
                            {
                                IXDR_PUT_U_LONG(buf,*genp++);
                            }
                    }
                    IXDR_PUT_LONG(buf,objp->eot_warning);
                    IXDR_PUT_LONG(buf,objp->unit);
                }
            return (TRUE);
        }
    else if (xdrs->x_op == XDR_DECODE)
        {
            buf = XDR_INLINE(xdrs,(2 + (2 + 2 + 2 + 2 + 2 + 2)) *
                             BYTES_PER_XDR_UNIT);
            if (buf == NULL)
                {
                    if (!xdr_vector(xdrs,(char *)objp->drives0,2,
                                    sizeof (u_long),(xdrproc_t)xdr_u_long))
                      return (FALSE);
                    if (!xdr_vector(xdrs,(char *)objp->drives1,2,
                                    sizeof (u_long),(xdrproc_t)xdr_u_long))
                      return (FALSE);
                    if (!xdr_vector(xdrs,(char *)objp->number_of_drives,2,
                                    sizeof (u_long),(xdrproc_t)xdr_u_long))
                      return (FALSE);
                    if (!xdr_vector(xdrs,(char *)objp->status,2,
                                    sizeof (u_long),(xdrproc_t)xdr_u_long))
                      return (FALSE);
                    if (!xdr_vector(xdrs,(char *)objp->failures,2,
                                    sizeof (u_long),(xdrproc_t)xdr_u_long))
                      return (FALSE);
                    if (!xdr_vector(xdrs,(char *)objp->attempts,2,
                                    sizeof (u_long),(xdrproc_t)xdr_u_long))
                      return (FALSE);
                    if (!xdr_int(xdrs,&objp->eot_warning))
                      return (FALSE);
                    if (!xdr_int(xdrs,&objp->unit))
                      return (FALSE);
                }
            else
                {
                    {
                        register u_long *genp;

                        for (i = 0, genp = objp->drives0;
                             i < 2; i++)
                            {
                                *genp++ = IXDR_GET_U_LONG(buf);
                            }
                    }
                    {
                        register u_long *genp;

                        for (i = 0, genp = objp->drives1;
                             i < 2; i++)
                            {
                                *genp++ = IXDR_GET_U_LONG(buf);
                            }
                    }
                    {
                        register u_long *genp;

                        for (i = 0, genp = objp->number_of_drives;
                             i < 2; i++)
                            {
                                *genp++ = IXDR_GET_U_LONG(buf);
                            }
                    }
                    {
                        register u_long *genp;

                        for (i = 0, genp = objp->status;
                             i < 2; i++)
                            {
                                *genp++ = IXDR_GET_U_LONG(buf);
                            }
                    }
                    {
                        register u_long *genp;

                        for (i = 0, genp = objp->failures;
                             i < 2; i++)
                            {
                                *genp++ = IXDR_GET_U_LONG(buf);
                            }
                    }
                    {
                        register u_long *genp;

                        for (i = 0, genp = objp->attempts;
                             i < 2; i++)
                            {
                                *genp++ = IXDR_GET_U_LONG(buf);
                            }
                    }
                    objp->eot_warning = IXDR_GET_LONG(buf);
                    objp->unit = IXDR_GET_LONG(buf);
                }
            return (TRUE);
        }

    if (!xdr_vector(xdrs,(char *)objp->drives0,2,
                    sizeof (u_long),(xdrproc_t)xdr_u_long))
      return (FALSE);
    if (!xdr_vector(xdrs,(char *)objp->drives1,2,
                    sizeof (u_long),(xdrproc_t)xdr_u_long))
      return (FALSE);
    if (!xdr_vector(xdrs,(char *)objp->number_of_drives,2,
                    sizeof (u_long),(xdrproc_t)xdr_u_long))
      return (FALSE);
    if (!xdr_vector(xdrs,(char *)objp->status,2,
                    sizeof (u_long),(xdrproc_t)xdr_u_long))
      return (FALSE);
    if (!xdr_vector(xdrs,(char *)objp->failures,2,
                    sizeof (u_long),(xdrproc_t)xdr_u_long))
      return (FALSE);
    if (!xdr_vector(xdrs,(char *)objp->attempts,2,
                    sizeof (u_long),(xdrproc_t)xdr_u_long))
      return (FALSE);
    if (!xdr_int(xdrs,&objp->eot_warning))
      return (FALSE);
    if (!xdr_int(xdrs,&objp->unit))
      return (FALSE);
    return (TRUE);
}
