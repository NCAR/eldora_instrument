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
            std::string topicName) { 
        if (! _theArchiver)
            _theArchiver = new EldoraArchiver(subscriber, topicName);
        return _theArchiver;
    }
    
    /// Subclass DDSReader::notify(), which wil be called
    /// whenever new samples are added to the DDSReader available
    /// queue. Process the samples here.
    virtual void notify();
protected:
    EldoraArchiver(DDSSubscriber& subscriber, std::string topicName);
    virtual ~EldoraArchiver();
private:
    // Pointer to the singleton instance
    static EldoraArchiver* _theArchiver;

    archiver::ArchiverService_impl* _servantImpl;
    archiver::ArchiverStatus_var _status;
    DoradeHeader* _hdr;
    archiver::ByteBlock _hdrBlock;
    short* _dataBuf;
    int _dataBufLen;
};

#endif /*ELDORAARCHIVER_H_*/
