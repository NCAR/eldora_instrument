/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		HpaRpc.hh 
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        
 */
#ifndef INCHpaRpchh
#define INCHpaRpchh

#ifdef OK_RPC
#include <rpc/rpc.h>
#include <c_varieties.h>
#endif // OK_RPC

enum HPA_CMD {
        HPA_OFF = 0,
        HPA_STANDBY = 1,
        HPA_OPERATE = 2,
        HPA_ANTENNA = 3,
        HPA_DUMMYLOAD = 4,
        HPA_STATUS = 5,
        HPA_CLEAR = 6,
        HPA_EXIT = 7
};

#ifdef OK_RPC
typedef enum HPA_CMD HPA_CMD;
bool_t xdr_HPA_CMD(XDR *, HPA_CMD *);

#define HpaRpcProg1 ((u_long)0x30000601)
#define HpaRpcProg2 ((u_long)0x30000602)
#define HpaRpcVers ((u_long)1)
#define HpaSendCmd ((u_long)1)

#ifdef HPA_CLIENT_SIDE
extern void *hpasendcmd_1(enum HPA_CMD *, CLIENT *);
#else
extern char *hpasendcmd_1(char *, struct svc_req *);
int HpaRpcInit(int hpa);
#endif // HPA_CLIENT_SIDE

#else
program HpaRpcProg {
    version HpaRpcVers {
	void HpaSendCmd(enum HPA_CMD) = 1;
    } = 1;
} = 0x30000600;
#endif // OK_RPC

#endif // INCHpaRpchh
