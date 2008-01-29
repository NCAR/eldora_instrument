/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Header.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.16  2005/08/03 20:20:56  granger
 * updates copied from the versions in the rpc/include directory, except for
 * Aircraft.h which looks it got accidentally replaced with the contents of
 * Parameter.h; some of these are commits of changes from
 * /net/eldora/eldora/rpc/header which were never committed to rcs; and some
 * files are being added to revision control for the first time
 *
 * Revision 1.15  1994/10/24  18:55:52  thor
 * Fixed broked << operator.
 *
 * Revision 1.14  1994/08/30  15:19:03  thor
 * Lots of bug fixes & changes.
 *
 * Revision 1.13  1993/08/17  15:07:32  thor
 * More 5.1 stuff.
 *
 * Revision 1.12  1993/07/21  21:45:20  vanandel
 * compat changes for compile with gcc
 *
 * Revision 1.11  1992/09/29  19:10:10  thor
 * Removed some now extraneous init code. Changed >> & << to use Sherrie's
 * new file format.
 *
 * Revision 1.10  1992/09/24  13:32:10  thor
 * Added stream I/O, new Send method.
 *
 * Revision 1.9  1992/07/28  17:29:19  thor
 * Changed bcopy to memcpy, added nav & insitu stuff.
 *
 * Revision 1.8  1992/03/06  18:50:49  thor
 * Fix lack of return statement in = method.
 *
 * Revision 1.7  1992/01/02  20:41:16  thor
 * Added method & C function to change TAPEHEADER of object.
 *
 * Revision 1.6  1992/01/02  20:28:02  thor
 * Added method for = operator & corresponding C function.
 *
 * Revision 1.5  1991/10/22  17:06:42  thor
 * Removed all the pointer crap and now use the in place parameter structs
 * in the header. Added fork code to make Send work.
 *
 * Revision 1.4  1991/10/16  14:35:14  thor
 * Added code for constructor using real tape header, changed GetRealHeader
 * to use user supplied memory.
 *
 * Revision 1.3  1991/10/14  19:14:54  thor
 * Fixed radar descriptor functions to handle 2 descriptors & added error
 * reporting for parameter functions.
 *
 * Revision 1.2  1991/09/11  16:26:15  thor
 * Added code to generate real tape header. Added code to handle passing
 * wavefrom counts. Fixed C routines.
 *
 * Revision 1.1  1991/09/05  16:40:59  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This file contains both the methods for the Header class and
 * the C interface routines to the class, so our non-C++ literate
 * programmers can use this class.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#ifdef UNIX
#define CLIENT_SIDE
#endif // UNIX

#include "Header.h"
#include <netinet/in.h>

#ifndef FAST
#define FAST register
#endif // FAST

#include <stdlib.h>
#include <cstring>

#ifdef UNIX
#include <signal.h>
#include <unistd.h>

static void Die(int);

static void Die(int)
{
  exit(0);
}
#endif // UNIX

Header::Header(void)
{
  if ((th = (TAPEHEADER *)malloc(sizeof(TAPEHEADER))) == NULL)
    {
      cerr << "Cannot allocate space for header!" << endl;
      exit(1);
    }
    
  numParams = 0;
}

Header::Header(FAST TAPEHEADER *t)
{
  if ((th = (TAPEHEADER *)malloc(sizeof(TAPEHEADER))) == NULL)
    {
      cerr << "Cannot allocate space for header!" << endl;
      exit(1);
    }

  memcpy(th,t,sizeof(TAPEHEADER));

  numParams = th->Fore.Radar.num_parameter_des;
}

