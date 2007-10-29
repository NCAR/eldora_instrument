/**********************************************************************************************
	Red Rapids
	797 N. Grove Rd
	Suite 101
	Richardson, TX 75081
	www.redrapids.com | support@redrapids.com

	Desc: ChannelAdapter PCI Bridge (BAR0) Memory Map defines

			
***********************************************************************************************/

#ifndef CA_BAR0_MEMMAP__H
#define CA_BAR0_MEMMAP__H

#if defined(__cplusplus)
    extern "C" {
#endif

//Bridge Address Registers
	#define BRG_INTRSTAT_ADR	0x84
	#define BRG_INTRMASK_ADR	0x8C
		#define BRG_INTR_EN		0x01000000
		#define BRG_INTR_DIS	0x0

	#define BRG_RESET_ADR		0xE8
	
	#define BRG_FPGARESET_ADR	0xEC
	
	#define BRG_REV_ADR			0x100
	
	#define BRG_FPGA_STAT		0x104 //Was BRG_V4_STAT
	#define BRG_V4_STAT			BRG_FPGA_STAT
		#define FPGA_PROG_DONE	0x1
	
	#define BRG_FPGAPROG_ADR	0x11C //Was BRG_V4_PROG
	#define BRG_V4_PROG			BRG_FPGAPROG_ADR
	
	#define BRG_TEMPCTL_ADR		0x120
	
	#define BRG_USERCLKCTL_ADR	0x124 //Was BRG_DCLK_DIST_ADR
	#define BRG_DCLK_DIST_ADR   BRG_USERCLKCTL_ADR	
	
		/*M316*/
		#define GPIOCLK_ON		(0x0<<4)
		#define GPIOCLK_OFF		(0x1<<4)
		#define PDSEL1_M316		(0x1<<3)
		#define PDSEL0_M316		(0x1<<2)
		
		/*M314 & M320*/
		#define DCLK_EN			(0x1<<6)
		#define P4_CLK_OFF		(0x1<<5)
		#define P4_CLK_ON		(0x0<<5)
		#define DCLK_P4_PWRON   (0x1<<4)
		#define DCLK_P4_PWROFF  (0x0<<4)
		#define DCLK_P4_OFF		(0x1<<2)
		#define DCLK_P4_ON		(0x0<<2)
		#define DCLK_REF_1		(0x1<<1)
		#define DCLK_REF_0		(0x0<<1)
		#define DCLK_PWR_ON		0x1
		#define DCLK_PWR_OFF	0x0
	
	#define BRG_DCLK_SER_ADR	0x128
	
	#define BRG_ADCCLKCTL_ADR	0x12C
	#define BRG_M316ADC_CAL_ADR	0x12C
	
	#define BRG_ADCCLK_SER_ADR	0x130 //Was BRG_ACLK_SER_ADR
	#define BRG_M316ADC_SER_ADR	0x130
	#define BRG_ACLK_SER_ADR    BRG_ADCCLK_SER_ADR
	
	#define BRG_TEST_ADR		0x134

	#define	BRG_HWCONF_ADR		0x200 //Was   BRG_ACTL_ADR	
	#define BRG_ACTL_ADR	    BRG_HWCONF_ADR
		#define ADCB_VRNG1		(0x1<<5) //was ADCB_1VPP
		#define ADCB_VRNG0		(0x0<<5) //was ADCB_2VPP
		#define ADCA_VRNG1		(0x1<<4) //was ADCA_1VPP
		#define ADCA_VRNG0		(0x0<<4) //was ADCA_2VPP
		
				
		//M314
		#define M314_EXT_TRIG_ON	(0x0<<2)
		#define M314_EXT_TRIG_OFF	(0x1<<2)
		#define M314_SYNTH_PWR_ON	(0x1<<1)
		#define M314_SYNTH_PWR_OFF	(0x0<<1)
		
		//M320
		#define M320_RXPWR_ON		(0x1<<0)
		#define M320_RXPWR_OFF		(0x0<<0) 
		#define M320_GPIO_ON		(0x0<<3)
		#define M320_GPIO_OFF		(0x0<<3)
		#define M320_ADCA_VRNG1		(0x1<<4)
		#define M320_ADCA_VRNG0		(0x0<<4)
		#define M320_ADCB_VRNG1		(0x1<<5)
		#define M320_ADCB_VRNG0		(0x0<<5)
		
		//M316
		
		//M321
		#define M321_ADCB_HIRANGE	(0x0<<5)
		#define M321_ADCB_LORANGE	(0x1<<5)
		#define M321_ADCA_HIRANGE	(0x0<<4)
		#define M321_ADCA_LORANGE	(0x1<<4)
		#define M321_ADCB_RAND_ON	(0x1<<3)
		#define M321_ADCB_RAND_OFF	(0x0<<3)
		#define M321_ADCA_RAND_ON 	(0x1<<2)
		#define M321_ADCA_RAND_OFF 	(0x0<<2)
		#define M321_ADCB_DITHER_ON	(0x1<<1)
		#define M321_ADCB_DITHER_OFF (0x0<<1)
		#define M321_ADCA_DITHER_ON  (0x1<<0)
		#define M321_ADCA_DITHER_OFF (0x0<<0)
		
		
	#define BRG_LED_ADR				0x204
	#define BRG_M316ACLK_SYNC_ADR	0x204
	
	#define BRG_GPIO_ADR			0x208   	/*For M314*/
		#define	BRG_M314GPIO0_HI	0x1
		#define	BRG_M314GPIO1_HI	(0x1<<1)
		#define	BRG_M314GPIO2_HI	(0x1<<2)
		#define	BRG_M314GPIO3_HI	(0x1<<3)
		#define	BRG_M314GPIO_EN		(0x0<<4)
		#define	BRG_M314GPIO_DIS	(0x1<<4)
		
	#define BRG_M320CHA_ATTN_ADR	0x208		/*For M320*/
	#define BRG_M316_CLK_CTLA_ADR	0x208

	#define BRG_M320CHB_ATTN_ADR	0x20C		/*For M320*/
	#define BRG_M316CLK_CTLB_ADR	0x20C		/*For M316*/
	
	#define BRG_M320_FPGAATTNCTL_ADR 0x210
		#define ALLOW_FPGA_ATTN_CTL 0x1
	

#ifdef __cplusplus
}
#endif

#endif