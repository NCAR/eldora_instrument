#include <iostream>

#include "DDSPublisher.h"
#include "DDSWriter.h"
#include <boost/program_options.hpp>
#include "ArgvParams.h"
namespace po = boost::program_options;

int main(int argc, char* argv[])
  {

    std::string pulseTopic;
    std::string tsTopic;
    std::string ORB;
    std::string DCPS;
    int delta;

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() ("help", "describe options") ("delta",
        po::value<int>(&delta)->default_value(50000), "publish delta (uSecs)") (
        "pulsetopic", po::value<std::string>(&pulseTopic), "DDS pulse topic")
        ("tstopic", po::value<std::string>(&tsTopic), "DDS time series topic")
        ("ORB", po::value<std::string>(&ORB),"ORB service configuration file (Corba ORBSvcConf arg)")
        ("DCPS", po::value<std::string>(&DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, descripts), vm);
        po::notify(vm);

        if (vm.count("help") || !vm.count("ORB") || !vm.count("DCPS") || !vm.count("pulsetopic") || !vm.count("tstopic"))
          {
            std::cout << descripts << "\n";
            exit(1);
          }

        // we have to do this bit of translation since the 
        // DDS routines want arguments starting with a single dash,
        // whereas boost::program_options uses double dashes.
        ArgvParams pubParams(argv[0]);
        pubParams["-ORBSvcConf"] = ORB;
        pubParams["-DCPSConfigFile"] = DCPS;

        ACE_Time_Value small(0, delta);
        DDSPublisher publisher(pubParams.argc(), pubParams.argv());

        int pubStatus = publisher.status();

        if (pubStatus)
          {
            std::cout << "Unable to run the publsher, status is "
            << pubStatus << std::endl;
            return pubStatus;
          }

        PulseWriter pulseWriter(publisher, pulseTopic);
        TSWriter tsWriter(publisher, tsTopic);

        // number of ts writes per pulse writes
        int tsWrites = 1;
        // pulse counter
        int numPulses = 0;
        // gates in each pulse
        int gates = 1000;
        // iq pairs in each timeseries
        int numiq = 1000;
        // a timestamp
        unsigned long timestamp;

        while ( !pulseWriter.is_finished())
          {
            for (int i = 0; i < 5; i++)
              {
                /////////// Pulses //////////////

                  {
                    while (!pulseWriter.itemsAvailable())
                      {
                        ACE_OS::sleep(small);
                      }

                    EldoraDDS::Pulse* pPulse;
                    // get an available empty pulse from the publisher
                    pPulse = pulseWriter.getEmptyItem();
                    pPulse->abp.length(3*gates);

                    // bump the timestamp on alternating pulses
                    // so that forward and aft share a common timestamp
                    timestamp += (numPulses % 2);
                    numPulses++;

                    for (int n = 0; n < 3*gates; n += 3)
                      {
                        pPulse->abp[n ] = timestamp + n;
                        pPulse->abp[n+1] = timestamp + n+1;
                        pPulse->abp[n+2] = timestamp + n+2;
                      }

                    // set the timestamp
                    pPulse->timestamp = timestamp;
                    // alternate the radar id between forward and aft
                    pPulse->radarId = (numPulses % 2) ? EldoraDDS::Forward : EldoraDDS::Aft;
                    // send the pulse to the PulseWriter
                    pulseWriter.publishItem(pPulse);
                  }

                /////////// TimeSeries //////////////

                  {
                    for (int j = 0; j < tsWrites; j++)
                      {
                        while (!tsWriter.itemsAvailable())
                          {
                            ACE_OS::sleep(small);
                          }
                        EldoraDDS::TimeSeries* pTS;
                        // get an available empty pulse from the publisher
                        pTS = tsWriter.getEmptyItem();
                        pTS->tsdata.length(2*numiq);

                        // don't set any data values right now

                        // set the timestamp
                        pTS->timestamp = timestamp;
                        // alternate the radar id between forward and aft
                        pTS->radarId = (numPulses % 2) ? EldoraDDS::Forward : EldoraDDS::Aft;
                        // send the pulse to the PulseWriter
                        tsWriter.publishItem(pTS);
                      }
                  }
              }
            ACE_OS::sleep(small);
          }

        return 0;
      }
