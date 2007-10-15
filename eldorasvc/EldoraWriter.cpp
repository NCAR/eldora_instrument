#include "EldoraWriter.h"

const int num_instances_per_writer = 1;

////////////////////////////////////////////////////////////

EldoraWriter::EldoraWriter(DDS::DataWriter_ptr writer) :
	writer_(DDS::DataWriter::_duplicate(writer)), _condition(_mutex),
			finished_instances_(0), timeout_writes_(0) {

	for (int i = 0; i < 100; i++) {
		OURDDSTYPE* pItem = new OURDDSTYPE;
		_outQueue.push_back(pItem);
	}
}

////////////////////////////////////////////////////////////

void EldoraWriter::start() {
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

void EldoraWriter::end() {
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Writer::end \n")));
	wait();
}

////////////////////////////////////////////////////////////

int EldoraWriter::svc() {
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

		item_dw = ::EldoraDDS::PulseDataWriter::_narrow(writer_.in());
		if (CORBA::is_nil (pulse_dw.in ())) {
			cerr << "Data Writer could not be narrowed"<< endl;
			exit(1);
		}

		OURDDSTYPE item;
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

bool EldoraWriter::is_finished() const {
	return finished_instances_ == num_instances_per_writer;
}

////////////////////////////////////////////////////////////

int EldoraWriter::get_timeout_writes() const {
	return timeout_writes_.value();
}

////////////////////////////////////////////////////////////

int EldoraWriter::itemsAvailable() {

	guard_t guard(_mutex);

	return _outQueue.size();

}
////////////////////////////////////////////////////////////

OURDDSTYPE* EldoraWriter::getEmptyItem() {

	guard_t guard(_mutex);

	if (_outQueue.size() == 0)
		return 0;

	OURDDSTYPE* pItem = _outQueue[0];
	_outQueue.erase(_outQueue.begin());

	//std:cout << "pulse removed from outQueue " << _outQueue.size() << "\n";

	return pItem;

}
////////////////////////////////////////////////////////////

void EldoraWriter::publishItem(OURDDSTYPE* pItem) {

	guard_t guard(_mutex);

	_inQueue.push_back(pPulse);

	_condition.broadcast();
}

////////////////////////////////////////////////////////////

void EldoraWriter::waitForItem() {

	guard_t guard(_mutex);

	while (_inQueue.size() == 0)
		_condition.wait();

}

////////////////////////////////////////////////////////////

bool EldoraWriter::publish(DDS::InstanceHandle_t handle) {

	guard_t guard(_mutex);

	if (_inQueue.size() == 0)
		return false;

	OURDDSTYPE* pItem = _inQueue[0];

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

////////////////////////////////////////////////////////////
