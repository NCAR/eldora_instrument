#include "SvnVersion.h"
#include "svnInfo.h"

std::string
SvnVersion::revision() {
    return std::string(SVNREVISION);
}

