#include "DDSReader.h"

#include <dds/DCPS/Service_Participant.h>
#include <ace/streams.h>

using namespace DDS;
using namespace CORBA;

////////////////////////////////////////////////////////////

template<TEMPSIG1> DDSReader<TEMPSIG2>::DDSReader(DDSSubscriber& ddsSubscriber,
		std::string topicName):
_numSamples(0),
_droppedSamples(0),
_topicName(topicName)
{

	// reserve the space in the queues
	for (int i = 0; i < 100; i++) {
		DDSTYPE* pItem = new DDSTYPE;
		_inQueue.push_back(pItem);
	}

	Subscriber_var& subscriber = ddsSubscriber.getSubscriber();

	DomainParticipant_var& participant = ddsSubscriber.getParticipant();

	try {
		// register our type
		DDSTYPESUPPORT_VAR typeSupport = new DDSTYPESUPPORTIMPL();
		if (RETCODE_OK != typeSupport->register_type(participant.in (), "")) {
			cerr << "register_type failed for " << _topicName << endl;
			exit(1);
		}

		// get the type name
		String_var type_name = typeSupport->get_type_name ();

		// get the default quality of service
		TopicQos topic_qos;
		participant->get_default_topic_qos(topic_qos);
		// specify reliable reliability. What a concept.
		topic_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;

		// create our topic, using our type name and the default qos. 
		std::cout << "Creating topic " << _topicName << ", for type name "
		<< type_name << std::endl;
		Topic_var topic =
		participant->create_topic (topicName.c_str(),
				type_name.in (),
				topic_qos,
				DDS::TopicListener::_nil());
		if (is_nil (topic.in ())) {
			cerr << "create_topic failed for topic " << _topicName << ", "
			<< " for type name ,  " << type_name << std::endl;
			exit(1);
		}

		// activate the listener
		DDS::DataReaderListener_var listener =
		::OpenDDS::DCPS::servant_to_reference(this);

		if (CORBA::is_nil (listener.in ())) {
			cerr << "listener is nil for " << _topicName << endl;
			exit(1);
		}

		// Create the generic datareader for our topic
		DataReaderQos dw_qos;
		subscriber->get_default_datareader_qos (dw_qos);
		_genericReader = subscriber->create_datareader(topic.in (),
				dw_qos,
				listener.in());
		if (is_nil (_genericReader.in ())) {
			cerr << "create_datawriter failed for topic " << _topicName << ", "
			<< " for type name ,  " << type_name << std::endl;
			exit(1);
		}

	}
	catch (Exception& e)
	{
		cerr << "Exception caught for " << _topicName << ", " << e
		<< " " <<__FILE__ << " line:,  " <<__LINE__ << e << endl;
		exit(1);
	}
}

////////////////////////////////////////////////////////////

