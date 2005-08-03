/*
 *	$Id$
 *
 *	Module:		 InSitu.h
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/07/28  17:25:10  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCInSituh
#define INCInSituh

struct insitu_parameter {
    char  name[8];
    char  units[8];
};

struct insitu_descript {
    char  insitu_descript_id[4]; /* Identifier = SITU. */
    long  insitu_descript_len;	/* Block size in bytes. */
    long  number_params;	/* Number of paramters. */
    struct insitu_parameter params[256]; /* Is this enough? */
};

typedef struct insitu_descript insitu_descript;
typedef struct insitu_descript INSITUDESC;

typedef struct insitu_parameter insitu_parameter;
typedef struct insitu_parameter INSITU_PARAMETER;

#endif /* INCInSituh */

