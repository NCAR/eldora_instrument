#include "EldoraWriter.h"

const int num_instances_per_writer = 1;

////////////////////////////////////////////////////////////

EldoraWriter::EldoraWriter(DDS::DataWriter_ptr writer) :
  writer_ (DDS::DataWriter::_duplicate (writer)),
  _condition(_mutex),
  finished_instances_ (0),
  timeout_writes_ (0)
{

  _gates = 1500;

  for (int i = 0; i < 100; i++) {
    pulse_t* pPulse = new pulse_t;
    pPulse->abp.length(3*_gates);
    _outQueue.push_back(pPulse);
  }
}

////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////

void
EldoraWriter::end ()
{
  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT("(%P|%t) Writer::end \n")));
  wait ();
}


////////////////////////////////////////////////////////////

int
EldoraWriter::svc ()
{
  ACE_DEBUG((LM_DEBUG,
	     ACE_TEXT("(%P|%t) Writer::svc begins.\n")));

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

    pulse_dw = ::EldoraDDS::PulseDataWriter::_narrow(writer_.in());
    if (CORBA::is_nil (pulse_dw.in ())) {
      cerr << "Data Writer could not be narrowed"<< endl;
      exit(1);
    }

    pulse_t pulse;
    DDS::InstanceHandle_t handle = pulse_dw->_cxx_register (pulse);

    ACE_DEBUG((LM_DEBUG,
	       ACE_TEXT("%T (%P|%t) Writer::svc starting to write.\n")));
    int i = 0;

    while(1) {
      waitForPulse();
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

bool
EldoraWriter::is_finished () const
{
  return finished_instances_ == num_instances_per_writer;
}

////////////////////////////////////////////////////////////

int
EldoraWriter::get_timeout_writes () const
{
  return timeout_writes_.value ();
}

////////////////////////////////////////////////////////////

int
EldoraWriter::pulsesAvailable() {

  guard_t guard(_mutex);

  return _outQueue.size();

}
////////////////////////////////////////////////////////////

pulse_t*
EldoraWriter::getEmptyPulse() {

  guard_t guard(_mutex);

  if (_outQueue.size() == 0 )
    return 0;

  EldoraDDS::Pulse*  pPulse = _outQueue[0];
  _outQueue.erase(_outQueue.begin());

  //std:cout << "pulse removed from outQueue " << _outQueue.size() << "\n";

  return pPulse;

}
////////////////////////////////////////////////////////////

void
EldoraWriter::publishPulse(pulse_t* pPulse) {

  guard_t guard(_mutex);

  _inQueue.push_back(pPulse);

  std::cout << __FILE__ << "pulse added to inQueue " << _inQueue.size() << "\n";

  _condition.broadcast();
}

////////////////////////////////////////////////////////////

void
EldoraWriter::waitForPulse() {

  guard_t guard(_mutex);

  while(_inQueue.size() == 0)
    _condition.wait();

}

////////////////////////////////////////////////////////////

bool
EldoraWriter::publish(DDS::InstanceHandle_t handle) {

  guard_t guard(_mutex);

  if (_inQueue.size() == 0)
    return false;

  pulse_t* pPulse = _inQueue[0];

  _inQueue.erase(_inQueue.begin());
  
  DDS::ReturnCode_t ret;

  std::cout << __FILE__ << " writing pulse\n";

  ret = pulse_dw->write(*pPulse, handle);
    
  std::cout << __FILE__ << " returned from writing pulse\n";

  if (ret != DDS::RETCODE_OK) {
    ACE_ERROR ((LM_ERROR,
		ACE_TEXT("(%P|%t)ERROR  EldoraWriter::svc, ")
		ACE_TEXT ("write() returned %d.\n"),
		ret));
    if (ret == DDS::RETCODE_TIMEOUT) {
      timeout_writes_ ++;
    }
  }

  _outQueue.push_back(pPulse);

  //std::cout << "pulse returned to outQueue\n";

  return( _inQueue.size() != 0);
}

////////////////////////////////////////////////////////////
