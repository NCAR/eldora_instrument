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

    numParams = 0;
    th->Fore.Params.Params_val = params;
    th->Aft.Params.Params_val = params;

    th->Volume.number_sensor_des = 2; // Until we get ADS, etc defined.

    memcpy(&th->Insitu,t,size);
void Header::Parameter(FAST PARAMETER &param, FAST int paramNum)

    FAST PARAMETER *p = params;
{
    FAST PARAMETER *p = &th->Fore.Params[0];

    if (paramNum <= MAX_PARAM)	// Still room.
      return;
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
int Header::GetRealHeader(void **header)

int Header::GetRealHeader(void *header)
    FAST int size = sizeof(VOLUME) + ( 2 * (sizeof(RADARDESC)
					    + sizeof(FIELDRADAR)
					    + sizeof(CELLSPACING)));
    
    // We need to remove the structures used by RPC.
    FAST int wavesize = sizeof(WAVEFORM) - (5 * (sizeof(u_int) + 
						 sizeof(short *)));
    size += wavesize;

    FAST WAVEFORM *wv = &th->Wave;

    // Now we get the variable part of the waveform structure's size.
    FAST short *gates = &wv->num_gates[0];
    FAST int j = 5;
    FAST short wsize = wavesize;

    for (FAST int i = 0; i < j; i++, gates++)
      {
	  size += sizeof(short) * *gates;
	  wsize += *gates;
      }

    wv->waveform_des_length = wsize;
      (2 * (sizeof(RADARDESC) + sizeof(FIELDRADAR) + sizeof(CELLSPACING))) +
	sizeof(NAVDESC) + sizeof(INSITUDESC);

    FAST RADARDESC *rdr = &th->Fore.Radar;

    FAST int np = numParams;
    *header = (void *)malloc(size);

    if (*header == NULL)
      return(0);

    FAST void *work = *header;

    bcopy((char *)&th->Volume,(char *)work,sizeof(VOLUME));

    memcpy(work,&th->Volume,sizeof(VOLUME));

    bcopy((char *)&th->Wave,(char *)work,sizeof(WAVEFORM));

    work += wavesize;

    // Now get the waveform structures variable length arrays.
    gates = &wv->num_gates[0];

    FAST short *counts = wv->gate_dist1.gate_dist1_val;

    bcopy((char *)counts,(char *)work,*gates);

    work += *gates++;

    counts = wv->gate_dist1.gate_dist1_val;

    bcopy((char *)counts,(char *)work,*gates);

    work += *gates++;

    counts = wv->gate_dist2.gate_dist2_val;

    bcopy((char *)counts,(char *)work,*gates);

    work += *gates++;

    counts = wv->gate_dist3.gate_dist3_val;

    bcopy((char *)counts,(char *)work,*gates);

    work += *gates++;

    counts = wv->gate_dist4.gate_dist4_val;

    bcopy((char *)counts,(char *)work,*gates);

    work += *gates++;

    counts = wv->gate_dist5.gate_dist5_val;

    bcopy((char *)counts,(char *)work,*gates);

    work += *gates;

    // Whew!

    work += sizeof(WAVEFORM);
    bcopy((char *)&th->Fore.Radar,(char *)work,sizeof(RADARDESC));
    // Do forward radar.
    memcpy(work,&th->Fore.Radar,sizeof(RADARDESC));

    bcopy((char *)&th->Fore.FieldInfo,(char *)work,sizeof(FIELDRADAR));

    memcpy(work,&th->Fore.FieldInfo,sizeof(FIELDRADAR));

    bcopy((char *)&th->Fore.CellSpacing,(char *)work,sizeof(CELLSPACING));

    memcpy(work,&th->Fore.CellSpacing,sizeof(CELLSPACING));

    for (i = 0; i < np; i++)	// Do once for each parameter block.

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
    FAST WAVEFORM *wave = &th->Wave;
    FAST short *ptrs[5];
    
    // Save the user's arrays;
    ptrs[0] = &wave->gate_dist1.gate_dist1_val[0];
    ptrs[1] = &wave->gate_dist2.gate_dist2_val[0];
    ptrs[2] = &wave->gate_dist3.gate_dist3_val[0];
    ptrs[3] = &wave->gate_dist4.gate_dist4_val[0];
    ptrs[4] = &wave->gate_dist5.gate_dist5_val[0];

    // Zero them for RPC purposes.
    wave->gate_dist1.gate_dist1_val = NULL;
    wave->gate_dist2.gate_dist2_val = NULL;
    wave->gate_dist3.gate_dist3_val = NULL;
    wave->gate_dist4.gate_dist4_val = NULL;
    wave->gate_dist5.gate_dist5_val = NULL;

    wave->gate_dist1.gate_dist1_len = 0;
    wave->gate_dist2.gate_dist2_len = 0;
    wave->gate_dist3.gate_dist3_len = 0;
    wave->gate_dist4.gate_dist4_len = 0;
    wave->gate_dist5.gate_dist5_len = 0;

    FAST short *gates = &wave->num_gates[0];

    FAST int j = 5;

    FAST void *v;

    for (FAST int i = 0; i < j; i++, gates++)
      {
	  FAST int c = *gates;
	  FAST int offset = 0;
	  FAST int size;
	  WAVECOUNTS wc;

	  wc.index = i;

	  while (c > 0)
	    {
		if (c > 512)	// Why 512? 1024 seemed to break things!
		  {
		      size = 512 * sizeof(short);
		      wc.count = size;
		      wc.offset = offset;
		      bcopy((char *)&ptrs[i][offset],&wc.counts,size);
		      offset += 512;
		      c -= 512;
		  }
		else 
		  {
		      size = c * sizeof(short);
		      wc.count = size;
		      bcopy((char *)&ptrs[i][offset],&wc.counts,size);
		      wc.offset = offset;
		      c = 0;
		  }

		if ((v = sendcounts_1(&wc,client)) == NULL)
		  {
		      clnt_perror(client,"Header method Send failed. (wc)");
		      return(-1);
		  }
	    }
      }

    if ((v = sendheader_1(th,client)) == NULL)
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
    FAST Header *h = (Header *)ptr;

    NAVDESC &ref = *nd;

    bcopy((char *)h->GetRpcHeader(),(char *)th,sizeof(TAPEHEADER));
}

int GetRealHeader(HeaderPtr ptr, void **header)
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