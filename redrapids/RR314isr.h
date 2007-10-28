#include "RR314.h"

//////////////////////////////////////////////////////////////////////
/// @section RR314 C routines
///
/// The interrupt service routine that is called by windrvr6. It will
/// be called on any interrupt that the RR314 has been configured
/// to generate. Usually these will be the DMA group transfer complete
/// interrupt. 
/// 
/// Note that these are not true interrupts, but rather a repackaging
/// of interrupts from the windrvr6 driver, and deliverd (I think) as
/// a signal. Since signals are delivered and handled asynchronously
/// by the process, they can appear as interrupts. Also note that this
/// signal processing will have semantics similar to a multithreaded
/// application.
///
/// The Red River dmaChan adapter library provides a C API for manipulating
/// the 314 card. The library manages a descriptor of type s_ChannelAdapter 
/// for each physical card in use. Access to any functionality of 
/// a given card is always done through the descriptor.
///
/// The RR314 class is designed to provide a C++ representation of a
/// 314 card. It holds the s_ChannelAdapter variable. However, since the 
/// interrupt service routine is a C function, it is not natively aware
/// of the RR314 class instance associated with a given 314 card. To allow
/// the ISR code to access that instance, a static map is provided by
/// the RR314 class which associates the pointer to the s_ChannelAdapter
/// instance with the instance of the RR314 calls. The ISR can retrieve this
/// pointer and thus access the controlling RR314 instance.
///
/// @param pCA The dmaChan adapter structure associated with the card 
/// providing the interrupt. This pointer has been associated with the card 
/// via the Adapter_Open(&s_ChannelAdapter) function in the Red River 
/// dmaChan adapter library.
void Adapter_ISR(s_ChannelAdapter *pCA);

/// Process completed DMA groups for one dmaChan. The data will be
/// transferred from each DMA group buffer into the buffer queues
/// in the RR314 class.
/// @param pCA The dmaChan adapter descriptor
/// @param chan The dmaChan number
/// @param pRR314 The RR314 instance associated with pCA
void processDMAGroups(s_ChannelAdapter *pCA, int chan, RedRapids::RR314* pRR314);

/// Transfer one DMA group to RR314
/// @param pCA The dmaChan adapter.
/// @param chan The dmaChan number.
/// @param pRR314 The RR314 instance associated with pCA
void sendGroupToRR314(s_ChannelAdapter* pCA, int chan, int group);
