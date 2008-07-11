# include <TimetagUtils.h>
# include "DoradeHskp.h"


DoradeHskp::DoradeHskp(const EldoraDDS::Housekeeping& ddsHskp) {
    _ryib = new DoradeRYIB(timetagToPtime(ddsHskp.timetag), ddsHskp.sweepNum,
            ddsHskp.azimuth, ddsHskp.elevation, ddsHskp.peakXmitPower, 
            ddsHskp.trueScanRate, ddsHskp.rayStatus);
    _asib = new DoradeASIB(ddsHskp.longitude, ddsHskp.latitude,
            ddsHskp.altitudeMSL, ddsHskp.altitudeAGL, 
            ddsHskp.groundSpeedEW, ddsHskp.groundSpeedNS, ddsHskp.vertVelocity,
            ddsHskp.heading, ddsHskp.roll, ddsHskp.pitch, ddsHskp.yaw,
            ddsHskp.radarRotAngle, ddsHskp.radarTiltAngle, 
            ddsHskp.windEW, ddsHskp.windNS, ddsHskp.windVert,
            ddsHskp.headingChangeRate, ddsHskp.pitchChangeRate);
    // We start with a dataless FRAD, but our owner can add data to it
    // via frad()->putTwoByteData()
    _frad = new DoradeFRAD(ddsHskp.dataSysStatus, ddsHskp.radarName,
            ddsHskp.testPulsePower, ddsHskp.testPulseStart, 
            ddsHskp.testPulseWidth, ddsHskp.testPulseFreq, 
            ddsHskp.testPulseAtten, ddsHskp.testPulseFNum, 
            ddsHskp.noisePower, ddsHskp.rayCount,
            ddsHskp.firstRecGate, ddsHskp.lastRecGate);
}

DoradeHskp::~DoradeHskp() {
    delete _ryib;
    delete _asib;
    delete _frad;
}

std::ostream&
DoradeHskp::streamTo(std::ostream& os, bool asLittleEndian) const {
    _ryib->streamTo(os, asLittleEndian);
    _asib->streamTo(os, asLittleEndian);
    _frad->streamTo(os, asLittleEndian);
    return os;
}

unsigned int
DoradeHskp::size() const {
    unsigned int nbytes = 0;
    nbytes += _ryib->getDescLen();
    nbytes += _asib->getDescLen();
    nbytes += _frad->getDescLen();
    return nbytes;
}
