#include "DrxRPC.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include <sys/time.h>
#include <time.h>

///////////////////////////////////////////////////////////////////
DrxRPC::DrxRPC(int rpcport,
               bool* startFlag,
               bool* stopFlag,
               bool* shutdownFlag,
               EldoraRadarParams* radarParams,
               std::vector<unsigned long> (*bytesFunction)(int boardNum)) :
    _port(rpcport), _startFlag(startFlag), _stopFlag(stopFlag),
            _shutdownFlag(shutdownFlag), _radarParams(radarParams),
            _bytesFunction(bytesFunction), _server(_port, 0),
            _startCmd(&_server, *this, "start", &DrxRPC::start),
            _stopCmd(&_server, *this, "stop", &DrxRPC::stop),
            _shutdownCmd(&_server, *this, "shutdown", &DrxRPC::shutdown),
            _paramsCmd(&_server, *this, "params", &DrxRPC::params),
            _statusCmd(&_server, *this, "status", &DrxRPC::status)
{

    struct timeval currentTimeTv;
    gettimeofday(&currentTimeTv, 0);
    _lastTime = currentTimeTv.tv_sec + currentTimeTv.tv_usec/1.0e6;

    // start the server (in its own thread)
    _server.start();
}

///////////////////////////////////////////////////////////////////
DrxRPC::~DrxRPC()
{

}

///////////////////////////////////////////////////////////////////
void DrxRPC::serverShutdown()
{
    std::cout << "RPC serverShutdown received\n";
    _server.shutdown();
}

///////////////////////////////////////////////////////////////////
void DrxRPC::serverExit()
{
    std::cout << "RPC serverExit received\n";
    _server.exit();
}

///////////////////////////////////////////////////////////////////
void DrxRPC::start(XmlRpc::XmlRpcValue& params,
                   XmlRpc::XmlRpcValue& result)
{
    std::cout << "RPC start received\n";
    *_startFlag = true;
    result = "eldoradrx received start command";
}

///////////////////////////////////////////////////////////////////
void DrxRPC::stop(XmlRpc::XmlRpcValue& params,
                  XmlRpc::XmlRpcValue& result)
{
    std::cout << "RPC stop received\n";
    *_stopFlag = true;
    result = "eldoradrx received stop command";
}

///////////////////////////////////////////////////////////////////
void DrxRPC::shutdown(XmlRpc::XmlRpcValue& params,
                      XmlRpc::XmlRpcValue& result)
{
    std::cout << "RPC shutdown received\n";
    result = "eldoradrx received shutdown command";
    *_shutdownFlag = 0;
}

///////////////////////////////////////////////////////////////////
void DrxRPC::status(XmlRpc::XmlRpcValue& params,
                    XmlRpc::XmlRpcValue& result)
{
    XmlRpc::XmlRpcValue retval;

    // get the current byte counts from the board
    std::vector<unsigned long> bytes[2];
    bytes[0] = _bytesFunction(0);
    bytes[1] = _bytesFunction(1);

    struct timeval currentTimeTv;
    gettimeofday(&currentTimeTv, 0);
    double currentTime = currentTimeTv.tv_sec + currentTimeTv.tv_usec/1.0e6;

    // calculate the rates
    double deltaT = currentTime - _lastTime;
    _lastTime = currentTime;
    std::vector<double> rates[2];
    for (unsigned int i = 0; i < 2; i++) {
        rates[i].resize(8);
        for (unsigned int j = 0; j < 8; j++) {
            rates[i][j] = bytes[i][j]/deltaT;
        }
    }

    // create return values for each
    for (unsigned int i = 0; i < 8; i++) {
        std::stringstream key0, key1;
        key0 << "board0-" << i;
        retval[key0.str()] = rates[0][i]/1.0e6;
        key1 << "board1-" << i;
        retval[key1.str()] = rates[1][i]/1.0e6;
    }

    // return it
    result = retval;
}

