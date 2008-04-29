#ifndef CA_SRAMMEMMAP_H
#define CA_SRAMMEMMAP__H

#if defined(__cplusplus)
    extern "C" {
#endif

#define SRAM_ALL -1
#define SRAMA	0
#define SRAMB 	1
#define SRAMC	2
#define SRAMD	3

#define V4_SRAMA_ADDR_ADR		0x930
		#define SRAM_WREN			(0x1<<31)
		#define SRAM_RDEN			(0x1<<30)
	#define V4_SRAMA_WRDATA_ADR		0x934
	#define V4_SRAMA_RDDATA_ADR		0x938
	#define V4_SRAMA_CTL_ADR		0x93C
	
	
	#define V4_IDLY_ADR			0x844
		#define ADC_CLK_HI		(0x0<<4)	/*ADC Clk 150-213 MHz*/
		#define ADC_CLK_LO		(0x1<<4)	/*ADC Clk 50-150 MHz*/
		#define IDLY_RST		(0x1)		
		#define BUS_66			(0x0<<1)
		#define BUS_33			(0x1<<1)	
	
#ifdef __cplusplus
}
#endif

#endif