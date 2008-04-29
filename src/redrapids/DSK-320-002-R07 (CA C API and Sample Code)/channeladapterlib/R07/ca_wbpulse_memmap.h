#ifndef CA_WBPULSE_MEMMAP__H
#define CA_WBPULSE_MEMMAP__H

#if defined(__cplusplus)
    extern "C" {
#endif
	
	#define V4_REV_ADR			0x800
	
	#define V4_CTL_ADR			0x80C
		#define HDR_EN			(0x1<<29)
		#define CONT_CAP		(0x1<<27)
		#define TIMECTL_EN		(0x1<<18)
		#define DMA_EN			(0x1<<16)
		#define SOFT_RST		(0x1<<15)
		#define DDCFF_FLUSH		(0x1<<13)
		#define USE_ADCB		(0x1<<7)
		#define USE_ADCA		(0x0)
		#define BYTE_SWAP_EN	(0x1<<4)
		#define ADC_DCM_RST		0x1	
		
	#define V4_STAT_ADR			0x810
		#define INT_ACTV			(0x1<<31)
		#define INTR_EN				(0x1<<31)
		#define DMA_GRP_DONE		(0x1<<30)
		#define SNAPSHOT_DONE		(0x1<<28)
		#define IDELAY_NOT_DONE		(0x1<<27)
		#define TRIG_1PPS			(0x1<<25)
		#define DDC_FF_EMPTY		(0x1<<23) 
		#define DDC_FF_OVFLW		(0x1<<18)
		#define DDC_FF_FULL			(0x1<<13)
		#define ADCA_CURR_OVR		(0x1<<9)
		#define ADCB_CURR_OVR		(0x1<<8)
		#define ADCA_PREV_OVR		(0x1<<5)
		#define ADCB_PREV_OVR		(0x1<<4)
		#define ADC_DCM_UNLOCKED 	(0x1)
		
	#define V4_MASK_ADR			0x814
		//Bits as defined in V4_STAT_ADR
		
	#define V4_INTTRIG_ADR		0x840
		#define SOFT_TRIGGER	0x1	
		
	#define V4_POLYCOEF0_ADR		0x1000
	
	#define V4_FIR1COEF0_ADR		0x1100
	
	#define V4_TUNERINC_ADR			0x1300
	
	#define V4_TUNERPHASE_ADR		0x1300
	
	#define V4_POLYDEC_ADR			0x1308
	
	#define V4_POLYSCALE_ADR		0x130C
	
	#define V4_FIR1DEC_ADR			0x1310
	
	#define V4_FIR1SCALE_ADR		0x1314
	
	#define V4_DDCSOFTSYNC_ADR		0x1400
		
	#define V4_DDCMODE_ADR			0x1404
		#define DDC_PROG			0x0
		#define DDC_RUN				0x1
		#define ADC_ZEROS			(0x0<<1)
		#define ADC_A				(0x1<<1)
		#define ADC_B				(0x2<<1)
		#define ADC_DC				(0x3<<1)
		
	#define V4_START_SEC_ADR		0x1500	
		#define SYNCEN				(0x1<<31)
		
	#define V4_START_FRAC_ADR		0x1504
	
	#define V4_SAMPLES_ADR			0x1508
	
	#define V4_SNAPEN_ADR			0x150C
	
	#define V4_INT_EXT_ADR			0x1510
	
	#define V4_SECS_IN_ADR			0x1514
	
	#define V4_LOAD_ADR				0x1518
	
	#define V4_CLK_FREQ_ADR			0x151C

#ifdef __cplusplus
}
#endif

#endif