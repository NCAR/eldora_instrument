#include <iostream>
#include <math.h>

#include <ace/Time_Value.h>
#include <ace/Event.h>

#include "EldoraProducts.h"

#include "TimetagUtils.h"

//
// Products playback from an archive file
//
// Create a DoradeHeader object to parse the housekeeping headers at the
// start of the file.
//
// beams are parsed from triplets of
// RYIB  : ray time, sweepnumber, az, el
// ASIB  : aircraft specific info: including roll, heading, radarRotAngle
// FRAD  : the actual data, with parameters ordered the same as PARM
//        descriptors in the header


// TODO:

// adjust the length of rays in _products



/// Macro used to apply scale and bias to convert to 16 bit 
/// integer int for on-the-wire transfer
#define TOSHORT(data, scale, bias) ((short)(data*scale+bias))

/// A squaring function
#define square(a) (a*a)

////////////////////////////////////////////////////
EldoraProducts::EldoraProducts(DDSPublisher& publisher,
                               std::string productsTopic,
                               bool reverseVelocity,
                               int usPerBeam) :
    _rays(0), _publisher(publisher), _productsTopic(productsTopic),
            _productsWriter(publisher, productsTopic), _droppedRays(0),
    _publishedRays(0),
    _termsInitialized(false),_products(0),_beamDelay(0,usPerBeam), _event(),
    _verbose(false)
{
    if (reverseVelocity)
        _velSign = -1;
    else
        _velSign = 1;

}

////////////////////////////////////////////////////
EldoraProducts::~EldoraProducts()
{

}

void EldoraProducts::initProducts(EldoraDDS::Products * products, EldoraDDS::Housekeeping & ddsHskp, int gates)
{
        products->hskp = ddsHskp;
        products->radarId = ddsHskp.radarName[0] == 'F' ? EldoraDDS::Forward : EldoraDDS::Aft;
        products->timestamp = ddsHskp.rayNum = _rays++;
        products->rotAngle = ddsHskp.radarRotAngle;
        products->gateSpacingMeters = ddsHskp.cellWidth[0];
        // Resize the ray lengths.

        products->p1.length(gates);
        _prodPtrMap["P1"] = &products->p1;
        products->p2.length(gates);
        _prodPtrMap["P2"] = &products->p2;
        products->p3.length(gates);
        _prodPtrMap["P3"] = &products->p3;
        products->p4.length(gates);
        _prodPtrMap["P4"] = &products->p4;
        products->vr.length(gates);
        _prodPtrMap["VR"] = &products->vr;
        products->vs.length(gates);
        _prodPtrMap["VS"] = &products->vs;
        products->vl.length(gates);
        _prodPtrMap["VL"] = &products->vl;
        products->dm.length(gates);
        _prodPtrMap["DM"] = &products->dm;
        products->dbz.length(gates);
        _prodPtrMap["DBZ"] = &products->dbz;
        products->sw.length(gates);
        _prodPtrMap["SW"] = &products->sw;
        products->ncp.length(gates);
        _prodPtrMap["NCP"] = &products->ncp;

        products->dbzScale = ddsHskp.dbzScale;
        products->dbzOffset = ddsHskp.dbzBias;

        products->swScale = ddsHskp.swScale;
        products->swOffset = ddsHskp.swBias;

        products->vrScale = ddsHskp.vrScale;
        products->vrOffset = ddsHskp.vrBias;

        products->vlScale = ddsHskp.vlScale;
        products->vlOffset = ddsHskp.vlBias;
        
        products->vsScale = ddsHskp.vsScale;
        products->vsOffset = ddsHskp.vsBias;

#ifdef NOTDEF
        products->dmScale = ddsHskp.dmScale;
        products->dmOffset = ddsHskp.dmBias;
#endif

        products->ncpScale = ddsHskp.ncpScale;
        products->ncpOffset = ddsHskp.ncpBias;
        
        
}


