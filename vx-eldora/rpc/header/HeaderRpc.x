/*
 * $Id$
 *
 * Header RPC definitions
 *
 * Use "rpcgen -N" to process this file.
 */

struct headername {
	char name[256];
};
typedef struct headername headername;

program HEADERRPC {
	version HEADERRPCVERS {
		int HEADERRPCFUNC(struct headername) = 1;
	} = 1;
} = 0x30000100;
