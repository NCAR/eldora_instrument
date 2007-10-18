// -*- C++ -*-

#include "EldoraReaderListener.h"
#include "PulseTypeSupportC.h"
#include "PulseTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <ace/streams.h>

using namespace EldoraDDS;

////////////////////////////////////////////////////////////

// Implementation skeleton constructor
EldoraReaderListenerImpl::EldoraReaderListenerImpl() :
	num_reads_(0), _numBytes(0) {
	_lastTimestamp[Forward] = 0;
	_lastTimestamp[Aft] = 0;
	_sequenceErrors[Forward] = 0;
	_sequenceErrors[Aft] = 0;
}

////////////////////////////////////////////////////////////

// Implementation skeleton destructor
EldoraReaderListenerImpl::~EldoraReaderListenerImpl() {
}

////////////////////////////////////////////////////////////

void EldoraReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
		throw (CORBA::SystemException) {
	num_reads_ ++;

	try {
		::EldoraDDS::PulseDataReader_var pulse_dr
		= ::EldoraDDS::PulseDataReader::_narrow(reader);
		if (CORBA::is_nil (pulse_dr.in ())) {
			cerr << "read: _narrow failed." << endl;
			exit(1);
		}

		EldoraDDS::Pulse pulse;
		DDS::SampleInfo si;
		DDS::ReturnCode_t status = pulse_dr->take_next_sample(pulse, si);

		// Alternate code to read directlty via the servant
		//EldoraReaderImpl* dr_servant =
		//  reference_to_servant<EldoraReaderImpl>(pulse_dr.in());
		//DDS::ReturnCode_t status = dr_servant->take_next_sample(pulse, si) ;

		if (status == DDS::RETCODE_OK) {
			_numBytes += pulse.abp.length()*sizeof(pulse.abp[0]);

			if (_lastTimestamp[pulse.radarId]+1 != pulse.timestamp) {
				_sequenceErrors[pulse.radarId]++;
			}

			_lastTimestamp[pulse.radarId] = pulse.timestamp;

		} else if (status == DDS::RETCODE_NO_DATA) {
			cerr << "ERROR: reader received DDS::RETCODE_NO_DATA!" << endl;
		} else {
			cerr << "ERROR: read Message: Error: " << status << endl;
		}
	} catch (CORBA::Exception& e) {
		cerr << "Exception caught in read:" << endl << e << endl;
		exit(1);
	}
}

////////////////////////////////////////////////////////////

void EldoraReaderListenerImpl::on_requested_deadline_missed(
		DDS::DataReader_ptr, const DDS::RequestedDeadlineMissedStatus &)
		throw (CORBA::SystemException) {
	cerr << "DataReaderListenerImpl::on_requested_deadline_missed" << endl;
}

////////////////////////////////////////////////////////////

void EldoraReaderListenerImpl::on_requested_incompatible_qos(
		DDS::DataReader_ptr, const DDS::RequestedIncompatibleQosStatus &)
		throw (CORBA::SystemException) {
	cerr << "DataReaderListenerImpl::on_requested_incompatible_qos" << endl;
}

////////////////////////////////////////////////////////////

void EldoraReaderListenerImpl::on_liveliness_changed(DDS::DataReader_ptr,
		const DDS::LivelinessChangedStatus &) throw (CORBA::SystemException) {
	cerr << "DataReaderListenerImpl::on_liveliness_changed" << endl;
}

////////////////////////////////////////////////////////////

void EldoraReaderListenerImpl::on_subscription_match(DDS::DataReader_ptr,
		const DDS::SubscriptionMatchStatus &) throw (CORBA::SystemException) {
	cerr << "DataReaderListenerImpl::on_subscription_match" << endl;
}

////////////////////////////////////////////////////////////

void EldoraReaderListenerImpl::on_sample_rejected(DDS::DataReader_ptr,
		const DDS::SampleRejectedStatus&) throw (CORBA::SystemException) {
	cerr << "EldoraReaderListenerImpl::on_sample_rejected" << endl;
}

////////////////////////////////////////////////////////////

void EldoraReaderListenerImpl::on_sample_lost(DDS::DataReader_ptr,
		const DDS::SampleLostStatus&) throw (CORBA::SystemException) {
	cerr << "EldoraReaderListenerImpl::on_sample_lost" << endl;
}

////////////////////////////////////////////////////////////

long EldoraReaderListenerImpl::numBytes() {
	long current = _numBytes;
	_numBytes = 0;
	return current;
}

////////////////////////////////////////////////////////////

std::map<RadarChoice, long> EldoraReaderListenerImpl::sequenceErrors() {
	std::map<RadarChoice, long> current = _sequenceErrors;
	_sequenceErrors[Forward] = 0;
	_sequenceErrors[Aft] = 0;
	return current;
}

