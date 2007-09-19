#ifndef ELDORASUBSCRIBERINC_
#define ELDORASUBSCRIBERINC_

#include "EldoraReaderListener.h"
#include "PulseTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/SubscriberImpl.h>
#include <dds/DCPS/transport/framework/TheTransportFactory.h>
#include <dds/DCPS/transport/simpleTCP/SimpleTcpConfiguration.h>
#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/simpleTCP/SimpleTcp.h>
#include <dds/DCPS/transport/simpleUnreliableDgram/SimpleUnreliableDgram.h>
#include <dds/DCPS/transport/ReliableMulticast/ReliableMulticast.h>
#endif

class EldoraSubscriber {

 public:

  EldoraSubscriber();

  virtual ~EldoraSubscriber();

  int run(int argc, char* argv[]);

  int numBytes();

  std::map<RadarChoice, long> sequenceErrors();

 protected:

  int parse_args (int argc, char *argv[]);

  DDS::DomainParticipant_var _participant;

  DDS::DomainParticipantFactory_var _dpf;

  EldoraReaderListenerImpl _listenerServant;

};


#endif
