#ifndef CA_RETURNS__H
#define CA_RETURNS__H

#if defined(__cplusplus)
    extern "C" {
#endif

	#define CA_OK	0x0
	#define CA_FAIL 0x80000000
		#define CA_OPEN_FAIL			CA_FAIL|0x1
		#define CA_RDWR_FAIL			CA_FAIL|0x2
		#define CA_DMA_ALLOC_FAIL		CA_FAIL|0x3
		#define CA_ASY_FAIL				CA_FAIL|0x4



#ifdef __cplusplus
}
#endif

#endif