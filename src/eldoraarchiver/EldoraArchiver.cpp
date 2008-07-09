
#include <archiver/ArchiverService_impl.h>
#include <archiver/ArchiverListener.h>
#include <archiver/Resolver.h>
#include <archiver/EldoraWriter.h>
#include <tao/ORB_Core.h>
#include <logx/Logging.h>

#include <boost/date_time/posix_time/ptime.hpp>

#include <iostream>
#include <string>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

#include <sstream>
#include <fstream>

#include "EldoraHeader.h"
#include "checks.h"

LOGGING("EldoraArchiver");

#define NLOG ILOG << "======== "

using std::string;
using std::cerr;
using std::endl;

using namespace archiver;

typedef NameResolver< PortableServer::POA, InitResolver > POAResolver;

class Monitor : public ArchiverListener
{
public:

  void
  receiveStatus(const ArchiverStatus& status)
  {
    cerr << status;
  }

};

int
CheckBlock(istream& in, ByteBlock& block)
{
  int errors = 0;
  ByteBlock inblock;

  if (in.eof())
  {
    cerr << "premature end of file\n";
    return 1;
  }
  unsigned long length;
  in.read((char*)&length, sizeof(length));
  length = ntohl(length);
  Check(length == block.length());
  inblock.length(length);
  in.read((char*)inblock.get_buffer(), inblock.length());
  if (Check(in.good()) && Check(inblock.length() == block.length()))
  {
    Check(memcmp(block.get_buffer(), inblock.get_buffer(), 
		  block.length()) == 0);
  }
  in.read((char*)&length, sizeof(length));
  length = ntohl(length);
  Check(length == block.length());
  return errors;
}


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
}



int
checkFile(const std::string& filename,
	   ArchiverStatus_var& status, 
	   ByteBlockVector& headerv, ByteBlockVector& blockv)
{
  int errors = 0;

  // Check that all the blocks are in the file as written and the
  // file is not split.
  if (Check(filename.length() > 0) && Check(status->errorFlag == false))
  {
    ifstream in(filename.c_str());
    if (!in)
    {
      cerr << "Failed to open: " << filename << "\n";
      ++errors;
    }
    else
    {
      for (unsigned int bi = 0; bi < headerv.length(); ++bi)
      {
	if (CheckBlock(in, headerv[bi]) > 0)
	{
	  cerr << "mismatch for header block " << bi << "\n";
	  ++errors;
	  break;
	}
      }
      for (unsigned int bi = 0; bi < blockv.length(); ++bi)
      {
	if (CheckBlock(in, blockv[bi]) > 0)
	{
	  cerr << "mismatch for data block " << bi << "\n";
	  ++errors;
	  break;
	}
      }
    }
  }    
  return errors;
}



