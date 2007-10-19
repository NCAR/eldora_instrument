#include "DDSReader.h"

#include <dds/DCPS/Service_Participant.h>
#include <ace/streams.h>

using namespace DDS;
using namespace CORBA;

////////////////////////////////////////////////////////////

template<TEMPSIG1> DDSReader<TEMPSIG2>::DDSReader(DDSSubscriber& ddsSubscriber, std::string topicName) {

	// reserve the space in the queues
	for (int i = 0; i < 100; i++) {
		DDSTYPE* pItem = new DDSTYPE;
		_outQueue.push_back(pItem);
	}

	Subscriber_var& subscriber = ddsSubscriber.getSubscriber();

	DomainParticipant_var& participant = ddsSubscriber.getParticipant();

	try {
		// register our type
		DDSTYPESUPPORT_VAR typeSupport = new DDSTYPESUPPORTIMPL();
		if (RETCODE_OK != typeSupport->register_type(participant.in (), "")) {
			cerr << "register_type failed." << endl;
			exit(1);
		}

		// get the type name
		String_var type_name = typeSupport->get_type_name ();

		// get the default quality of service
		TopicQos topic_qos;
		participant->get_default_topic_qos(topic_qos);

		// create our topic, using our type name and the default qos. 
		std::cout << "Creating topic " << topicName.c_str()
		<< " for type name " << type_name << std::endl;
		Topic_var topic =
		participant->create_topic (topicName.c_str(),
				type_name.in (),
				topic_qos,
				DDS::TopicListener::_nil());
		if (is_nil (topic.in ())) {
			cerr << "create_topic failed for topic " << topicName.c_str()
			<< " for type name " << type_name << std::endl;
			exit(1);
		}

		// activate the listener
		DDS::DataReaderListener_var listener =
		::OpenDDS::DCPS::servant_to_reference(this);

		if (CORBA::is_nil (listener.in ())) {
			cerr << "listener is nil." << endl;
			exit(1);
		}

		// Create the generic datareader for our topic
		DataReaderQos dw_qos;
		subscriber->get_default_datareader_qos (dw_qos);
		_genericReader = subscriber->create_datareader(topic.in (),
				dw_qos,
				listener.in());
		if (is_nil (_genericReader.in ())) {
			cerr << "create_datawriter failed for topic " << topicName.c_str()
			<< " for type name " << type_name << std::endl;
			exit(1);
		}

	}
	catch (Exception& e)
	{
		cerr << "Exception caught in " << __FILE__
		<< " line:" << __LINE__
		<< e << endl;
		exit(1);
	}
}

////////////////////////////////////////////////////////////

// Implementation skeleton destructor
template<TEMPSIG1> DDSReader<TEMPSIG2>::~DDSReader() {
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_data_available(
		DDS::DataReader_ptr reader) throw (CORBA::SystemException) {
	try {
		_specificReader = DDSDATAREADER::_narrow(reader);
		if (CORBA::is_nil (_specificReader.in ())) {
			cerr << "read: _narrow failed in on_data_avaiable()" << endl;
			exit(1);
		}

		_numSamples++;
		
		DDS::SampleInfo si;
		DDS::ReturnCode_t status;
		DDSTYPE item;
		status = _specificReader->take_next_sample(item, si);

		// Alternate code to read directlty via the servant
		//EldoraReaderImpl* dr_servant =
		//  reference_to_servant<EldoraReaderImpl>(pulse_dr.in());
		//DDS::ReturnCode_t status = dr_servant->take_next_sample(pulse, si) ;

		if (status == DDS::RETCODE_OK) {
			_numSamples++;
		} else {
			if (status == DDS::RETCODE_NO_DATA) {

				cerr << "ERROR: reader received DDS::RETCODE_NO_DATA!" << endl;
			} else {
				cerr << "ERROR: read Message: Error: " << status << endl;
			}
		}
	} catch (CORBA::Exception& e) {
		cerr << "Exception caught in read:" << endl << e << endl;
		exit(1);
	}
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_requested_deadline_missed(
		DDS::DataReader_ptr, const DDS::RequestedDeadlineMissedStatus &)
throw (CORBA::SystemException) {
	cerr << "DDSReader::on_requested_deadline_missed" << endl;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_requested_incompatible_qos(
		DDS::DataReader_ptr, const DDS::RequestedIncompatibleQosStatus &)
throw (CORBA::SystemException) {
	cerr << "DDSReader::on_requested_incompatible_qos" << endl;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_liveliness_changed(
		DDS::DataReader_ptr, const DDS::LivelinessChangedStatus &)
throw (CORBA::SystemException) {
	cerr << "DDSReader::on_liveliness_changed" << endl;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_subscription_match(
		DDS::DataReader_ptr, const DDS::SubscriptionMatchStatus &)
throw (CORBA::SystemException) {
	cerr << "DDSReader::on_subscription_match" << endl;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_sample_rejected(
		DDS::DataReader_ptr, const DDS::SampleRejectedStatus&)
throw (CORBA::SystemException) {
	cerr << "DDSReader::on_sample_rejected" << endl;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_sample_lost(
		DDS::DataReader_ptr, const DDS::SampleLostStatus&)
throw (CORBA::SystemException) {
	cerr << "DDSReader::on_sample_lost" << endl;
}

template<TEMPSIG1> unsigned int DDSReader<TEMPSIG2>::numSamples() {
	unsigned int n = _numSamples;
	_numSamples = 0;
	return n;
}

/// @todo These instantiations really belong somewhere else
#include "PulseTypeSupportC.h"
#include "PulseTypeSupportImpl.h"
#include "TimeSeriesTypeSupportC.h"
#include "TimeSeriesTypeSupportImpl.h"
using namespace EldoraDDS;
template class DDSReader<Pulse, PulseTypeSupportImpl, PulseTypeSupport_var, PulseDataReader, PulseDataReader_var>;
template class DDSReader<TimeSeries, TimeSeriesTypeSupportImpl, TimeSeriesTypeSupport_var, TimeSeriesDataReader, TimeSeriesDataReader_var>;