void EldoraProducts::playback(std::string inputFileName)
{

    EldoraDDS::Housekeeping ddsHskp;

    parseDoradeHeader(inputFileName, ddsHskp);

    char block[65536];
    unsigned char* ublock = (unsigned char*)block;
    
    std::ifstream ifile(inputFileName.c_str());
    if (ifile.fail()) {
        std::cerr << "Failed to open " << inputFileName << std::endl;
        exit(1);
    }
    std::cerr << "Dropped rays = " << _droppedRays <<
               " published rays = " << _publishedRays <<
        std::endl;
    
    
    ifile.seekg(0, std::ios::end);
    int fileLen = ifile.tellg();
    std::cerr << "File size = " << fileLen << std::endl;
    ifile.seekg(0, std::ios::beg);

    try {
        while (! ifile.eof() && (ifile.tellg() < fileLen)) {
            if ( ( (_droppedRays % 100) == 1) || ((_publishedRays % 100) == 1) )
                std::cerr << "Dropped rays = " << _droppedRays <<
                    " published rays = " << _publishedRays << std::endl;
            unsigned int blockpos = 0;
            // FORTRAN blocking: 
            //      <4-byte big-endian block length>
            //      <data>
            //      <trailing 4-byte big-endian block length>
            
            // read the big-endian block length
            unsigned int blocksize;
            ifile.read(block, 4);
            blocksize = (ublock[0] << 24) | (ublock[1] << 16) | 
                (ublock[2] << 8) | ublock[3];
            if (_verbose) std::cerr << "blocksize = " << blocksize << std::endl;

            assert(blocksize <= sizeof(block));
            

            // Now read the data portion of the block
            ifile.read(block, blocksize);
            
            while (blockpos < blocksize) {
                if (_verbose)                std::cerr << "##### Blockpos = " << blockpos << " filepos = " << ifile.tellg() << std::endl;
                unsigned char* udata = (unsigned char*)block + blockpos;
                DoradeDescriptor desc(udata, blocksize - blockpos, false);
                std::string descName(desc.getDescName());
                unsigned int descLen(desc.getDescLen());

                if (descName == "VOLD") {
                    DoradeVOLD vold(udata, blocksize - blockpos, false);
                    
                    vold.setVerbose(true);
                    // Historical KLUGE: VOLD "sensor descriptor count" for
                    // ELDORA is actually twice the number of sensors.  I'm 
                    // guessing this is because they are counting both RADD
                    // and FRIB descriptors as "sensor" descriptors, while
                    // the DORADE document shows a sensor descriptor as the
                    // whole block of descriptors describing a sensor and its
                    // parameters.
                    if (_verbose) std::cout << "VOLD @ " << vold.getVolumeDateTime() << 
                        " with " << vold.getSensorCount() / 2 << " sensors" << 
                        std::endl;
                } else if (descName == "RADD") {
                    DoradeRADD radd(udata, blocksize - blockpos, false);
                    if (_verbose) std::cout << "RADD for " << radd.getRadarName() << 
                        std::endl;   
                } else if (desc.getDescName() == "PARM") {
                    DoradePARM parm(udata, blocksize - blockpos, false);
                    int paramSize = 0;
                    switch (parm.getParamType()) {
                    case 1:
                        paramSize = 1;
                        break;
                    case 2:
                        paramSize = 2;
                        break;
                    case 3:
                    case 4:
                        paramSize = 4;
                        break;
                    }
                    if (_verbose) std::cout << "PARM " << paramSize << "-byte " <<
                        parm.getParamName() << std::endl;
                } else if (descName == "RYIB") {
                    // a new beam starts witha RYIB.
                    
                    // get an empty products container from DDS
                    _products = _productsWriter.getEmptyItem();
                    

                    DoradeRYIB ryib(udata, blocksize - blockpos, false);
                    parseRYIB(ryib, ddsHskp);
                    if (_verbose) std::cout << "RYIB @ " << ryib.getDayOfYear() << "/" << 
                        ryib.getRayTime() << ", sweep: " <<
                        ryib.getSweepNumber() << std::endl;   
                } else if (desc.getDescName() == "RAWD") {
                    DoradeRAWD rawd(udata, blocksize - blockpos, false);
                    // RAWD is a strange bird, and we don't get a good descriptor
                    // length until we have a DoradeRAWD rather than just
                    // a DoradeDescriptor.
                    descLen = rawd.getDescLen();
                    if (_verbose) std::cout << "RAWD (" << descLen << " bytes)" << std::endl;
                } else if (desc.getDescName() == "CSPD") {
                    DoradeCSPD cspd(udata, blocksize - blockpos, false);
                    short nsegs = cspd.getNSegments();
                    const short* ncells = cspd.getNCells();
                    int nGates = 0;
                    for (int s = 0; s < nsegs; s++)
                        nGates += ncells[s];
                    if (_verbose) std::cout << "CSPD with " << nGates << " gates" << std::endl;
                } else if (desc.getDescName() == "FRAD") {
                    DoradeFRAD frad(udata, blocksize - blockpos, false);
                    parseFRAD(frad, ddsHskp);
                    const short* sdata = frad.getShortData();
                    if (_verbose) {
                        std::cout << "FRAD (" << frad.getRadarName() << ") data: ";
                        for (int i = 0; i < 10; i++)
                            std::cout << sdata[i] << ",";
                        std::cout << "..." << std::endl;
                    }
                } else if (desc.getDescName() == "ASIB"){
                    DoradeASIB asib(udata, blocksize - blockpos, false);
                    parseASIB(asib, ddsHskp);
                    if (_verbose) std::cout << "ASIB (" << asib.getLatitude() << "," <<
                        asib.getLongitude() << ")\n: ";
                } else {
                   
                    if (_verbose) std::cout << desc.getDescName() << " (" << 
                        desc.getDescLen() << " bytes)" << std::endl;
                }
                blockpos += descLen;
            }

            // trailing block length
            int endblocksize;
            ifile.read(block, 4);
            endblocksize = (ublock[0] << 24) | (ublock[1] << 16) | 
                (ublock[2] << 8) | ublock[3];
            //            std::cerr << "endblocksize = " << endblocksize << std::endl;
        }
    } 
    catch (DescriptorException dex) {
        std::cerr << dex.what() << std::endl;
        abort();
    }

}



