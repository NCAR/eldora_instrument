#ifndef ETH29INCLUDE
#define ETH29INCLUDE

int eth29Init(char* interfaceIPaddress, int intVector, int intLevel);

int eth29SendDgram(char* sendToIPaddress, int port, unsigned char* buffer, int length);

int eth29FastDgram(unsigned char *buffer, int length);
#endif

