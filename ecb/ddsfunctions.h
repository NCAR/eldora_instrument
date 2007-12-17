extern "C"{
void DisplayMenu();
short mygetline(char *buf);
int LoadFile();
int InitFile();
int AppendFile();
void delay();
int hextodec(char hex);
char *dectohex(int dec);
void UDPSocketInit();
void UDPSocketClose();
void rfinit(int unit);
void rfputval(int uin, double freqin, int nrin); 
void Output(unsigned char cmd[]); 
void Toggle(void); 
}