///////////////////////////////////////////////////////////////////
void DrxRPC::params(XmlRpc::XmlRpcValue& params,
                    XmlRpc::XmlRpcValue& result)
{
    std::cout << "RPC params received\n";

    // Decode the radar header information. This code is
    // VERY dependent on the header blocks and fields
    // to arrive in the correct order. The WAVE block must
    // come first. The for or aft RADD blocks can then 
    // follow, with all of the parameters for that radar
    // following the RADD block. Within a PARM block, the 
    // PARMNAME must come before the PARMBIAS and PARMSCALE.
    
    // We will be filling parameters for both
    // radars at the beginning, for the WAVE block.
    // When we get a RADDNAME field, we will select either 
    // the forward or aft radar.
    enum {FORE, AFT, BOTH} radarChoice = BOTH;
    
    for (int i = 0; i < params[0].size(); i++) {

        std::string key = params[0][i][0];
        XmlRpc::XmlRpcValue& values = params[0][i][1];

        // look for radar name
        if (!key.compare("RADDNAME")) {
            std::string s = values[0];
            if (!s.compare("AFT")) {
                radarChoice = AFT;
                std::cout << "processing forward radar header\n";
            } else { 
                radarChoice = FORE;
                std::cout << "processing aft radar header\n";
            }
            continue;
        }
        
        switch (radarChoice) {
            case BOTH:
                setRadarParams(&_radarParams[0], key, values);
                setRadarParams(&_radarParams[1], key, values);
                break;
            case FORE:
                setRadarParams(&_radarParams[0], key, values);
                break;
            case AFT:
                setRadarParams(&_radarParams[1], key, values);
                break;
                
        }
    }

    std::cout << "**** forward radar:\n";
    _radarParams[0].dump();
    std::cout << "**** aft radar:\n";
    _radarParams[1].dump();
    std::cout.flush();

    result = 1;
}
///////////////////////////////////////////////////////////////////
bool DrxRPC::setRadarParams(EldoraRadarParams* p,
                            std::string key,
                            XmlRpc::XmlRpcValue& values)
{

    bool retval = false;

    // Map each identifier onto the field in _radarParams.
    // I know, this is expensive to do for each call, but 
    // it doesn't happen very often....

    // Map the integer types
    std::map<std::string, int*> intMap;
    // Map the double types
    std::map<std::string, double*> doubleMap;

    intMap["WAVENCHIPS"] = p->wave_nchips; // {2,2,2,2,0,2} 
    intMap["WAVECHPOFF"] = p->wave_chpoff; // {17,77,137,197,0,0} 
    intMap["WAVECHPWID"] = p->wave_chpwid; // {60,60,60,60,0,257} 
    intMap["WAVENGATES"] = p->wave_ngates; // {376,376,376,376,0} 
    intMap["WAVEGATE1"] = p->wave_gate1; // {90,60} 
    intMap["WAVEGATE2"] = p->wave_gate2; // {90,60} 
    intMap["WAVEGATE3"] = p->wave_gate3; // {90,60} 
    intMap["WAVEGATE4"] = p->wave_gate4; // {90,60} 
    intMap["WAVEGATE5"] = p->wave_gate5; // {-999,-999} 
    doubleMap["WAVEMSREP"] = &p->wave_msrep; // {1.125}
    intMap["WAVESEQREP"]= &p->wave_seqrep; // {45}

    doubleMap["RADDCONST"] = &p->radd_const; // {-80.290924} 
    doubleMap["RADDNOIPOW"] = &p->radd_noipow; // {-112.920517} 
    doubleMap["RADDUNVEL"] = &p->radd_unvel; // {77.226303} 
    doubleMap["RADDUNRNG"] = &p->radd_unrng; // {60.000000} 
    intMap["RADDNFREQ"] = &p->radd_nfreq; // {4} 
    intMap["RADDNIPP"] = &p->radd_nipp; // {2} 
    doubleMap["RADDFREQ1"] = &p->radd_freq1; // {9.710000} 
    doubleMap["RADDFREQ2"] = &p->radd_freq2; // {9.700000} 
    doubleMap["RADDFREQ3"] = &p->radd_freq3; // {9.690000} 
    doubleMap["RADDFREQ4"] = &p->radd_freq4; // {9.720000} 
    doubleMap["RADDFREQ5"] = &p->radd_freq5; // {-999.000000} 
    doubleMap["RADDIPP1"] = &p->radd_ipp1; // {0.400000} 
    doubleMap["RADDIPP2"] = &p->radd_ipp2; // {0.500000} 
    doubleMap["RADDIPP3"] = &p->radd_ipp3; // {-999.000000} 
    doubleMap["RADDIPP4"] = &p->radd_ipp4; // {-999.000000} 
    doubleMap["RADDIPP5"] = &p->radd_ipp5; // {-999.000000} 
    doubleMap["RADDROTVEL"] = &p->radd_rotvel; // 50.00000

    doubleMap["FRIBLNALOS"] = &p->frib_lnalos; // {1.097910} 
    doubleMap["FRIBXGAIN"] = &p->frib_xgain; // {21.100000} 
    doubleMap["FRIBRXGAIN"] = p->frib_rxgain; // {30.770000,27.959999,30.480000,30.670000,30.000000} 
    doubleMap["FRIBENCANG"] = &p->frib_encang; // 276.538086

    doubleMap["CSPDWIDTH"] = p->cspd_width; // 150 150 150 150 -999 -999
    doubleMap["CSPD1STGAT"] = &p->cspd_1stgat; // 225
    
    doubleMap["PARMSCALEVS"] = &p->parm_vs_scale; // {424.259552} 
    doubleMap["PARMBIASVS"] = &p->parm_vs_bias; // {8191.000000} 
    doubleMap["PARMSCALEVL"] = &p->parm_vl_scale; // {530.324463} 
    doubleMap["PARMBIASVL"] = &p->parm_vl_bias; // {8191.000000} 
    doubleMap["PARMSCALESW"] = &p->parm_sw_scale; // {65.425186} 
    doubleMap["PARMBIASSW"] = &p->parm_sw_bias; // {0.000000} 
    doubleMap["PARMSCALEVR"] = &p->parm_vr_scale; // {424.298431} 
    doubleMap["PARMBIASVR"] = &p->parm_vr_bias; // {32767.000000} 
    doubleMap["PARMSCALENCP"] = &p->parm_ncp_scale; // {1024.000000} 
    doubleMap["PARMBIASNCP"] = &p->parm_ncp_bias; // {0.000000} 
    doubleMap["PARMSCALEDBZ"] = &p->parm_dbz_scale; // {8.000000} 
    doubleMap["PARMBIASDBZ"] = &p->parm_dbz_bias; // {280.000000} 

    std::map<std::string, int*>::iterator intIt = intMap.find(key);
    if (intIt != intMap.end()) {
        retval = true;
        for (int i = 0; i < values.size(); i++) {
            std::string s = values[i];
            intIt->second[i] = atoi(s.c_str());
        }
    }

    std::map<std::string, double*>::iterator doubleIt = doubleMap.find(key);
    if (doubleIt != doubleMap.end()) {
        retval = true;
        for (int i = 0; i < values.size(); i++) {
            std::string s = values[i];
            doubleIt->second[i] = atof(s.c_str());
        }
    }

    if (!retval) 
        std::cout << "*** unable to match header key " << key << "\n";
    return retval;
}
