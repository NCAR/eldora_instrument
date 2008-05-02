/*
 * $Id$
 *
 * Module: vmevmeFunc.h  
 * Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 * Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log: vmevmeFunc.h,v $
 * Revision 1.8  2005/08/04 22:27:39  granger
 * commit vx/hskp as copied from /net/eldora/eldora, except a few obsolete
 * (afaik) directories were removed, like hskp/src/clock/newclk and
 * hskp/src/arinc_NCAR
 *
 * Revision 1.7  1996/02/21  17:21:35  craig
 * *** empty log message ***
 *
 * Revision 1.6  1993/09/20  16:39:15  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/19  19:35:34  craig
 * Initial revision
 *
 *        
 */

/* Functions used with the vme to vme interfaces */

scope void init_vmevme(void);
scope void stop_vmevme(void);
scope void start_vmevme(void);
scope void vmevme_isr(void);
scope void fill_nav_info(int);
scope void fill_platform(long);
scope void vmevme_menu(void);
scope void midbeam(void);
//Tom 3/20/08
scope void init_info_blocks(void);
//scope void SendUDP(char* outstring);
scope int openOutSocket(void);
scope void makeAddr(struct sockaddr_in *addr, char* host, int port);

