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
 * Revision 1.2  1991/09/11  16:26:15  thor
 * Added code to generate real tape header. Added code to handle passing
 *
 *        
 *
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 *        This file contains both the methods for the Header class and
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

    th->Fore.Params.Params_val = params;
    th->Aft.Params.Params_val = params;
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
	  sizeof(PARAMETER));

    th->Fore.Params.Params_val = params;
    th->Aft.Params.Params_val = params;

    numParams = th->Fore.Radar.num_parameter_des;
Header::Header(void *t)



    memcpy(&th->Insitu,t,size);
void Header::Parameter(FAST PARAMETER &param, FAST int paramNum)

    FAST PARAMETER *p = params;
{
    if (paramNum <= MAX_PARAM)

    if (paramNum <= MAX_PARAM)	// Still room.
      return;
    else
    bcopy((char *)&param,(char *)p,sizeof(PARAMETER));
      }
    return(0);
}

    FAST PARAMETER *p = params;
{
    if (paramNum > MAX_PARAM)

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
void Header::Radar(FAST RADARDESC &r)

int Header::Radar(FAST RADARDESC &r, FAST int descNum)
    FAST RADARDESC *ptr = &th->Fore.Radar;
{
    bcopy((char *)&r,(char *)ptr,i);

    ptr = &th->Aft.Radar;

    bcopy((char *)&r,(char *)ptr,i);
      }
    return(-1);
RADARDESC *Header::Radar(void)

    return(&th->Fore.Radar);
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
void *Header::GetRealHeader(void)

    return(NULL);

    return(size);
Header &Header::operator=(FAST Header &in)
{
    FAST Header *ptr = this;

    memcpy(ptr->th,in.th,sizeof(TAPEHEADER));

    ptr->numParams = in.numParams;

    return(*ptr);
}

      ptr = NULL;
{
    FAST Header *ptr = this;

    memcpy(ptr->th,th,sizeof(TAPEHEADER));
void SetParameter(HeaderPtr ptr, PARAMETER *param, int paramNum)
    numParams = th->Fore.Radar.num_parameter_des;

    return(*this);
}    
    h->Parameter(ref,paramNum);

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
void SetRadar(HeaderPtr ptr, RADARDESC *cs)
    Header *h = (Header *)ptr;
    PARAMETER &ref = *param;

    return(h->Parameter(ref,paramNum));
    h->Radar(ref);

PARAMETER *GetParameter(HeaderPtr ptr, int paramNum)
RADARDESC *GetRadar(HeaderPtr ptr)
    Header *h = (Header *)ptr;

    return(h->Parameter(paramNum));
    return(h->Radar());

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
TAPEHEADER *GetRpcHeader(HeaderPtr ptr)

    NAVDESC &ref = *nd;

    return(h->GetRpcHeader());
}

void *GetRealHeader(HeaderPtr ptr)
{
    FAST Header *h = (Header *)ptr;

    return(h->GetRealHeader());
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