// Implementation skeleton destructor
template<TEMPSIG1> DDSReader<TEMPSIG2>::~DDSReader() {
	std::cout << __FUNCTION__ << " destructor\n";
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_data_available(
		DDS::DataReader_ptr reader) throw (CORBA::SystemException) {

	// this will be set true if we need to send a notify
	// after the mutex has been released.
	bool sendNotify = false;
	try {

		/// @todo Is it really necessary to narrow the reader on every entry
		/// to on_data_available? Could this be done once during the setup?
		_specificReader = DDSDATAREADER::_narrow(reader);
		if (CORBA::is_nil (_specificReader.in ())) {
			cerr << "read: _narrow failed in on_data_avaiable(), " << _topicName << ", " << endl;
			exit(1);
		}

		// place the mutex capture inside of the try block so that the mutex 
		// is released before we send the a notify() which could lead to
		// the client trying to access the queus.
		guard_t guard(_queueMutex);

		DDS::SampleInfo si;
		DDS::ReturnCode_t status;

		if (_inQueue.size()) {
			/// @todo we should read all available items
			// we have an available item. read it and place
			// on the queue
			DDSTYPE* pItem = _inQueue[0];
			_inQueue.erase(_inQueue.begin());
			status = _specificReader->take_next_sample(*pItem, si);
			_outQueue.push_back(pItem);
			sendNotify = true;
		} else {
			// no available items, so just read it
			///@todo There must be a call which throws a sample away.
			DDSTYPE item;
			status = _specificReader->take_next_sample(item, si);
			_droppedSamples++;
		}
		// Alternate code to read directlty via the servant
		//EldoraReaderImpl* dr_servant =
		//  reference_to_servant<EldoraReaderImpl>(pulse_dr.in());
		//DDS::ReturnCode_t status = dr_servant->take_next_sample(pulse, si) ;

		if (status == DDS::RETCODE_OK) {
			_numSamples++;
		} else {
			if (status == DDS::RETCODE_NO_DATA) {

				cerr << "ERROR: reader received DDS::RETCODE_NO_DATA!,  " << _topicName << ", " << endl;
			} else {
				cerr << "ERROR: read Message: Error: ,  " << _topicName << ", " << status << endl;
			}
		}
	} catch (CORBA::Exception& e) {
		cerr << "Exception caught in read for " << _topicName << ", " << e << endl;
		exit(1);
	}

	if (sendNotify) {
		notify();
	}
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> int DDSReader<TEMPSIG2>::itemsAvailable()
{
	guard_t guard(_queueMutex);
	return _outQueue.size();
}

////////////////////////////////////////////////////////////

/// Get the next full item from the _outQueue. 
template<TEMPSIG1> DDSTYPE* DDSReader<TEMPSIG2>::getNextItem() {
	guard_t guard(_queueMutex);
	DDSTYPE* pItem;
	if (_outQueue.size()) {
		pItem = _outQueue[0];
		_outQueue.erase(_outQueue.begin());
	} else {
		pItem = 0;
	}

	return pItem;
}

////////////////////////////////////////////////////////////

/// Return an item to be placed back on the _inQueue.
/// @param pItem The item to be returned.
template<TEMPSIG1> void DDSReader<TEMPSIG2>::returnItem(DDSTYPE* pItem) {
	guard_t guard(_queueMutex);

	if (pItem)
	_inQueue.push_back(pItem);
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_requested_deadline_missed(
		DDS::DataReader_ptr, const DDS::RequestedDeadlineMissedStatus &)
throw (CORBA::SystemException) {
	cerr << "DDSReader::on_requested_deadline_missed, " << _topicName << endl;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_requested_incompatible_qos(
		DDS::DataReader_ptr, const DDS::RequestedIncompatibleQosStatus &)
throw (CORBA::SystemException) {
	cerr << "DDSReader::on_requested_incompatible_qos, " << _topicName << endl;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_liveliness_changed(
		DDS::DataReader_ptr, const DDS::LivelinessChangedStatus &)
throw (CORBA::SystemException) {
	cerr << "DDSReader::on_liveliness_changed, " << _topicName << endl;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_subscription_match(
		DDS::DataReader_ptr, const DDS::SubscriptionMatchStatus & status)
throw (CORBA::SystemException ) {
	cerr << "DDSReader::on_subscription_match, " << _topicName << endl;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_sample_rejected(
		DDS::DataReader_ptr, const DDS::SampleRejectedStatus&)
throw (CORBA::SystemException) {
	cerr << "DDSReader::on_sample_rejected, " << _topicName << endl;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::on_sample_lost(
		DDS::DataReader_ptr, const DDS::SampleLostStatus&)
throw (CORBA::SystemException) {
	cerr << "DDSReader::on_sample_lost, " << _topicName << endl;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> unsigned int DDSReader<TEMPSIG2>::numSamples() {
	unsigned int n = _numSamples;
	_numSamples = 0;
	return n;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> unsigned int DDSReader<TEMPSIG2>::droppedSamples() {
	unsigned int n = _droppedSamples;
	_droppedSamples = 0;
	return n;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> void DDSReader<TEMPSIG2>::notify() {
	return;
}

/// @todo These instantiations really belong somewhere else
using namespace EldoraDDS;
template class DDSReader<Pulse, PulseTypeSupportImpl, PulseTypeSupport_var, PulseDataReader, PulseDataReader_var>;
template class DDSReader<TimeSeries, TimeSeriesTypeSupportImpl, TimeSeriesTypeSupport_var, TimeSeriesDataReader, TimeSeriesDataReader_var>;
template class DDSReader<Products, ProductsTypeSupportImpl, ProductsTypeSupport_var, ProductsDataReader, ProductsDataReader_var>;
