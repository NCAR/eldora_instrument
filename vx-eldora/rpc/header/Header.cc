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
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 *        This file contains both the methods for the Header class and
 * the C interface routines to the class, so our non-C++ literate
 * programmers can use this class.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#ifdef UNIX
#define CLIENT_SIDE
#endif // UNIX

#include "Header.hh"

#ifndef FAST
#define FAST register
#endif // FAST

static void Die(int);

static void Die(int)
{
    exit(0);
}
#endif // UNIX

Header::Header(void)
{
    if ((params = (PARAMETER *)malloc(sizeof(PARAMETER) * 10)) ==
	NULL)
      {
	  fprintf(stderr,"Cannot allocate space for header!\n");
	  exit(1);
      }

    if ((th = (TAPEHEADER *)malloc(sizeof(TAPEHEADER))) == NULL)
    th->Fore.Params.Params_val = params;
    th->Aft.Params.Params_val = params;
      {
	  fprintf(stderr,"Cannot allocate space for header!\n");
	  exit(1);
      }
    
    numParams = 0;
}

Header::Header(FAST TAPEHEADER *t)
{
    if ((th = (TAPEHEADER *)malloc(sizeof(TAPEHEADER))) == NULL)
      {
    if ((params = (PARAMETER *)malloc(sizeof(PARAMETER) * 10)) ==
	NULL)
      {
	  fprintf(stderr,"Cannot allocate space for header!\n");
	  exit(1);
      }

    bcopy((char *)t,(char *)th,sizeof(TAPEHEADER));
	  exit(1);
    bcopy((char *)th->Fore.Params.Params_val,(char *)params,
	  sizeof(PARAMETER) * th->Fore.Params.Params_len);

    numParams = th->Fore.Params.Params_len;

    th->Fore.Params.Params_val = params;
    th->Aft.Params.Params_val = params;

    memcpy(th,t,sizeof(TAPEHEADER));

    numParams = th->Fore.Radar.num_parameter_des;
Header::Header(void *t)

Header::Header(void *v)
{
    if ((th = (TAPEHEADER *)malloc(sizeof(TAPEHEADER))) == NULL)
      {
	  fprintf(stderr,"Cannot allocate space for header!\n");
	  exit(1);
    if ((params = (PARAMETER *)malloc(sizeof(PARAMETER) * 10)) ==
	NULL)
      {
	  fprintf(stderr,"Cannot allocate space for header!\n");
	  exit(1);
      }

      }
    
    FAST int size = sizeof(VOLUME) + sizeof(WAVEFORM) + sizeof(RADARDESC) +
    bcopy(t,(char *)th,size);

    memcpy(th,t,size);

    t += size;

    FAST int p = th->Fore.Radar.num_parameter_des;

	  FAST PARAMETER *parm = t;
      {
	  FAST PARAMETER *parm = (PARAMETER *)t;

	  Parameter(*parm,i);

	  t += sizeof(PARAMETER);
      }

    bcopy(t,(char *)&th->Aft,size);

    memcpy(&th->Insitu,t,size);
}

    FAST PARAMETER *p = params;
{
    FAST PARAMETER *p = &th->Fore.Params[0];

    if (paramNum <= MAX_PARAM)	// Still room.
      p += paramNum;
    else
    bcopy((char *)&param,(char *)p,sizeof(PARAMETER));

    memcpy(p,&param,sizeof(PARAMETER));

    paramNum++;			// To get correct count.

    if (paramNum > numParams)	// Need to update parameter values.
	  th->Fore.Params.Params_len = paramNum;
	  th->Aft.Params.Params_len = paramNum;
      {
	  numParams = paramNum;
	  th->Fore.Radar.num_parameter_des = paramNum;
	  th->Aft.Radar.num_parameter_des = paramNum;
	  th->Fore.Radar.total_num_des = paramNum + 1;
	  th->Aft.Radar.total_num_des = paramNum + 1;
      }
    return(0);
}

    FAST PARAMETER *p = params;
{
    FAST PARAMETER *p = &th->Fore.Params[0];

    if (paramNum > numParams)
      p = NULL;
    else
      p += paramNum;

    return(p);
}

void Header::CellSpacing(CELLSPACING &cs)
{
    FAST CELLSPACING *c = &th->Fore.CellSpacing;
    bcopy((char *)&cs,(char *)c,i);

    memcpy(c,&cs,i);

    bcopy((char *)&cs,(char *)c,i);

    memcpy(c,&cs,i);
}

CELLSPACING *Header::CellSpacing(void)
{
    return(&th->Fore.CellSpacing);
}

int Header::Radar(FAST RADARDESC &r, FAST int descNum)
{
    FAST int i = sizeof(RADARDESC);

    FAST RADARDESC *ptr;

    if (descNum == 1)
	  bcopy((char *)&r,(char *)ptr,i);
	  ptr = &th->Fore.Radar;
	  memcpy(ptr,&r,i);
	  return(0);
      }
    else if (descNum == 2)
	  bcopy((char *)&r,(char *)ptr,i);
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

void Header::FieldRadar(FIELDRADAR &f)
{
    FAST int i = sizeof(FIELDRADAR);
    bcopy((char *)&f,(char *)ptr,i);

    memcpy(ptr,&f,i);

    bcopy((char *)&f,(char *)ptr,i);

    memcpy(ptr,&f,i);
}

FIELDRADAR *Header::FieldRadar(void)
{
    return(&th->Fore.FieldInfo);
}

void Header::Volume(FAST VOLUME &r)
{
    FAST int i = sizeof(VOLUME);
    bcopy((char *)&r,(char *)ptr,i);

    memcpy(ptr,&r,i);
}

VOLUME *Header::Volume(void)
{
    return(&th->Volume);
}

void Header::Waveform(FAST WAVEFORM &r)
{
    FAST int i = sizeof(WAVEFORM);
    bcopy((char *)&r,(char *)ptr,i);

    memcpy(ptr,&r,i);
}

WAVEFORM *Header::Waveform(void)
{
    return(&th->Wave);
}

int Header::GetRealHeader(void *header)
{
      (2 * (sizeof(RADARDESC) + sizeof(FIELDRADAR) + sizeof(CELLSPACING)));
    
      (2 * (sizeof(RADARDESC) + sizeof(FIELDRADAR) + sizeof(CELLSPACING))) +
	sizeof(NAVDESC) + sizeof(INSITUDESC);

    FAST RADARDESC *rdr = &th->Fore.Radar;

    FAST int np = numParams;
    FAST void *work = header;

    bcopy((char *)&th->Volume,(char *)work,sizeof(VOLUME));

    memcpy(work,&th->Volume,sizeof(VOLUME));

    bcopy((char *)&th->Wave,(char *)work,sizeof(WAVEFORM));

    memcpy(work,&th->Wave,sizeof(WAVEFORM));

    work += sizeof(WAVEFORM);
    bcopy((char *)&th->Fore.Radar,(char *)work,sizeof(RADARDESC));
    // Do forward radar.
    memcpy(work,&th->Fore.Radar,sizeof(RADARDESC));

    bcopy((char *)&th->Fore.FieldInfo,(char *)work,sizeof(FIELDRADAR));

    memcpy(work,&th->Fore.FieldInfo,sizeof(FIELDRADAR));

    bcopy((char *)&th->Fore.CellSpacing,(char *)work,sizeof(CELLSPACING));

    memcpy(work,&th->Fore.CellSpacing,sizeof(CELLSPACING));

    work += sizeof(CELLSPACING);

	  bcopy((char *)Parameter(i),(char *)work,sizeof(PARAMETER));
      {
	  memcpy(work,Parameter(i),sizeof(PARAMETER));
	  work += sizeof(PARAMETER);
      }
    bcopy((char *)&th->Aft.Radar,(char *)work,sizeof(RADARDESC));
    // Do aft radar.
    memcpy(work,&th->Aft.Radar,sizeof(RADARDESC));

    bcopy((char *)&th->Aft.FieldInfo,(char *)work,sizeof(FIELDRADAR));

    memcpy(work,&th->Aft.FieldInfo,sizeof(FIELDRADAR));

    bcopy((char *)&th->Aft.CellSpacing,(char *)work,sizeof(CELLSPACING));

    memcpy(work,&th->Aft.CellSpacing,sizeof(CELLSPACING));

    work += sizeof(CELLSPACING);

	  bcopy((char *)Parameter(i),(char *)work,sizeof(PARAMETER));
      {
	  memcpy(work,Parameter(i),sizeof(PARAMETER));
	  work += sizeof(PARAMETER);
    memcpy(work,&th->Insitu,sizeof(INSITUDESC));

    return(size);
}

// This only applies to the client side.
#ifdef CLIENT_SIDE
    FAST CLIENT *client = clnt_create(target,HeaderRPC,HeaderVers,"udp");
{
    FAST CLIENT *client = clnt_create(target,HeaderRPC,HeaderVers,"tcp");

    if (client == NULL)
    if (sendheader_1(th,client) == NULL)
      {
	  clnt_perror(client,"Header method Send failed. (th)");
	  return(-1);
	    }
      }
    clnt_destroy(client);

    return(0);
}

    return(0);
}

    if (params != NULL)
      free((char *)params);
#endif // UNIX

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

void SetCellSpacing(HeaderPtr ptr, CELLSPACING *cs)
{
    Header *h = (Header *)ptr;
    CELLSPACING &ref = *cs;

    h->CellSpacing(ref);
}

CELLSPACING *GetCellSpacing(HeaderPtr ptr)
{
    Header *h = (Header *)ptr;

    return(h->CellSpacing());
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

void SetFieldRadar(HeaderPtr ptr, FIELDRADAR *cs)
{
    Header *h = (Header *)ptr;
    FIELDRADAR &ref = *cs;

    h->FieldRadar(ref);
}

FIELDRADAR *GetFieldRadar(HeaderPtr ptr)
{
    Header *h = (Header *)ptr;

    return(h->FieldRadar());
}

void SetVolume(HeaderPtr ptr, VOLUME *cs)
    FAST Header *h = (Header *)ptr;

    NAVDESC &ref = *nd;

    bcopy((char *)h->GetRpcHeader(),(char *)th,sizeof(TAPEHEADER));
}

NAVDESC *GetNavDesc(FAST HeaderPtr ptr)
{
    FAST Header *h = (Header *)ptr;

    return(h->NavDesc());
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

#ifdef UNIX
ostream& operator<<(ostream &os, Header &hdr)
{
    TAPEHEADER *th = hdr.GetRpcHeader();

    int size = sizeof(VOLUME) + sizeof(WAVEFORM) +
      (2 * ((th->Fore.Radar.num_parameter_des * sizeof(PARAMETER)) +
	    sizeof(CELLSPACING) + sizeof(FIELDRADAR) + sizeof(RADARDESC)));

    os.write((unsigned char *)th,size);

    return(os);
}

istream& operator>>(istream &is, Header &hdr)
{
    TAPEHEADER th;

    is.read((unsigned char *)&th.Volume,sizeof(VOLUME));
    is.read((unsigned char *)&th.Wave,sizeof(WAVEFORM));
    is.read((unsigned char *)&th.Fore.Radar,sizeof(RADARDESC));

    int size = (th.Fore.Radar.num_parameter_des * sizeof(PARAMETER)) + 
      sizeof(CELLSPACING) + sizeof(FIELDRADAR);

    is.read((unsigned char *)&th.Fore.FieldInfo,size);

    size += sizeof(RADARDESC);

    is.read((unsigned char *)&th.Aft,size);

    hdr = &th;

    return(is);
}
#endif // UNIX