int
main(int argc, char *argv[])
{
    ACE_LOG_MSG->stop_tracing();
    EldoraHeader *hdr;
    try {
        hdr = new EldoraHeader("/code/burghart/eldora/src/headermaker/headers/tparc_conv.hd");
    } catch (EldoraHeader::BadHeaderException ex) {
        cout << "Header exception: " << ex << endl;
        abort();
    }

    int errors = 0;
    try 
    {
        logx::ParseLogArgs(argc, argv);
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

        if (argc != 1)
        {
            cerr << "Usage: " << string(argv[0]) << " [orb options]" << endl;
            logx::LogUsage();
            return 1;
        }

        // Create the two archiver servants
        ArchiverService_impl servantImpl0("archiver0", orb.in(), 
                           new archiver::EldoraWriter(), /*standalone*/ 0);
        ArchiverService_var servant0 = servantImpl0._this();
        
//        ArchiverService_impl servantImpl1("archiver1", orb.in(), 
//                           new archiver::EldoraWriter(), /*standalone*/ 0);
//        ArchiverService_var servant1 = servantImpl1._this();
        
        ArchiverStatus_var status;

        // Activate the POA
        PortableServer::POA_var poa = POAResolver::resolve(orb.in(), "RootPOA");
        PortableServer::POAManager_var mgr = poa->the_POAManager();
        mgr->activate();

        // Check for its status
        NLOG << "requesting and checking status...\n";
        status = servant0->getStatus();
        cerr << "servant 0 status: \n" << *status.ptr() << endl;
//        status = servant1->getStatus();
//        cerr << "servant 1 status: \n" << *status.ptr() << endl;

        // Simple checks
        Check(status->state == Stopped);
        Check(status->totalKilobytes == 0);
        Check(status->errorFlag == false);
        Check(status->kilobytesPerSecond == 0);

        NLOG << "setting and checking config...\n";
        ArchiverConfig config = archiver::defaultConfig();
        config.sessionId = 12345;
        config.projectName = "fooProject";
        config.operatorName = "george";
        config.fileName = "eldora";
        // Now try writing to the non-existent directory and check that it gets
        // created, along with the file.  Verify by actually reading the file.
        const char* datadir = "/tmp/archiver-test-data";
        system("/bin/rm -rf /tmp/archiver-test-data");
        config.directoryName = datadir;
        NLOG << "configuring for data directory: " << datadir;
        config.fileCloseDelay = 3;
//        config.directoryName = "/donotcreate";
        config.fileSizeLimit = 32000;

        // Verify that reconfig() sets the correct config but leaves it stopped.
        status = servant0->reconfig(config);
        Check(status->errorFlag == false);
        Check(status->state == Stopped);
        Check(status->totalKilobytes == 0);

        ArchiverConfig_var cv = servant0->getConfig();
        Check(cv->sessionId == config.sessionId);
        Check(cv->fileSizeLimit == config.fileSizeLimit);
        Check(!strcmp(cv->directoryName, config.directoryName));
        Check(cv->statusPeriod == 1);
        Check(cv->fileCloseDelay == 10);

        // Block stuff
        ByteBlock block;
        unsigned int b = 0;
        unsigned int i;
        fillBlock(block, b);
        ByteBlock header(1212);
        header.length(1212);
        for (i = 0; i < header.length(); ++i)
            header[i] = i % 255;
        header[0] = 'V';
        header[1] = 'O';
        header[2] = 'L';
        header[3] = 'D';

        boost::posix_time::ptime now = 
            boost::posix_time::second_clock::universal_time();
        DoradeVOLD vold(2, 1, 4096, "fooProj", now, "flight1", "ELDORA",
                        now.date(), 1);

        // Send a block prior to a header, and verify we get an error back
        // about waiting for a header.
        NLOG << "Sending a block ahead of a header to a running servant...";
        status = servant0->start();
        status = servant0->sendBlock(block);
        Check(status->errorFlag == true);
        NLOG << status->errorMessage;
        std::string expect = "Waiting for a header before writing data...";
        std::string got(status->errorMessage);
        Check(expect == got);
        status = servant0->stop();

        NLOG << "sending header and block while not running...";
        status = servant0->sendBlock(header);
        status = servant0->sendBlock(block);
        // There should be no error while the servant is not running,
        // and no bytes should have been written.
        Check(status->errorFlag == false);
        Check(status->totalKilobytes == 0);
        Check(status->state == Stopped);

        // Send a different header.  Even though the servant is stopped,
        // it should still detect the new header.
        NLOG << "sending a different header while not running...";
        header[4] = 255;
        status = servant0->sendBlock(header);
        status = servant0->sendBlock(block);
        // There should still be no error and no bytes.
        Check(status->errorFlag == false);
        Check(status->totalKilobytes == 0);
        Check(status->state == Stopped);

        // So start it.
        NLOG << "starting and stopping...";
        status = servant0->start();
        Check(status->state == Running);
        status = servant0->stop();
        Check(status->state == Stopped);
        status = servant0->start();
        Check(status->state == Running);

        // Send an empty block to verify it does not break anything.
        NLOG << "sending an empty block, nothing should happen...";
        ByteBlock empty;
        status = servant0->sendBlock(empty);
        Check(status->errorFlag == false);

//        NLOG << "sending block to non-makeable directory...\n";
//        status = servant0->sendBlock(header);
//        status = servant0->sendBlock(block);
//        if (Check(status->errorFlag == true))
//        {
//            cerr << "error: '" << status->errorMessage << "'" << endl;
//        }

//        // Now try writing to the non-existent directory and check that it gets
//        // created, along with the file.  Verify by actually reading the file.
//        const char* datadir = "/tmp/archiver-test-data";
//        system("/bin/rm -rf /tmp/archiver-test-data");
//        config.directoryName = datadir;
//        NLOG << "configuring for data directory: " << datadir;
//        config.fileCloseDelay = 3;
        status = servant0->reconfig(config);

        // The reconfig() and subsequent restart should have created the
        // data directory and updated the free space.
        long kbfree = status->kilobytesAvailable;
        NLOG << "initial kilobytes available: " << kbfree;
        Check(kbfree > 0);
        Check(status->errorFlag == false);

        cv = servant0->getConfig();
        // Check that the minimum delay of 10 seconds was enforced.
        Check(cv->fileCloseDelay == 10);
        // Archiver should be running again after a reconfig.
        Check(status->state == Running);
        NLOG << "sending block to create directory and start a new file...";
        status = servant0->sendBlock(header);
        b = 0;
        fillBlock(block, b);
        status = servant0->sendBlock(block);

        string filename = status->currentFilename.in();
        NLOG << "filename is '" << filename << "'";
        Check(filename.length() > 20);
        unsigned long total =(header.length() + block.length() + 2*8);
        Check(status->totalKilobytes ==((long)total / 1024));

        // Keep writing blocks until the file rolls over.
        while(total <=(unsigned long)config.fileSizeLimit)
        {
            fillBlock(block, ++b);
            total += block.length() + 8;
            status = servant0->sendBlock(block);
        }
        // Header is appended to the end of the file.
        total += header.length() + 8;
        long totalkb =(long)total / 1024;
        Check(status->totalKilobytes == totalkb);
        // Retrieve the status to get an update of available disk space.
        status = servant0->getStatus();
        kbfree -= totalkb;
        NLOG << "available KB after writing " << total << " bytes: "
        << status->kilobytesAvailable << ", expected " << kbfree;
        Check((status->kilobytesAvailable >= kbfree - 15) && 
                (status->kilobytesAvailable <= kbfree + 15));
        // Need to write one more to actually start the next file.
        total = header.length() + 8;
        fillBlock(block, ++b);
        total += block.length() + 8;
        status = servant0->sendBlock(block);
        Check(status->totalKilobytes ==((long)total / 1024));
        Check(filename != status->currentFilename.in());

        // Now verify the blocks in the file.
        if (Check(status->errorFlag == false))
        {
            ifstream in(filename.c_str());
            if (!in)
            {
                cerr << "Failed to open: " << filename << "\n";
                ++errors;
            }
            else
            {
                errors += CheckBlock(in, header);
                for (i = 0; i < b; ++i)
                {
                    fillBlock(block, i);
                    errors += CheckBlock(in, block);
                }
            }
        }    

        // Wait for the automatic closing period to expire.
        filename = status->currentFilename.in();
        NLOG << "waiting for automatic closing... (set to " << 
            cv->fileCloseDelay << " seconds)";
        sleep(2*cv->fileCloseDelay);
        // Get new status.
        NLOG << "getting status...";
        status = servant0->getStatus();
        Check(filename != status->currentFilename.in());

        // New filename should be empty since no file is opened.
        filename = status->currentFilename.in();
        Check(filename.length() == 0);

        // Test that the 10-second update of free disk space is working
        kbfree = status->kilobytesAvailable;
        NLOG << "available kilobytes: " << kbfree;
        total = header.length() + 8;
        fillBlock(block, 0);
        while(total <=(unsigned long)16*1024)
        {
            total += block.length() + 8;
            status = servant0->sendBlock(block);
        }
        NLOG << "waiting to make sure free disk space updating automatically...";
        sleep(11);
        // File will have closed automatically, which adds a header.
        total += header.length() + 8;

        block.length(0);
        kbfree -=(long)total / 1024;
        status = servant0->sendBlock(block);
        NLOG << "available KB after writing " << total << " bytes: "
        << status->kilobytesAvailable << ", expected " << kbfree;
        Check((status->kilobytesAvailable >= kbfree - 15) && 
                (status->kilobytesAvailable <= kbfree + 15));
        filename = status->currentFilename.in();

        // Send a different header and make sure it starts a new file
        // containing all the blocks in a block vector.
        ByteBlockVector headerv;
        headerv.length(2);
        headerv[0] = header;
        (headerv[0])[32] = 202;
        fillBlock(headerv[1], 1);
        ByteBlockVector blockv;
        blockv.length(64);

        unsigned int bi;
        for (bi = 0; bi < blockv.length(); ++bi)
        {
            fillBlock(blockv[bi], bi);
        }

        // We have to send a few blocks separately first to get the
        // filename.  Sending all blocks at once exceeds the file size
        // limit and so the returned status has an empty file name since
        // the file was closed.
        status = servant0->sendBlockVector(headerv);
        Check(filename != status->currentFilename.in());
        filename = status->currentFilename.in();
        status = servant0->sendBlockVector(blockv);
        errors += checkFile(filename, status, headerv, blockv);

        // Send the exact same vector, but this one should also be in
        // a new file since the last vector should have exceeded the
        // file size limit.
        status = servant0->sendBlockVector(headerv);
        Check(filename != status->currentFilename.in());
        filename = status->currentFilename.in();
        status = servant0->sendBlockVector(blockv);
        errors += checkFile(filename, status, headerv, blockv);

        // Test is over, tell it to quit
        NLOG << "sending quit message...";
        status = servant0->quit();
        NLOG << "Status: \n" << *status.ptr();
        // Give it some time to finish up.
        sleep(2);
    }
    catch(const CORBA::Exception& e)
    {
        cerr << "CORBA exception:\n " << e << endl;
        ++errors;
    }

    //  kill(0, 1);
    cerr << nchecks << " checks.\n"
    << errors << " failures.\n";
    return errors;
}

