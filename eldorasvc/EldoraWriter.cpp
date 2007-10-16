#include "EldoraWriter.h"
#include "PulseTypeSupportC.h"

using namespace EldoraDDS;

const int num_instances_per_writer = 1;

////////////////////////////////////////////////////////////

template<TEMPSIG1> 
EldoraWriter<TEMPSIG2>::EldoraWriter(
		EldoraPublisher& eldoraPublisher) :
	_condition(_mutex), finished_instances_(0), timeout_writes_(0) {

	// reserve the space in the queues
	for (int i = 0; i < 100; i++) {
		DDSTYPE* pItem = new DDSTYPE;
		_outQueue.push_back(pItem);
	}

	DDS::Publisher_var& publisher = eldoraPublisher.getPublisher();

	DDS::DomainParticipant_var& participant = eldoraPublisher.getParticipant();

	try {
		DDSTYPESUPPORT_VAR mts = new DDSTYPESUPPORTIMPL();
		if (DDS::RETCODE_OK != mts->register_type(participant.in (), "")) {
			cerr << "register_type failed." << endl;
			exit(1);
		}

		CORBA::String_var type_name = mts->get_type_name ();

		DDS::TopicQos topic_qos;
		participant->get_default_topic_qos(topic_qos);
		DDS::Topic_var topic =
		participant->create_topic ("testtopic",
				type_name.in (),
				topic_qos,
				DDS::TopicListener::_nil());
		if (CORBA::is_nil (topic.in ())) {
			cerr << "create_topic failed." << endl;
			exit(1);
		}

		// Create the datawriter
		DDS::DataWriterQos dw_qos;
		publisher->get_default_datawriter_qos (dw_qos);
		DDS::DataWriter_var dw = publisher->create_datawriter(topic.in (),
				dw_qos,
				DDS::DataWriterListener::_nil());
		if (CORBA::is_nil (dw.in ())) {
			cerr << "create_datawriter failed." << endl;
			exit(1);
		}

	}
	catch (CORBA::Exception& e)
	{
		cerr << "PUB: Exception caught in main.cpp:" << endl
		<< e << endl;
		exit(1);
	}
	
	// start the writer
	start();
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> 
void 
EldoraWriter<TEMPSIG2>::start() {
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Writer::start \n")));
	// Lanuch num_instances_per_writer threads.
	// Each thread writes one instance which uses the thread id as the
	// key value.
	if (activate(THR_NEW_LWP | THR_JOINABLE, num_instances_per_writer) == -1) {
		cerr << "Writer::start(): activate failed" << endl;
		exit(1);
	}
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> 
void 
EldoraWriter<TEMPSIG2>::end() {
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Writer::end \n")));
	wait();
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> 
int 
EldoraWriter<TEMPSIG2>::svc() {
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Writer::svc begins.\n")));

	DDS::InstanceHandleSeq handles;
	try {

		while (1)
		{
			writer_->get_matched_subscriptions(handles);
			if (handles.length() > 0)
			break;
			else
			ACE_OS::sleep(ACE_Time_Value(0,200000));
		}

		item_dw = DDSDATAWRITER::_narrow(writer_.in());
		if (CORBA::is_nil (item_dw.in ())) {
			cerr << "Data Writer could not be narrowed"<< endl;
			exit(1);
		}

		DDSTYPE item;
		DDS::InstanceHandle_t handle = item_dw->_cxx_register (item);

		ACE_DEBUG((LM_DEBUG,
						ACE_TEXT("%T (%P|%t) Writer::svc starting to write.\n")));
		int i = 0;

		while(1) {
			waitForItem();
			while(publish(handle)) {};
		}
	} catch (CORBA::Exception& e) {
		cerr << "Exception caught in svc:" << endl
		<< e << endl;
	}

	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Writer::svc finished.\n")));

	finished_instances_ ++;

	return 0;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> 
bool 
EldoraWriter<TEMPSIG2>::is_finished() const {
	return finished_instances_ == num_instances_per_writer;
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> 
int 
EldoraWriter<TEMPSIG2>::get_timeout_writes() const {
	return timeout_writes_.value();
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> 
int 
EldoraWriter<TEMPSIG2>::itemsAvailable() {

	guard_t guard(_mutex);

	return _outQueue.size();

}
////////////////////////////////////////////////////////////

template<TEMPSIG1> 
DDSTYPE* 
EldoraWriter<TEMPSIG2>::getEmptyItem() {

	guard_t guard(_mutex);

	if (_outQueue.size() == 0)
		return 0;

	DDSTYPE* pItem = _outQueue[0];
	_outQueue.erase(_outQueue.begin());

	//std:cout << "pulse removed from outQueue " << _outQueue.size() << "\n";

	return pItem;

}
////////////////////////////////////////////////////////////

template<TEMPSIG1>
void 
EldoraWriter<TEMPSIG2>::publishItem(
		DDSTYPE* pItem) {

	guard_t guard(_mutex);

	_inQueue.push_back(pItem);

	_condition.broadcast();
}

////////////////////////////////////////////////////////////

template<TEMPSIG1> 
void 
EldoraWriter<TEMPSIG2>::waitForItem() {

	guard_t guard(_mutex);

	while (_inQueue.size() == 0)
		_condition.wait();

}

////////////////////////////////////////////////////////////

template<TEMPSIG1> 
bool 
EldoraWriter<TEMPSIG2>::publish(
		DDS::InstanceHandle_t handle) {

	guard_t guard(_mutex);

	if (_inQueue.size() == 0)
		return false;

	DDSTYPE* pItem = _inQueue[0];

	_inQueue.erase(_inQueue.begin());

	DDS::ReturnCode_t ret;

	ret = item_dw->write(*pItem, handle);

	if (ret != DDS::RETCODE_OK) {
		ACE_ERROR ((LM_ERROR,
						ACE_TEXT("(%P|%t)ERROR  EldoraWriter::svc, ")
						ACE_TEXT ("write() returned %d.\n"),
						ret));
		if (ret == DDS::RETCODE_TIMEOUT) {
			timeout_writes_ ++;
		}
	}

	_outQueue.push_back(pItem);

	//std::cout << "pulse returned to outQueue\n";

	return (_inQueue.size() != 0);
}

template class EldoraWriter<Pulse, PulseTypeSupportImpl, PulseTypeSupport_var, PulseDataWriter>;