Header::Header(void *v)
{
  if ((th = (TAPEHEADER *)malloc(sizeof(TAPEHEADER))) == NULL)
    {
      cerr << "Cannot allocate space for header!" << endl;
      exit(1);
    }
    
  FAST int size = sizeof(VOLUME) + sizeof(WAVEFORM) + sizeof(RADARDESC) +
    sizeof(FIELDRADAR) + sizeof(CELLSPACING);

  FAST unsigned char *t = (unsigned char *)v;

  memcpy(th,t,size);

  t += size;

  FAST int p = th->Fore.Radar.num_parameter_des;

  for (FAST int i = 0; i < p; i++)
    {
      FAST PARAMETER *parm = (PARAMETER *)t;

      Parameter(*parm,i);

      t += sizeof(PARAMETER);
    }

  size = sizeof(RADARDESC) + sizeof(FIELDRADAR) + sizeof(CELLSPACING);

  memcpy(&th->Aft,t,size);

  t += size;

  size = sizeof(NAVDESC);

  memcpy(&th->Nav,t,size);

  t += size;

  size = sizeof(INSITUDESC);

  memcpy(&th->Insitu,t,size);
}

Header::Header(const char *file)
{
  if ((th = (TAPEHEADER *)malloc(sizeof(TAPEHEADER))) == NULL)
    {
      cerr << "Cannot allocate space for header!" << endl;
      exit(1);
    }
  readFile(file);
}

int Header::Parameter(FAST PARAMETER &param, FAST int paramNum)
{
  FAST PARAMETER *p = &th->Fore.Params[0];

  if (paramNum <= MAX_PARAM)	// Still room.
    p += paramNum;
  else
    return(-1);

  memcpy(p,&param,sizeof(PARAMETER));

  paramNum++;			// To get correct count.

  if (paramNum > numParams)	// Need to update parameter values.
    {
      numParams = paramNum;
      th->Fore.Radar.num_parameter_des = paramNum;
      th->Aft.Radar.num_parameter_des = paramNum;
      th->Fore.Radar.total_num_des = paramNum + 1;
      th->Aft.Radar.total_num_des = paramNum + 1;
    }
  return(0);
}

PARAMETER *Header::Parameter(FAST int paramNum)
{
  FAST PARAMETER *p = &th->Fore.Params[0];

  if (paramNum > numParams)
    p = NULL;
  else
    p += paramNum;

  return(p);
}

void Header::CellSpacing(CELLSPACING &cs, FAST int descNum)
{
  FAST int i = sizeof(CELLSPACING);

  FAST CELLSPACING *c;

  if (descNum == 1)
    c = &th->Fore.CellSpacing;
  else
    c = &th->Aft.CellSpacing;

  memcpy(c,&cs,i);
}

CELLSPACING *Header::CellSpacing(FAST int descNum)
{
  if (descNum == 1)
    return(&th->Fore.CellSpacing);
  else
    return(&th->Aft.CellSpacing);
}

int Header::Radar(FAST RADARDESC &r, FAST int descNum)
{
  FAST int i = sizeof(RADARDESC);

  FAST RADARDESC *ptr;

  if (descNum == 1)
    {
      ptr = &th->Fore.Radar;
      memcpy(ptr,&r,i);
      return(0);
    }
  else if (descNum == 2)
    {
      ptr = &th->Aft.Radar;
      memcpy(ptr,&r,i);
      return(0);
    }
  return(-1);
}

RADARDESC *Header::Radar(FAST int descNum)
{
  if (descNum == 1)
    return(&th->Fore.Radar);
  else if (descNum == 2)
    return(&th->Aft.Radar);
  else
    return(NULL);
}

void Header::FieldRadar(FAST FIELDRADAR &f, FAST int descNum)
{
  FAST int i = sizeof(FIELDRADAR);
  FAST FIELDRADAR *ptr;

  if (descNum == 1)
    ptr = &th->Fore.FieldInfo;
  else
    ptr = &th->Aft.FieldInfo;

  memcpy(ptr,&f,i);
}

FIELDRADAR *Header::FieldRadar(FAST int descNum)
{
  if (descNum == 1)
    return(&th->Fore.FieldInfo);
  else
    return(&th->Aft.FieldInfo);
}