////////////////////////////////////////////////////

int EldoraProducts::numRays()
{
    int n = _rays;
    _rays = 0;
    return n;
}


////////////////////////////////////////////////////

void EldoraProducts::parseVOLD(DoradeVOLD *vold, EldoraDDS::Housekeeping &ddsHskp)
{
    
    _volumeStart = vold->getVolumeDateTime();
    
}
void EldoraProducts::parseRYIB(DoradeRYIB &ryib, EldoraDDS::Housekeeping &ddsHskp)
{
    ddsHskp.timetag = ptimeToTimetag(ryib.getRayDateTime(_volumeStart));
    ddsHskp.sweepNum = ryib.getSweepNumber();
    ddsHskp.julianDay = ryib.getDayOfYear();
    ddsHskp.hour = ryib.getHour();
    ddsHskp.minute = ryib.getMinute();
    ddsHskp.second = ryib.getSecond();
    ddsHskp.millisecond = ryib.getMillisecond();
    ddsHskp.azimuth = ryib.getAzimuth();
    ddsHskp.elevation = ryib.getElevation();
    ddsHskp.peakXmitPower = ryib.getPeakXmitPower();
    ddsHskp.trueScanRate = ryib.getTrueScanRate();
    ddsHskp.rayStatus = ryib.getRayStatus();
}
void EldoraProducts::parseASIB(DoradeASIB &asib, EldoraDDS::Housekeeping &ddsHskp)
{
    ddsHskp.longitude = asib.getLongitude();
    ddsHskp.latitude = asib.getLatitude();
    ddsHskp.altitudeMSL = asib.getAltitudeMSL();
    ddsHskp.altitudeAGL = asib.getAltitudeAGL();
    ddsHskp.groundSpeedEW = asib.getGroundSpeedEW();
    ddsHskp.groundSpeedNS = asib.getGroundSpeedNS();
    ddsHskp.vertVelocity = asib.getVerticalVelocity();
    ddsHskp.heading = asib.getHeading();
    ddsHskp.roll = asib.getRoll();
    ddsHskp.pitch = asib.getPitch();
    ddsHskp.yaw = asib.getYaw();
    ddsHskp.radarRotAngle = asib.getAntennaScanAngle();
    ddsHskp.radarTiltAngle = asib.getAntennaTiltAngle();
    ddsHskp.windEW = asib.getUWind();
    ddsHskp.windNS = asib.getVWind();
    ddsHskp.windVert = asib.getWWind();
    ddsHskp.headingChangeRate = asib.getHeadingChangeRate();
    ddsHskp.pitchChangeRate = asib.getPitchChangeRate();
    
}

