#ifndef CA_PULSEDIG__H
#define CA_PULSEDIG__H

#if defined(__cplusplus)
    extern "C" {
#endif


//V4 User Registers
	#define V4_REV_ADR			0x800
	
	#define V4_TEST_ADR			0x804
	
	#define V4_CTL_ADR			0x80C	
		#define HDR_EN			(0x1<<29)	
		#define CONT_CAP		(0x1<<27)	//Logic 1 has cont. capture after trigger, 0 uses snapshot
		#define ADCA_SIM		(0x1<<26)
		#define ADCB_SIM		(0x1<<25)
		#define EXT1PPS_EN		(0x1<<19)
		#define TIMECTL_EN		(0x1<<18)
		#define EXTTRIG_EN		(0x1<<17)	//Logic 1 for Ext Trig, logic 0 for trigger via Bus
		#define DMA_EN			(0x1<<16)
		#define SOFT_RST		(0x1<<15)
		#define ADCAFF_FLUSH	(0x1<<13)
		#define ADCBFF_FLUSH	(0x1<<12)
		#define ADCA_CAP		(0x1<<8)
		#define ADCB_CAP		(0x1<<7)
		#define BYTE_SWAP_EN	(0x1<<4)
		#define ADC_DCM_RST		0x1
			
	#define V4_STAT_ADR			0x810
		#define INT_ACTV			(0x1<<31)
		#define INTR_EN				(0x1<<31)
		#define DMA_GRP_DONE		(0x1<<30)
		#define IDELAY_NOT_DONE		(0x1<<27)
		#define OVERTEMP			(0x1<<26)
		#define TRIG_1PPS			(0x1<<25)
		#define TELAD_FF_EMPTY		(0x1<<24)
		#define ADCA_FF_EMPTY		(0x1<<23) 
		#define ADCB_FF_EMPTY		(0x1<<22) 
		#define ADCC_FF_EMPTY		(0x1<<21) 
		#define ADCD_FF_EMPTY		(0x1<<20) 
		#define TELAD_FF_OVFLW		(0x1<<19)
		#define ADCA_FF_OVFLW		(0x1<<18)
		#define ADCB_FF_OVFLW		(0x1<<17)
		#define ADCC_FF_OVFLW		(0x1<<16)
		#define ADCD_FF_OVFLW		(0x1<<15)
		#define TELAD_FF_FULL		(0x1<<14)
		#define ADCA_FF_FULL		(0x1<<13)
		#define ADCB_FF_FULL		(0x1<<12)
		#define ADCC_FF_FULL		(0x1<<11)
		#define ADCD_FF_FULL		(0x1<<10)
		#define ADCA_CURR_OVR		(0x1<<9)
		#define ADCB_CURR_OVR		(0x1<<8)
		#define ADCC_CURR_OVR		(0x1<<7)
		#define ADCD_CURR_OVR		(0x1<<6)	
		#define ADCA_PREV_OVR		(0x1<<5)
		#define ADCB_PREV_OVR		(0x1<<4)
		#define ADCC_PREV_OVR		(0x1<<3)
		#define ADCD_PREV_OVR		(0x1<<2)
		#define SRAM_DCM_UNLOCKED 	(0x1<<1)
		#define ADC_DCM_UNLOCKED 	(0x1)
		
	#define V4_MASK_ADR			0x814
		//Bits as defined in V4_STAT_ADR
	
	

	#define V4_ADCA_SNAPCNT_ADR 0x830
	#define V4_ADCB_SNAPCNT_ADR 0x834
	#define V4_ADCC_SNAPCNT_ADR 0x838
	#define V4_ADCD_SNAPCNT_ADR 0x83C
	
	#define V4_INTTRIG_ADR		0x840
	
	#define V4_IDLY_ADR			0x844
		#define ADC_CLK_HI		(0x0<<4)	/*ADC Clk 150-213 MHz*/
		#define ADC_CLK_LO		(0x1<<4)	/*ADC Clk 50-150 MHz*/
		#define IDLY_RST		(0x1)		
		#define BUS_66			(0x0<<1)
		#define BUS_33			(0x1<<1)	

	#define V4_ADCA_LOW_ADR		0x900
	#define V4_ADCA_HIGH_ADR	0x904
	
	#define V4_ADCB_LOW_ADR		0x908
	#define V4_ADCB_HIGH_ADR	0x90C
	
	#define V4_TOD_SEC_ADR		0xA000
	
	#define V4_CLKPERIOD_ADR	0xA004
	
	#define V4_ADCA_START_SEC_ADR	0xA010
	#define V4_ADCA_START_FRAC_ADR	0xA014
	
	#define V4_ADCB_START_SEC_ADR	0xA018
	#define V4_ADCB_START_FRAC_ADR	0xA01C
	
	
	
#ifdef __cplusplus
}
#endif

#endif