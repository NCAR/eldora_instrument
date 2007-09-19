#ifndef WRITER_H
#define WRITER_H

#include <ace/OS_NS_unistd.h>
#include <ace/streams.h>
#include <unistd.h>
#include "PulseTypeSupportC.h"
#include <dds/DdsDcpsPublicationC.h>
#include <ace/Task.h>
#include <ace/Synch.h>
#include <vector>

typedef ACE_Thread_Mutex mutex_t;
typedef ACE_Condition_Thread_Mutex condition_t;
typedef ACE_Guard<mutex_t> guard_t;
typedef EldoraDDS::Pulse pulse_t;

class EldoraWriter : public ACE_Task_Base
{

 public:

  EldoraWriter (DDS::DataWriter_ptr writer);

  void start ();

  void end();

  virtual int svc ();

  bool is_finished () const;

  int get_timeout_writes () const;

  void publishPulse(pulse_t* pPulse);

  int pulsesAvailable();

  pulse_t* getEmptyPulse();

 private:

  void waitForPulse();

  bool publish(::DDS::InstanceHandle_t handle);

  DDS::DataWriter_var writer_;

  EldoraDDS::PulseDataWriter_var pulse_dw;

  ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> finished_instances_;

  ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> timeout_writes_;

  mutex_t _mutex;

  condition_t _condition;

  std::vector<pulse_t*> _inQueue;

  std::vector<pulse_t*> _outQueue;

  int _gates;

};

#endif

