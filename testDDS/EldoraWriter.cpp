// -*- C++ -*-
//
// $Id: Writer.cpp 784 2007-05-30 19:26:41Z harriss $
#include "EldoraWriter.h"
#include "PulseTypeSupportC.h"
#include <ace/OS_NS_unistd.h>
#include <ace/streams.h>
#include <unistd.h>

using namespace EldoraDDS;

const int num_instances_per_writer = 1;
const int num_messages = 1000;

EldoraWriter::EldoraWriter(::DDS::DataWriter_ptr writer, int sleepUsecs)
  : writer_ (::DDS::DataWriter::_duplicate (writer)),
    finished_instances_ (0),
    timeout_writes_ (0),
    _sleepUsecs(sleepUsecs)
{
}

void
EldoraWriter::start ()
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Writer::start \n")));
  // Lanuch num_instances_per_writer threads.
  // Each thread writes one instance which uses the thread id as the
  // key value.
  if (activate (THR_NEW_LWP | THR_JOINABLE, num_instances_per_writer) == -1) {
    cerr << "Writer::start(): activate failed" << endl;
    exit(1);
  }
}

void
EldoraWriter::end ()
{
  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT("(%P|%t) Writer::end \n")));
  wait ();
}


int
EldoraWriter::svc ()
{
  ACE_DEBUG((LM_DEBUG,
	     ACE_TEXT("(%P|%t) Writer::svc begins.\n")));

  ::DDS::InstanceHandleSeq handles;
  try {

    while (1)
      {
	writer_->get_matched_subscriptions(handles);
	if (handles.length() > 0)
	  break;
	else
	  ACE_OS::sleep(ACE_Time_Value(0,200000));
      }

    ::EldoraDDS::PulseDataWriter_var pulse_dw
	= ::EldoraDDS::PulseDataWriter::_narrow(writer_.in());
    if (CORBA::is_nil (pulse_dw.in ())) {
      cerr << "Data Writer could not be narrowed"<< endl;
      exit(1);
    }

    EldoraDDS::Pulse pulse;
    ::DDS::InstanceHandle_t handle = pulse_dw->_cxx_register (pulse);

    pulse.timestamp  = 1;
    int gates = 1500;
    pulse.abp.length(3*gates);

    ACE_DEBUG((LM_DEBUG,
	       ACE_TEXT("%T (%P|%t) Writer::svc starting to write.\n")));
    int i = 0;

    while(1) {
      i++;

      for (int n = 0; n < 3*gates; n += 3) {
	pulse.abp[n  ] = (short)pulse.timestamp + n;
	pulse.abp[n+1] = (short)pulse.timestamp + n+1;
	pulse.abp[n+2] = (short)pulse.timestamp + n+2;
      }

      pulse.timestamp++;

      pulse.radarId = Forward;

      ::DDS::ReturnCode_t ret;

      ret = pulse_dw->write(pulse, handle);
    
      if (ret != ::DDS::RETCODE_OK) {
	ACE_ERROR ((LM_ERROR,
		    ACE_TEXT("(%P|%t)ERROR  EldoraWriter::svc, ")
		    ACE_TEXT ("%dth write() returned %d.\n"),
		    i, ret));
	if (ret == ::DDS::RETCODE_TIMEOUT) {
	  timeout_writes_ ++;
	}
      }

      pulse.radarId = Aft;

      ret = pulse_dw->write(pulse, handle);
    
      if (ret != ::DDS::RETCODE_OK) {
	ACE_ERROR ((LM_ERROR,
		    ACE_TEXT("(%P|%t)ERROR  EldoraWriter::svc, ")
		    ACE_TEXT ("%dth write() returned %d.\n"),
		    i, ret));
	if (ret == ::DDS::RETCODE_TIMEOUT) {
	  timeout_writes_ ++;
	}
      }

      ACE_OS::sleep(ACE_Time_Value(0,_sleepUsecs));

    }
  } catch (CORBA::Exception& e) {
    cerr << "Exception caught in svc:" << endl
	 << e << endl;
  }

  while (1)
    {
      writer_->get_matched_subscriptions(handles);
      if (handles.length() == 0)
        break;
      else
        ACE_OS::sleep(1);
    }
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Writer::svc finished.\n")));

  finished_instances_ ++;

  return 0;
}


bool
EldoraWriter::is_finished () const
{
  return finished_instances_ == num_instances_per_writer;
}

int
EldoraWriter::get_timeout_writes () const
{
  return timeout_writes_.value ();
}
