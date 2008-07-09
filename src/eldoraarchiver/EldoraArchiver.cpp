#include <archiver/ArchiverService_impl.h>
#include <archiver/ArchiverListener.h>
#include <archiver/Resolver.h>
#include <archiver/EldoraWriter.h>
#include <tao/ORB_Core.h>

#include <boost/date_time/posix_time/ptime.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>

#include "EldoraHeader.h"

using std::string;
using std::cerr;
using std::endl;

using namespace archiver;

typedef NameResolver<PortableServer::POA, InitResolver> POAResolver;

/**
 * Fill a dummy test block with generated values based on an index key.
 **/
void
fillBlock(ByteBlock& block, int b)
{
  block.length(1500+(b % 5)*200);

  for (unsigned int i = 0; i < block.length(); ++i)
  {
    block[i] = i+b;
  }
  block[0] = 'R';
  block[1] = 'Y';
  block[2] = 'I';
  block[3] = 'B';
  block[4] = 0;
  block[5] = 0;
  block[6] = 0;
  block[7] = 44;
  
  int crapLen = block.length() - 44;
  unsigned char uhigh = crapLen / 256;
  char* high_p = (char*)&uhigh;
  unsigned char ulow = crapLen % 256;
  char* low_p = (char*)&ulow;
  block[44] = 'C';
  block[45] = 'R';
  block[46] = 'A';
  block[47] = 'P';
  block[48] = 0;
  block[49] = 0;
  block[50] = *high_p;
  block[51] = *low_p;
}

int
main(int argc, char *argv[])
{
    ACE_LOG_MSG->stop_tracing();
    EldoraHeader *hdr;
    try {
        hdr = new EldoraHeader("/code/burghart/eldora/src/headermaker/headers/tparc_conv.hd");
        boost::posix_time::ptime now = 
            boost::posix_time::second_clock::universal_time();
        hdr->vold()->setVolumeDateTime(now);
        hdr->vold()->setGenerationDate(now.date());
    } catch (EldoraHeader::BadHeaderException ex) {
        cout << "Header exception: " << ex << endl;
        abort();
    }

        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

        if (argc != 1)
        {
            cerr << "Usage: " << string(argv[0]) << " [orb options]" << endl;
            return 1;
        }

        // Create the two archiver servants
        cout << "creating archiver0..." << endl;
        ArchiverService_impl servantImpl0("archiver0", orb.in(), 
                           new archiver::EldoraWriter(), /*standalone*/ 0);
        ArchiverService_var servant0 = servantImpl0._this();
        cout << "done" << endl;
        
        ArchiverStatus_var status;

        // Activate the POA
        cout << "creating POA..." << endl;
        PortableServer::POA_var poa = POAResolver::resolve(orb.in(), "RootPOA");
        PortableServer::POAManager_var mgr = poa->the_POAManager();
        mgr->activate();
        cout << "done" << endl;

        // Check for its status
        status = servant0->getStatus();
        cerr << "servant 0 status: \n" << *status.ptr() << endl;

        ArchiverConfig config = archiver::defaultConfig();
        config.sessionId = 12345;
        config.projectName = "fooProject";
        config.operatorName = "george";
        config.fileName = "eldora";
        config.fileSizeLimit = 32000;

        // Verify that reconfig() sets the correct config but leaves it stopped.
        status = servant0->reconfig(config);

        // Block stuff
        ByteBlock block;
        unsigned int b = 0;
        unsigned int i;
        fillBlock(block, b);
        
        // Build a ByteBlock containing our header
        cout << "header size is " << hdr->size() << endl;
        ByteBlock hdrBlock;
        hdrBlock.length(hdr->size());
        std::stringbuf sb;
        sb.pubsetbuf((char*)hdrBlock.get_buffer(), hdrBlock.length());
        std::ostream os(&sb);
        hdr->streamTo(os, false);
        cout << "ByteBlock-ed header, with size " << hdrBlock.length() << endl;

        const char* datadir = "/tmp/archiver-test-data";
        config.directoryName = datadir;
        status = servant0->reconfig(config);
        status = servant0->start();

        status = servant0->sendBlock(hdrBlock);
        b = 0;
        fillBlock(block, b);
        status = servant0->sendBlock(block);

        string filename = status->currentFilename.in();
        unsigned long total =(hdrBlock.length() + block.length() + 2*8);

        // Keep writing blocks until the file rolls over.
        while(total <=(unsigned long)config.fileSizeLimit)
        {
            fillBlock(block, ++b);
            total += block.length() + 8;
            status = servant0->sendBlock(block);
        }
        // Header is appended to the end of the file.
        total += hdrBlock.length() + 8;

        // Retrieve the status to get an update of available disk space.
        status = servant0->getStatus();
 
        // Need to write one more to actually start the next file.
        total = hdrBlock.length() + 8;
        fillBlock(block, ++b);
        total += block.length() + 8;
        status = servant0->sendBlock(block);

        // Now verify the blocks in the file.
        if (status->errorFlag == false)
        {
            ifstream in(filename.c_str());
            if (!in)
                cerr << "Failed to open: " << filename << "\n";
            else
            {
                for (i = 0; i < b; ++i)
                    fillBlock(block, i);
            }
        }    

        // Test is over, tell it to quit
        status = servant0->quit();
//        status = servant1->quit();
        // Give it some time to finish up.
        sleep(2);
}

