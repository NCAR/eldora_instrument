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
            std::string topicName, std::string dataDir) { 
        if (! _theArchiver)
            _theArchiver = new EldoraArchiver(subscriber, topicName, dataDir);
        return _theArchiver;
    }
    // Get the singleton instance or NULL
    static EldoraArchiver* TheArchiver() { return _theArchiver; }
    
    /// Subclass DDSReader::notify(), which wil be called
    /// whenever new samples are added to the DDSReader available
    /// queue. Process the samples here.
    virtual void notify();
    
    /// Return the number of rays written by this archiver.
    /// @return the number of rays written by this archiver.
    int raysWritten() const { return _raysWritten; }
    
    /// Load the named ELDORA header.
    /// @param hdrFileName the name of the header to be loaded
    /// @return true iff the header was loaded successfully
    bool loadHeader(std::string hdrFileName);
    
    /// Return the unsigned 32-bit checksum for the current header, or zero 
    /// if we do not have a valid header.
    /// @return the checksum for the current header, or zero if we do not have
    ///     a valid header
    unsigned int headerChecksum() { return(_hdrValid ? _hdrChecksum : 0); }
    
protected:
    EldoraArchiver(DDSSubscriber& subscriber, std::string topicName,
    		std::string dataDir);
    virtual ~EldoraArchiver();
private:
    // Pointer to the singleton instance
    static EldoraArchiver* _theArchiver;

    // Our radd/archiver servant and last status we got from it
    archiver::ArchiverService_var _archiverServant;
    archiver::ArchiverStatus_var _status;
    // Our header, its checksum, a ByteBlock containing its DORADE 
    // representation, and a boolean telling whether they are all valid
    DoradeHeader* _hdr;
    unsigned int _hdrChecksum;
    archiver::ByteBlock _hdrBlock;
    bool _hdrValid;
    // A data buffer used in notify() when building a data ray, and its
    // size
    short* _dataBuf;
    int _dataBufLen;
    // How many rays have we written?
    int _raysWritten;
};

#endif /*ELDORAARCHIVER_H_*/
