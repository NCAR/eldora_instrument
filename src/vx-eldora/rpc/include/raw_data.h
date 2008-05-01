/*
 *      $Id$
 *
 *      Module:          raw_data.h
 *      Original Author: Rich Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/03/21  18:07:46  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */

#ifndef _RAW_DATA_H
#define _RAW_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

struct raw_data {
  char raw_des[4];              /* Descriptor id: "RAWD". */
  char x_label[8];
  float xmax;
  float xmin;
  char y_label[8];
  float ymax;
  float ymin;
  int numPoints;
};

typedef struct raw_data RAW_DATA;

#ifdef __cplusplus
};
#endif

#endif /* _RAW_DATA_H */
