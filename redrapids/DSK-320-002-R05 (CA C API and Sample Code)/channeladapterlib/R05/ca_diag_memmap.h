#ifndef CA_MEMMAP__H
#define CA_MEMMAP__H

#if defined(__cplusplus)
    extern "C" {
#endif


//V4 User Registers
	#define V4_REV_ADR			0x800
	#define V4_TEST_ADR			0x804
	
	#define V4_LED_ADR			0x808
		#define LED_ON		0x0
		#define LED_OFF		0x1
		
	#define V4_CTL_ADR			0x80C	
		#define HDR_EN			(0x1<<29)
		#define SNAP_TO_MEM		(0x1<<28)
		#define CONT_CAP		(0x1<<27)
		#define ADCA_SIM		(0x1<<26)
		#define ADCB_SIM		(0x1<<25)
		#define ADCC_SIM		(0x1<<24)
		#define ADCD_SIM		(0x1<<23)
		#define EXTTRIG_EN		(0x1<<17)
		#define DMA_EN			(0x1<<16)
		#define SOFT_RST		(0x1<<15)
		#define TELADCFF_FLUSH	(0x1<<14)
		#define ADCAFF_FLUSH	(0x1<<13)
		#define ADCBFF_FLUSH	(0x1<<12)
		#define ADCCFF_FLUSH	(0x1<<11)
		#define ADCDFF_FLUSH	(0x1<<10)
		#define TELADC_CAP		(0x1<<9)
		#define ADCA_CAP		(0x1<<8)
		#define ADCB_CAP		(0x1<<7)
		#define ADCC_CAP		(0x1<<6)
		#define ADCD_CAP		(0x1<<5)
		#define BYTE_SWAP_EN	(0x1<<4)
		#define SRAM_DCM_RST	(0x1<<1)
		#define ADC_DCM_RST		0x1
			
	#define V4_STAT_ADR			0x810
		#define INT_ACTV			(0x1<<31)
		#define INTR_EN				(0x1<<31)
		#define DMA_GRP_DONE		(0x1<<30)
		#define SNAP_DONE			(0x1<<29)
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
	
	#define V4_IOCTL_ADR			0x818
		#define P4H_OUT				(0x1<<17)
		#define P4H_IN				(0x0<<17)
		#define P4L_OUT				(0x1<<16)
		#define P4L_IN				(0x0<<16)	
		
		#define GPIO7_IN		(0x0<<7)
		#define GPIO7_OUT		(0x1<<7)
		#define GPIO6_IN		(0x0<<6)
		#define GPIO6_OUT		(0x1<<6)
		#define GPIO5_IN		(0x0<<5)
		#define GPIO5_OUT		(0x1<<5)
		#define GPIO4_IN		(0x0<<4)
		#define GPIO4_OUT		(0x1<<4)
		#define GPIO3_IN		(0x0<<3)
		#define GPIO3_OUT		(0x1<<3)
		#define GPIO2_IN		(0x0<<2)
		#define GPIO2_OUT		(0x1<<2)
		#define GPIO1_IN		(0x0<<1)
		#define GPIO1_OUT		(0x1<<1)
		#define GPIO0_IN		(0x0)
		#define GPIO0_OUT		(0x1)
			
		
		
	#define V4_P4HIGH_ADR		0x81C
	#define V4_P4LOW_ADR		0x820
					
	#define	V4_GPIOHIGH_ADR		0x824
	
	#define	V4_GPIOLOW_ADR		0x828 
	#define V4_ATTNCTL_ADR		V4_GPIOLOW_ADR //M320 only
		#define ATTNCTL_EN		(0x1<<7)
		#define CHA_ATTN_SEL	(0x0<<5)
		#define CHB_ATTN_SEL	(0x1<<5)
		#define ATTN_SET		(0x1<<6)

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
	
	#define V4_ADCC_LOW_ADR		0x910
	#define V4_ADCC_HIGH_ADR	0x914
	
	#define V4_ADCD_LOW_ADR		0x918
	#define V4_ADCD_HIGH_ADR	0x91C
	
	#define V4_TELAD_LOW_ADR	0x920
	#define V4_TELAD_HIGH_ADR	0x924
	
	#define TEST_REG1	0X0AAAAA8
	#define TEST_REG2	0X0AAAAAC
	#define TEST_REG3	0X0CCCCC8
	#define TEST_REG4	0X0CCCCCC
	#define TEST_REG5	0X1333330
	#define TEST_REG6	0X1333334
	#define TEST_REG7	0X1555550
	#define TEST_REG8	0X1555554
	#define TEST_REG9	0X1FFFFF8
	#define TEST_REG10	0X1FFFFFC
#ifdef __cplusplus
}
#endif

#endif