void Header::Volume(FAST VOLUME &r)
{
  FAST int i = sizeof(VOLUME);
  FAST VOLUME *ptr = &th->Volume;

  memcpy(ptr,&r,i);
}

VOLUME *Header::Volume(void)
{
  return(&th->Volume);
}

void Header::Waveform(FAST WAVEFORM &r)
{
  FAST int i = sizeof(WAVEFORM);
  FAST WAVEFORM *ptr = &th->Wave;

  memcpy(ptr,&r,i);
}

WAVEFORM *Header::Waveform(void)
{
  return(&th->Wave);
}

void Header::NavDesc(FAST NAVDESC &n)
{
  FAST int i = sizeof(NAVDESC);

  FAST NAVDESC *ptr = &th->Nav;

  memcpy(ptr,&n,i);
}

NAVDESC *Header::NavDesc(void)
{
  return(&th->Nav);
}

void Header::Insitu(FAST INSITUDESC &n)
{
  FAST int i = sizeof(INSITUDESC);

  FAST INSITUDESC *ptr = &th->Insitu;

  memcpy(ptr,&n,i);
}

INSITUDESC *Header::Insitu(void)
{
  return(&th->Insitu);
}

int Header::GetRealHeader(void *header)
{
  // There are two radars, each with a field info and cellspacing block.
  FAST int size = sizeof(VOLUME) + sizeof(WAVEFORM) +
    (2 * (sizeof(RADARDESC) + sizeof(FIELDRADAR) + sizeof(CELLSPACING))) +
    sizeof(NAVDESC) + sizeof(INSITUDESC);

  FAST RADARDESC *rdr = &th->Fore.Radar;

  FAST int np = numParams;
  size += 2 * (sizeof(PARAMETER) * np); // # params * # radars.

  FAST unsigned char *work = (unsigned char *)header;

  memcpy(work,&th->Volume,sizeof(VOLUME));

  work += sizeof(VOLUME);

  memcpy(work,&th->Wave,sizeof(WAVEFORM));

  work += sizeof(WAVEFORM);

  // Do forward radar.
  memcpy(work,&th->Fore.Radar,sizeof(RADARDESC));

  work += sizeof(RADARDESC);

  memcpy(work,&th->Fore.FieldInfo,sizeof(FIELDRADAR));

  work += sizeof(FIELDRADAR);

  memcpy(work,&th->Fore.CellSpacing,sizeof(CELLSPACING));

  work += sizeof(CELLSPACING);

  for (FAST int i = 0; i < np; i++)	// Do once for each parameter block.
    {
      memcpy(work,Parameter(i),sizeof(PARAMETER));
      work += sizeof(PARAMETER);
    }

  // Do aft radar.
  memcpy(work,&th->Aft.Radar,sizeof(RADARDESC));

  work += sizeof(RADARDESC);

  memcpy(work,&th->Aft.FieldInfo,sizeof(FIELDRADAR));

  work += sizeof(FIELDRADAR);

  memcpy(work,&th->Aft.CellSpacing,sizeof(CELLSPACING));

  work += sizeof(CELLSPACING);

  for (FAST int i = 0; i < np; i++)
    {
      memcpy(work,Parameter(i),sizeof(PARAMETER));
      work += sizeof(PARAMETER);
    }

  // Do Nav & Insitu descriptors.
  memcpy(work,&th->Nav,sizeof(NAVDESC));

  work += sizeof(NAVDESC);

  memcpy(work,&th->Insitu,sizeof(INSITUDESC));

  return(size);
}

int Header::readFile(const char *file)
{

  ifstream input(file);

  if (!input.good())
    return 1;

  input >> *this;

  if (!input.good())
    return 1;

  return 0;
}

Header &Header::operator=(FAST Header &in)
{
  FAST Header *ptr = this;

  memcpy(ptr->th,in.th,sizeof(TAPEHEADER));

  ptr->numParams = in.numParams;

  return(*ptr);
}

