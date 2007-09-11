#ifndef ELDORAPUBLISHERINC_
#define ELDORAPUBLISHERINC_

#include "PulseTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/PublisherImpl.h>
#include <dds/DCPS/transport/framework/TheTransportFactory.h>
#include <dds/DCPS/transport/simpleTCP/SimpleTcpConfiguration.h>
#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/simpleTCP/SimpleTcp.h>
#include <dds/DCPS/transport/simpleUnreliableDgram/SimpleUnreliableDgram.h>
#include <dds/DCPS/transport/ReliableMulticast/ReliableMulticast.h>
#endif

#include <ace/Synch.h>

#include "EldoraWriter.h"

class EldoraPublisher {
  typedef ACE_Thread_Mutex mutex_t;
  typedef ACE_Condition_Thread_Mutex condition_t;
  typedef ACE_Guard<mutex_t> guard_t;

 public:
  EldoraPublisher();
  virtual ~EldoraPublisher();
  int run(int argc, char* argv[]);
  bool isFinished();
  void newData(int i);

 protected:
  int parse_args (int argc, char *argv[]);
  DDS::DomainParticipant_var _participant;
  DDS::DomainParticipantFactory_var _dpf;
  DDS::DataWriter_var dw;
  EldoraWriter* _writer;
  mutex_t _mutex;
  condition_t* _condition;
  std::vector<int> _queue;

};

#endif
