#include <archiver/ArchiverService_impl.h>
#include <archiver/ArchiverListener.h>
#include <archiver/Resolver.h>
#include <archiver/EldoraWriter.h>
#include <tao/ORB_Core.h>

#include <DoradeASIB.h>
#include <DoradeFRAD.h>
#include <DoradeRYIB.h>

#include <boost/date_time/posix_time/ptime.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>

#include "DoradeHeader.h"

using std::string;

using namespace archiver;

typedef NameResolver<PortableServer::POA, InitResolver> POAResolver;

///**
// * Fill a dummy test block with generated values based on an index key.
// **/
//void
//fillBlock(ByteBlock& block, int b)
//{
//  block.length(1500+(b % 5)*200);
//
//  for (unsigned int i = 0; i < block.length(); ++i)
//  {
//    block[i] = i+b;
//  }
//  block[0] = 'R';
//  block[1] = 'Y';
//  block[2] = 'I';
//  block[3] = 'B';
//  block[4] = 0;
//  block[5] = 0;
//  block[6] = 0;
//  block[7] = 44;
//  
//  int crapLen = block.length() - 44;
//  unsigned char uhigh = crapLen / 256;
//  char* high_p = (char*)&uhigh;
//  unsigned char ulow = crapLen % 256;
//  char* low_p = (char*)&ulow;
//  block[44] = 'C';
//  block[45] = 'R';
//  block[46] = 'A';
//  block[47] = 'P';
//  block[48] = 0;
//  block[49] = 0;
//  block[50] = *high_p;
//  block[51] = *low_p;
//}

int
main(int argc, char *argv[])
{
    boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();
    ACE_LOG_MSG->stop_tracing();
    DoradeHeader *hdr;
    try {
        hdr = new DoradeHeader("/code/burghart/eldora/src/headermaker/headers/tparc_conv.hd");
        hdr->vold()->setVolumeDateTime(now);
        hdr->vold()->setGenerationDate(now.date());
    } catch (DoradeHeader::BadHeaderException ex) {
        std::cerr << "Header exception: " << ex << std::endl;
        abort();
    }

    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (argc != 1)
    {
        std::cerr << "Usage: " << string(argv[0]) << " [orb options]" << std::endl;
        return 1;
    }

    // Create the archiver servant
    ArchiverService_impl servantImpl("archiver", orb.in(), 
            new archiver::EldoraWriter(), /*standalone*/ true);
    ArchiverService_var servant = servantImpl._this();

    ArchiverStatus_var status;

    // Activate the POA
    PortableServer::POA_var poa = POAResolver::resolve(orb.in(), "RootPOA");
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    ArchiverConfig config = archiver::defaultConfig();
    config.sessionId = 12345;
    config.projectName = "fooProject";
    config.operatorName = "george";
    config.fileName = "eldora";
    config.fileSizeLimit = 32000;
    config.directoryName = "/tmp/EldoraArchiver";

    // Verify that reconfig() sets the correct config but leaves it stopped.
    status = servant->reconfig(config);

    // Data ray
    DoradeRYIB ryib(now, 1, 0.0, 0.0, 500.0, 30.0, 0);
    DoradeASIB asib(-105.0, 40.0, 2.6, 1.0, 50.0, 50.0, 2.0, 0.0, 0.0, 1.1, 
            0.0, 180.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    DoradeFRAD frad(0, "FORE", 40.0, 20.0, 0.3, 3.2e9, 5, 0, -110.0, 0, 0, 399);
    short shortData[2400];
    frad.putTwoByteData(shortData, sizeof(shortData) / sizeof(*shortData));

    // output stringstream
    std::ostringstream os;

    // Stuff the data ray into a ByteBlock
    ByteBlock block;
    block.length(ryib.getDescLen() + asib.getDescLen() + frad.getDescLen());
    os.rdbuf()->pubsetbuf((char*)block.get_buffer(), block.length());
    ryib.streamTo(os, false);
    asib.streamTo(os, false);
    frad.streamTo(os, false);

    // Build a ByteBlock containing our header
    ByteBlock hdrBlock;
    hdrBlock.length(hdr->size());
    os.rdbuf()->pubsetbuf((char*)hdrBlock.get_buffer(), hdrBlock.length());
    hdr->streamTo(os, false);

    status = servant->start();

    status = servant->sendBlock(hdrBlock);
    status = servant->sendBlock(block);
    std::cout << "servant status: \n" << *status.ptr() << std::endl;

    string filename = status->currentFilename.in();
    unsigned long total =(hdrBlock.length() + block.length() + 2 * 8);

    // Keep writing blocks until the file rolls over.
    while(total <=(unsigned long)config.fileSizeLimit)
    {
        total += block.length() + 8;
        status = servant->sendBlock(block);
    }
    // Header is appended to the end of the file.
    total += hdrBlock.length() + 8;
    std::cout << "total for " << filename << " should be " << total << std::endl;

    // Need to write one more to actually start the next file.
    total = hdrBlock.length() + 8;
    total += block.length() + 8;
    status = servant->sendBlock(block);
    
    // Tell the servant to quit
    servant->quit();

    // A final header should have been appended to the file
    total += hdrBlock.length() + 8;
    std::cout << "total for " << status->currentFilename.in() << 
        " should be " << total << std::endl;

}

