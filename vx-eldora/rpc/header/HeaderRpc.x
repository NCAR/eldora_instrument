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

%/*
% * When we "send" a header, we actually just send a file name.  It is assumed
% * that the RPC server side can get to the header file using this name.  In 
% * practice, this means that the VxWorks machines hosting the RPC service
% * are mounting a directory via NFS, that the client writes the header 
% * there before issuing the RPC call below, and that the client sends the
% * file name appropriate for the RPC server side.
% */
program HEADERRPC {
	version HEADERRPCVERS {
		int HEADERRPCFUNC(struct headername) = 1;
	} = 1;
} = 0x30000100;