Header &Header::operator=(FAST TAPEHEADER *th)
{
  FAST Header *ptr = this;

  memcpy(ptr->th,th,sizeof(TAPEHEADER));

  numParams = th->Fore.Radar.num_parameter_des;

  return(*this);
}    

Header::~Header(void)
{
  if (th != NULL)
    free((char *)th);
}

// The following are C linkage routines to access this class.

HeaderPtr CreateHeader(FAST TAPEHEADER *t, FAST void *v)
{
  Header *ptr;

  if (v != NULL)
    ptr = new Header(v);
  else if (t != NULL)
    ptr = new Header(t);
  else
    ptr = new Header;

  return(ptr);
}

HeaderPtr CreateHeaderFromFile(char *file)
{
  Header *ptr = new Header(file);

  return(ptr);
}

int SetParameter(HeaderPtr ptr, PARAMETER *param, int paramNum)
{
  Header *h = (Header *)ptr;
  PARAMETER &ref = *param;

  return(h->Parameter(ref,paramNum));
}

PARAMETER *GetParameter(HeaderPtr ptr, int paramNum)
{
  Header *h = (Header *)ptr;

  return(h->Parameter(paramNum));
}

void SetCellSpacing(HeaderPtr ptr, CELLSPACING *cs, int descNum)
{
  Header *h = (Header *)ptr;
  CELLSPACING &ref = *cs;

  h->CellSpacing(ref,descNum);
}

CELLSPACING *GetCellSpacing(HeaderPtr ptr, int descNum)
{
  Header *h = (Header *)ptr;

  return(h->CellSpacing(descNum));
}

int SetRadar(HeaderPtr ptr, RADARDESC *cs, int descNum)
{
  Header *h = (Header *)ptr;
  RADARDESC &ref = *cs;

  return(h->Radar(ref,descNum));
}

RADARDESC *GetRadar(HeaderPtr ptr, int descNum)
{
  Header *h = (Header *)ptr;

  return(h->Radar(descNum));
}

void SetFieldRadar(HeaderPtr ptr, FIELDRADAR *cs, int descNum)
{
  Header *h = (Header *)ptr;
  FIELDRADAR &ref = *cs;

  h->FieldRadar(ref,descNum);
}

FIELDRADAR *GetFieldRadar(HeaderPtr ptr, int descNum)
{
  Header *h = (Header *)ptr;

  return(h->FieldRadar(descNum));
}

void SetVolume(HeaderPtr ptr, VOLUME *cs)
{
  Header *h = (Header *)ptr;
  VOLUME &ref = *cs;

  h->Volume(ref);
}

VOLUME *GetVolume(HeaderPtr ptr)
{
  Header *h = (Header *)ptr;

  return(h->Volume());
}

void SetWaveform(HeaderPtr ptr, WAVEFORM *cs)
{
  Header *h = (Header *)ptr;
  WAVEFORM &ref = *cs;

  h->Waveform(ref);
}

WAVEFORM *GetWaveform(HeaderPtr ptr)
{
  Header *h = (Header *)ptr;

  return(h->Waveform());
}

void SetNavDesc(FAST HeaderPtr ptr, FAST NAVDESC *nd)
{
  FAST Header *h = (Header *)ptr;

  NAVDESC &ref = *nd;

  h->NavDesc(ref);
}

NAVDESC *GetNavDesc(FAST HeaderPtr ptr)
{
  FAST Header *h = (Header *)ptr;

  return(h->NavDesc());
}

void SetInsitu(FAST HeaderPtr ptr, FAST INSITUDESC *id)
{
  FAST Header *h = (Header *)ptr;
  FAST INSITUDESC &ref = *id;

  h->Insitu(ref);
}

INSITUDESC *GetInsitu(HeaderPtr ptr)
{
  FAST Header *h = (Header *)ptr;

  return(h->Insitu());
}

