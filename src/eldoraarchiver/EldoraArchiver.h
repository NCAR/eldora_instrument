#ifndef ELDORAARCHIVER_H_
#define ELDORAARCHIVER_H_

#include <vector>
#include <archiver/ArchiverService_impl.h>
#include <DDSReader.h>
#include <ProductsTypeSupportC.h>
#include <ProductsTypeSupportImpl.h>
using namespace EldoraDDS;

class EldoraQtProductsSource;
class DDSSubscriber;
class DoradeHeader;

// Singleton EldoraArchiver class
class EldoraArchiver : public ProductsReader {
public:
    // Get the singleton instance, creating it if necessary.
    static EldoraArchiver* TheArchiver(DDSSubscriber& subscriber, 
            std::string topicName, std::string hdrFileName, 
            std::string dataDir) { 
        if (! _theArchiver)
            _theArchiver = new EldoraArchiver(subscriber, topicName, 
            		hdrFileName, dataDir);
        return _theArchiver;
    }
    
    /// Subclass DDSReader::notify(), which wil be called
    /// whenever new samples are added to the DDSReader available
    /// queue. Process the samples here.
    virtual void notify();
    
    int raysWritten() const { return _raysWritten; }
protected:
    EldoraArchiver(DDSSubscriber& subscriber, std::string topicName,
    		std::string hdrFileName, std::string dataDir);
    virtual ~EldoraArchiver();
private:
    // Pointer to the singleton instance
    static EldoraArchiver* _theArchiver;

    // Our radd/archiver servant and last status we got from it
    archiver::ArchiverService_var _archiverServant;
    archiver::ArchiverStatus_var _status;
    // Our header, and the ByteBlock containing its DORADE representation
    DoradeHeader* _hdr;
    archiver::ByteBlock _hdrBlock;
    // A data buffer used in notify() when building a data ray, and its
    // size
    short* _dataBuf;
    int _dataBufLen;
    // How many rays have we written?
    int _raysWritten;
    // Are we initialized?
    bool _initialized;
};

#endif /*ELDORAARCHIVER_H_*/
