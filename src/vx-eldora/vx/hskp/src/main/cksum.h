///
/// Calculate the POSIX CRC-32 checksum for the given file (same results as
/// the POSIX cksum command).  
/// On success, zero is returned and the CRC checksum is stored in crcRet.  
/// On error, -1 is returned and crcRet is left unchanged.
///
#include <string>
#include <sys/types.h>

int cksum(std::string filename, uint32_t& crcRet);