void GetRpcHeader(HeaderPtr ptr, TAPEHEADER *th)
{
  Header *h = (Header *)ptr;

  memcpy(th,h->GetRpcHeader(),sizeof(TAPEHEADER));
}

int GetRealHeader(HeaderPtr ptr, void *header)
{
  Header *h = (Header *)ptr;

  return(h->GetRealHeader(header));
}

void CopyHeader(HeaderPtr src, HeaderPtr dest)
{
  FAST Header *s = (Header *)src;
  FAST Header *d = (Header *)dest;

  *d = *s;
}

void ResetHeader(HeaderPtr ptr, FAST TAPEHEADER *th)
{
  Header *h = (Header *)ptr;

  *h = th;
}

void DestroyHeader(HeaderPtr ptr)
{
  Header *h = (Header *)ptr;

  delete(h);
}

int readHeaderFile(HeaderPtr ptr, char *file)
{
  Header *h = (Header *)ptr;

  return h->readFile(file);
}

ostream& operator<<(ostream &os, Header &hdr)
{
  TAPEHEADER *th = hdr.GetRpcHeader();

  os.write((unsigned char *)&th->Volume,sizeof(VOLUME));
  os.write((unsigned char *)&th->Wave,sizeof(WAVEFORM));
  os.write((unsigned char *)&th->Fore.Radar,sizeof(RADARDESC));

#ifdef linux
  int des = ntohs(th->Fore.Radar.num_parameter_des);
#else
  int des = th->Fore.Radar.num_parameter_des;
#endif

  int size = (des * sizeof(PARAMETER)) +  sizeof(CELLSPACING) + 
    sizeof(FIELDRADAR);

  os.write((unsigned char *)&th->Fore.FieldInfo,size);

  size += sizeof(RADARDESC);

  os.write((unsigned char *)&th->Aft,size);

  os.write((unsigned char *)&th->Nav,sizeof(struct nav_descript));

  os.write((unsigned char *)&th->Insitu,sizeof(struct insitu_descript));

  return(os);
}

istream& operator>>(istream &is, Header &hdr)
{
  TAPEHEADER th;

  memset(&th.Fore.FieldInfo.file_name[0],0,80);
  memset(&th.Aft.FieldInfo.file_name[0],0,80);

  is.read((unsigned char *)&th.Volume,sizeof(VOLUME));
  is.read((unsigned char *)&th.Wave,sizeof(WAVEFORM));
  is.read((unsigned char *)&th.Fore.Radar,sizeof(RADARDESC));

  int fsize = sizeof(FIELDRADAR);

#ifdef linux
  int i = ntohs(th.Volume.format_version);
  int des = ntohs(th.Fore.Radar.num_parameter_des);
#else
  int i = th.Volume.format_version;
  int des = th.Fore.Radar.num_parameter_des;
#endif

  if (i == 1)
    {
      fsize -= 80;
#ifdef linux
      th.Volume.format_version = htons(2);
#else
      th.Volume.format_version = 2;
#endif
    }

  is.read((unsigned char *)&th.Fore.FieldInfo,fsize);

  int size = (des * sizeof(PARAMETER)) + sizeof(CELLSPACING);

  is.read((unsigned char *)&th.Fore.CellSpacing,size);

  is.read((unsigned char *)&th.Aft.Radar,sizeof(RADARDESC));

  is.read((unsigned char *)&th.Aft.FieldInfo,fsize);

  is.read((unsigned char *)&th.Aft.CellSpacing,size);

  is.read((unsigned char *)&th.Nav,sizeof(struct nav_descript));

  is.read((unsigned char *)&th.Insitu,sizeof(struct insitu_descript));

#ifdef linux
  size = htonl(sizeof(FIELDRADAR));
#else
  size = sizeof(FIELDRADAR);
#endif

  th.Fore.FieldInfo.field_radar_info_len = size;
  th.Aft.FieldInfo.field_radar_info_len = size;

  hdr = &th;

  return(is);
}
