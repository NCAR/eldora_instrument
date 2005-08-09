
extern long endf1,endf2;	/* end of crc sections */
extern unsigned short crcfv,crcfa;

crc_task()
 {
extern unsigned int block_crc();

  gatarout(1,"The background crc task is alive!\n",34);
  while(1)
    {
    crcfv=block_crc(0x0f000000L,&endf1);
    crcfa=block_crc(0x0f006800L,&endf2);
    }

 }
