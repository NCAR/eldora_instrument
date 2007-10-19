// -*- C++ -*-
//
#ifndef ELDORADATAREADER_LISTENER_IMPL
#define ELDORADATAREADER_LISTENER_IMPL

#include <dds/DdsDcpsSubscriptionS.h>
#include "EldoraDdsC.h"
#include <map>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

using namespace EldoraDDS;

//Class EldoraReaderListenerImpl
class EldoraReaderListenerImpl
  : public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener>
{
public:
  //Constructor
  EldoraReaderListenerImpl ();

  //Destructor
  virtual ~EldoraReaderListenerImpl (void);

  virtual void on_requested_deadline_missed (
					     DDS::DataReader_ptr reader,
					     const DDS::RequestedDeadlineMissedStatus & status)
    throw (CORBA::SystemException);

  virtual void on_requested_incompatible_qos (
					      DDS::DataReader_ptr reader,
					      const DDS::RequestedIncompatibleQosStatus & status)
    throw (CORBA::SystemException);

  virtual void on_liveliness_changed (
				      DDS::DataReader_ptr reader,
				      const DDS::LivelinessChangedStatus & status)
    throw (CORBA::SystemException);

  virtual void on_subscription_match (
				      DDS::DataReader_ptr reader,
				      const DDS::SubscriptionMatchStatus & status
				      )
    throw (CORBA::SystemException);

  virtual void on_sample_rejected(
				  DDS::DataReader_ptr reader,
				  const DDS::SampleRejectedStatus& status
				  )
    throw (CORBA::SystemException);

  virtual void on_data_available(
				 DDS::DataReader_ptr reader
				 )
    throw (CORBA::SystemException);

  virtual void on_sample_lost(
			      DDS::DataReader_ptr reader,
			      const DDS::SampleLostStatus& status
			      )
    throw (CORBA::SystemException);

  long num_reads() const {
    return num_reads_;
  }

  long _numBytes;
  long numBytes();

  std::map<RadarChoice, long long> _lastTimestamp;
  std::map<RadarChoice, long> _sequenceErrors;
  std::map<RadarChoice, long> sequenceErrors();

private:

  DDS::DataReader_var reader_;
  long                num_reads_;
  // true if a pulse reader, false if a time series reader
  bool                pulse_reader_;
};

#endif /* DATAREADER_LISTENER_IMPL  */
