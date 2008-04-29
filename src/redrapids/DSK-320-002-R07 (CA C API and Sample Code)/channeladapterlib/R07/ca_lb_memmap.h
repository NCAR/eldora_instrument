#ifndef CA_LB_MEMMAP__H
#define CA_LB_MEMMAP__H

#if defined(__cplusplus)
    extern "C" {
#endif


	//V4 DMA registers	
	#define DMA_REV_ADR			0x0
	#define DMA_REV_EXP_ADR		0x4
	#define DMA_INT_MASK_ADR	0x8
	#define DMA_INT_STAT_ADR	0x10
	#define DMA_PAGE_ADR		0x30
	#define DMA_CH0_BLKSIZE_ADR 0x80
	#define	DMA_CH0_BLKCNT_ADR	0x100
	#define	DMA_CH0_GRPCNT_ADR	0x180
	#define	DMA_CH0_CURBLK_ADR	0x200
	#define	DMA_CH0_CURGRP_ADR	0x280
	#define DMA_CH0_GRPSPERINT_ADR 0x300
	#define DMA_CURPHYS0_ADR	0x400
	#define DMA_PHYS0_ADR		0x500


#ifdef __cplusplus
}
#endif

#endif