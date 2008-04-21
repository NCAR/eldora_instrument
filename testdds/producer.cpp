#include <iostream>

#include "DDSPublisher.h"
#include "DDSWriter.h"
#include <boost/program_options.hpp>
#include "ArgvParams.h"
namespace po = boost::program_options;

int main(int argc, char* argv[])
  {

    std::string rayTopic;
    std::string tsTopic;
    std::string ORB;
    std::string DCPS;
    int delta;

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() ("help", "describe options") ("delta",
        po::value<int>(&delta)->default_value(50000), "publish delta (uSecs)") (
        "raytopic", po::value<std::string>(&rayTopic), "DDS ray topic")
        ("tstopic", po::value<std::string>(&tsTopic), "DDS time series topic")
        ("ORB", po::value<std::string>(&ORB),"ORB service configuration file (Corba ORBSvcConf arg)")
        ("DCPS", po::value<std::string>(&DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, descripts), vm);
        po::notify(vm);

        if (vm.count("help") || !vm.count("ORB") || !vm.count("DCPS") || !vm.count("raytopic") || !vm.count("tstopic"))
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

        RayWriter rayWriter(publisher, rayTopic);
        TSWriter tsWriter(publisher, tsTopic);

        // number of ts writes per ray writes
        int tsWrites = 1;
        // ray counter
        int numRays = 0;
        // gates in each ray
        int gates = 1000;
        // iq pairs in each timeseries
        int numiq = 1000;
        // a timestamp
        unsigned long timestamp = 0;

        while ( !rayWriter.is_finished())
          {
            for (int i = 0; i < 5; i++)
              {
                /////////// Rays //////////////

                  {
                    while (!rayWriter.itemsAvailable())
                      {
                        ACE_OS::sleep(small);
                      }

                    EldoraDDS::Ray* pRay;
                    // get an available empty ray from the publisher
                    pRay = rayWriter.getEmptyItem();
                    pRay->abp.length(3*gates);

                    // bump the timestamp on alternating rays
                    // so that forward and aft share a common timestamp
                    timestamp += (numRays % 2);
                    numRays++;

                    for (int n = 0; n < 3*gates; n += 3)
                      {
                        pRay->abp[n ] = timestamp + n;
                        pRay->abp[n+1] = timestamp + n+1;
                        pRay->abp[n+2] = timestamp + n+2;
                      }

                    // set the timestamp
                    pRay->rayNum = timestamp;
                    // alternate the radar id between forward and aft
                    pRay->radarId = (numRays % 2) ? EldoraDDS::Forward : EldoraDDS::Aft;
                    // send the ray to the RayWriter
                    rayWriter.publishItem(pRay);
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
                        // get an available empty ray from the publisher
                        pTS = tsWriter.getEmptyItem();
                        pTS->tsdata.length(2*numiq);

                        // don't set any data values right now

                        // set the timestamp
                        pTS->rayNum = timestamp;
                        // alternate the radar id between forward and aft
                        pTS->radarId = (numRays % 2) ? EldoraDDS::Forward : EldoraDDS::Aft;
                        // send the ray to the RayWriter
                        tsWriter.publishItem(pTS);
                      }
                  }
              }
            ACE_OS::sleep(small);
          }

        return 0;
      }
