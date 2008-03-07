/*  REMORA LED BLINKER 
*	Blinks the LEDs for 30 seconds. Uses 
*	a physical memory buffer to map the 
* 	fpga registers to user space. 
*/
#include <stdio.h>
#include <dsp21k.h>
#include <unistd.h> /* sleep */

int main(int argc, char** argv)
{
	DSP21K_DEVICE_CFG cfg;
	PDSP21K dsp;
	DSP21K_PHYS_MEMORY pmem;
	U32 fpga_addr;
	int ret,i;
	U32 ledblink[] = { 0x00, 0x10, 0x40, 0x50 };

	if(!(dsp = dsp21k_open(-1)))
	{
		printf("error opening\n");
		return -1;
	}
	if((ret = dsp21k_get_device_info(dsp, &cfg)) != DSP21K_SUCCESS)
	{
		printf("error retrieving device info\n");
		return -2;
	}
	for(i = 0; i < 6; i++)
	{
		printf("BAR%d: addr %08x, size: %08x\n",
			i,cfg.pci_badr[i], cfg.badr_length[i]);
	}
	fpga_addr = (cfg.pci_badr[2] + 0x400000); 
	printf("fpga offset %08x\n", fpga_addr);
	
	pmem.phys_addr = fpga_addr;
	pmem.size = 0x100;		
	if((ret = dsp21k_alloc_phys_memory(dsp, &pmem)) != DSP21K_SUCCESS)
	{
		printf("error alloc phys mem\n");
		return -3;
	}
	for(i = 0; i < 30; i++)
	{
		*((U32*)pmem.mem_ptr) = ledblink[i%4];
		printf("register 0 = %08x\n", ((U32*)pmem.mem_ptr)[0]);
		sleep(1);
	}
	dsp21k_free_phys_memory(dsp, &pmem);
	dsp21k_close(dsp);
	return 0;
}