void EldoraProducts::parseFRAD(DoradeFRAD &frad, EldoraDDS::Housekeeping &ddsHskp)
{
    ddsHskp.dataSysStatus = frad.getDataSystemStatus();
    ddsHskp.testPulsePower = frad.getTestPulsePower();
    ddsHskp.testPulseStart = frad.getTestPulseStart();
    ddsHskp.testPulseWidth = frad.getTestPulseWidth();
    ddsHskp.testPulseFreq = frad.getTestPulseFreq();
    ddsHskp.testPulseAtten = frad.getTestPulseAttenuation();
    ddsHskp.testPulseFNum = frad.getTestPulseFNum();
    ddsHskp.noisePower = frad.getNoisePower();  /// XXX use this, or RADD
                                                /// noisepower ?
    ddsHskp.rayCount = frad.getRayCount();
    ddsHskp.firstRecGate = frad.getFirstGate();
    ddsHskp.lastRecGate = frad.getLastGate();

    const short *fradData = frad.getShortData();
    int sizeFradData = frad.getSizeShortData();
    int nParams = _prodNames.size();
    int nCells = sizeFradData/nParams;
    
    EldoraDDS::housekeeping_::_radarName_copy(ddsHskp.radarName, frad.getRadarName().c_str());
    if (_products) {
        //        std::cerr << "publishing: ";
        initProducts(_products, ddsHskp, nCells);
        // Now retrieve all of the data from the FRAD in the correct order
        // using the external DORADE representation 
        for (int p = 0; p < nParams; p++) {
            //            std::cerr << _prodNames[p] << ",";
            Product *prod = _prodPtrMap[_prodNames[p]];
            for (int cell = 0; cell < nCells; cell++) {
                (*prod)[cell] = fradData[cell * nParams + p];
            }
            
        }
        _productsWriter.publishItem(_products);
        ++_publishedRays;
        _event.wait(&_beamDelay, 0);
    } else {
        ++_droppedRays;
    }
    
}
void EldoraProducts::parseRADD(DoradeRADD *radd, EldoraDDS::Housekeeping &ddsHskp)
{
    EldoraDDS::housekeeping_::_radarName_copy(ddsHskp.radarName, radd->getRadarName().c_str());
    ddsHskp.radarConstant = radd->getRadarConstant();
    int i;
    for (i = 0; i < 4; ++i) {
        ddsHskp.freqs[0] = radd->getFrequency(i);
    }
    //    ddsHksp.noisePower = 
    ddsHskp.prt = radd->getIPP(0);
    ddsHskp.prtLong = radd->getIPP(1);
    ddsHskp.rotationVel = radd->getScanRate();
    
 
}
void EldoraProducts::parseFRIB(DoradeFRIB *frib, EldoraDDS::Housekeeping &ddsHskp)
{
    ddsHskp.xBandGain = frib->getxBandGain();
    ddsHskp.lnaLoss = frib->getWaveGuideLossIn();

}

void EldoraProducts::parseWAVE(DoradeWAVE *wave, EldoraDDS::Housekeeping &ddsHskp)
{
    ddsHskp.repeatSeqPerDwell = wave->getRepeatsPerDwell();
    ddsHskp.msPerRepeat = wave->getRepeatPeriod();
}
void EldoraProducts::parseCSPD(DoradeCSPD *cspd, EldoraDDS::Housekeeping &ddsHskp)
{
    ddsHskp.firstGateDistance = cspd->getDistToFirstGate();
    ddsHskp.cellWidth[0] = cspd->getCellSpacing()[0];
}
void EldoraProducts::parsePARMs(DoradeHeader &doradeHeader, EldoraDDS::Housekeeping & ddsHskp)
{
    int numParams = doradeHeader.nParams(0);
    int i;
    DoradePARM *param;
    std::string pname;

    // everytime we find a PARM, save the corresponding Product pointer in
    // a vector, so we associate the order of parameters in a FRAD

    for (i = 0; i < numParams; ++i) {
        param = doradeHeader.parm(0, i);
        pname = param->getParamName();
        _prodNames.push_back(pname);
        if (pname.compare("DBZ")== 0) {
            ddsHskp.dbzScale = param->getScale();
            ddsHskp.dbzBias = param->getOffset();
            
        }else if (pname.compare("SW")== 0) {
            ddsHskp.swScale = param->getScale();
            ddsHskp.swBias = param->getOffset();
        } else if (pname.compare("NCP")== 0) {
            ddsHskp.ncpScale = param->getScale();
            ddsHskp.ncpBias = param->getOffset();
        } else if (pname.compare("VR") == 0) {
            ddsHskp.vrScale = param->getScale();
            ddsHskp.vrBias = param->getOffset();
        } else if (pname.compare("VL")== 0) {
            ddsHskp.vlScale = param->getScale();
            ddsHskp.vlBias = param->getOffset();
        } else if (pname.compare("VS") == 0) {
            ddsHskp.vsScale = param->getScale();
            ddsHskp.vsBias = param->getOffset();
#ifdef NOTDEF
        } else if (pname.compare("DM") == 0) {
            ddsHskp.dmScale = param->getScale();
            ddsHskp.dmBias = param->getOffset();
#endif
        } else {
            std::cerr << "ignored " << pname << "PARM\n";
        }
    }
    
}




void EldoraProducts::parseDoradeHeader(std::string inputFileName,
                                       EldoraDDS::Housekeeping& hskp)
{
    DoradeHeader doradeHeader(inputFileName, false,
                              true // Fortran blocked file
                              );
    parseVOLD(doradeHeader.vold(), hskp);
    parseWAVE(doradeHeader.wave(), hskp);
    parseRADD(doradeHeader.radd(0), hskp);
    parseFRIB(doradeHeader.frib(0), hskp);
    parseCSPD(doradeHeader.cspd(0), hskp);
    /// parse PARM's
    parsePARMs(doradeHeader, hskp);
    
    //    parseSITU(doradeHeader.situ(), hskp);
    
    
}


