#include "DDSWriter.h"

using namespace DDS;
using namespace CORBA;

////////////////////////////////////////////////////////////

template<WRITERSIG1>
DDSWriter<WRITERSIG2>::DDSWriter(DDSPublisher& ddsPublisher,
        std::string topicName, int queueSize) :
finished_instances_(0), timeout_writes_(0), _condition(_mutex), _topicName(topicName), _queueSize(queueSize), _terminate(false)
{

    // reserve the space in the queues
    for (int i = 0; i < _queueSize; i++) {
        DDSTYPE* pItem = new DDSTYPE;
        _outQueue.push_back(pItem);
    }

    // get the DDS publisher, which we will need to create the writer.
    Publisher_var& publisher = ddsPublisher.getPublisher();

    // get the domain participant, which we register our type with
    DomainParticipant_var& participant = ddsPublisher.getParticipant();
    
    try {
        // register our type
        DDSTYPESUPPORT_VAR typeSupport = new DDSTYPESUPPORTIMPL();
        if (RETCODE_OK != typeSupport->register_type(participant.in (), "")) {
            cerr << "register_type failed." << endl;
            exit(1);
        }

        // get the type name
        String_var type_name = typeSupport->get_type_name ();
        if (0 ==type_name.in()){
            cerr << "get_type_name failed for topic " << topicName.c_str() << std::endl;
            exit(1);
        }

        // Activate the listener so we can get notification of assorted
        // state changes, e.g., publication matches and deleted connections.
        OpenDDS::DCPS::DataWriterListener_var listener(this);
        if (CORBA::is_nil (listener.in())) {
            cerr << "DataWriterListener is nil for " << _topicName << endl;
            exit(1);
        }

        // get the default quality of service
        TopicQos topic_qos;
        participant->get_default_topic_qos(topic_qos);
        // specify reliable reliability. What a concept.
        topic_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;

        // create our topic, using our type name and the default qos. 
        // We will not be using a listener.
        std::cout << "Creating topic " << topicName.c_str() << 
            " for type name " << type_name << std::endl;
        Topic_var topic = participant->create_topic (topicName.c_str(),
                type_name.in (),
                topic_qos,
                DDS::TopicListener::_nil());
        if (is_nil (topic.in ())) {
            cerr << "create_topic failed for topic " << 
                topicName.c_str() << " for type name " << type_name << std::endl;
            exit(1);
        }

        // Create the generic datawriter for our topic
        DataWriterQos dw_qos;
        publisher->get_default_datawriter_qos (dw_qos);
        _genericWriter = publisher->create_datawriter(topic.in (),
                dw_qos, listener.in());
        if (is_nil (_genericWriter.in ())) {
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

    // start the writer thread!
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Writer::start \n")));
    if (activate(THR_NEW_LWP | THR_JOINABLE, 1) == -1) {
        cerr << "Writer::start(): activate failed" << endl;
        exit(1);
    }
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
DDSWriter<WRITERSIG2>::~DDSWriter(){
    std::cout << __FUNCTION__ << " destructor\n";
}

////////////////////////////////////////////////////////////
template<WRITERSIG1>
void
DDSWriter<WRITERSIG2>::end() {
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Writer::end \n")));
    wait();
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
int
DDSWriter<WRITERSIG2>::svc() {
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Writer::svc begins.\n")));

    InstanceHandleSeq handles;
    try {

        while (1)
        {
            if (_terminate) {
                cerr << __FUNCTION__ << " terminating" << endl;
                return 0;
            }
                
            _genericWriter->get_matched_subscriptions(handles);
            if (handles.length() > 0)
            break;
            else
            ACE_OS::sleep(ACE_Time_Value(0,200000));
        }

        _specificWriter = DDSDATAWRITER::_narrow(_genericWriter.in());
        if (is_nil (_specificWriter.in ())) {
            cerr << "Data Writer could not be narrowed"<< endl;
            exit(1);
        }

        DDSTYPE item;
        InstanceHandle_t handle = _specificWriter->_cxx_register (item);

        ACE_DEBUG((LM_DEBUG,
                        ACE_TEXT("%T (%P|%t) Writer::svc starting to write.\n")));

        // this is where it all happens
        while(1) {

            if (_terminate)
            break;
            // block until item(s) appears on the _inQueue.
            waitForItem();

            // publish the next items on the _inQueue.
            // publish() returns non-zero if more items remain on _inQueue.
            while(publish(handle)) {};
        }

    } catch (Exception& e) {
        cerr << "Exception caught in svc:" << endl
        << e << endl;
    }

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Writer::svc finished.\n")));

    finished_instances_ ++;

    return 0;
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
bool
DDSWriter<WRITERSIG2>::is_finished() const {
    return finished_instances_ == 1;
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
int
DDSWriter<WRITERSIG2>::get_timeout_writes() const {
    return timeout_writes_.value();
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
int
DDSWriter<WRITERSIG2>::itemsAvailable() {

    guard_t guard(_mutex);

    return _outQueue.size();

}
////////////////////////////////////////////////////////////

template<WRITERSIG1>
DDSTYPE*
DDSWriter<WRITERSIG2>::getEmptyItem() {

    guard_t guard(_mutex);

    if (_outQueue.size() == 0)
    return 0;

    DDSTYPE* pItem = _outQueue[0];
    _outQueue.erase(_outQueue.begin());

    return pItem;

}
////////////////////////////////////////////////////////////

template<WRITERSIG1>
void
DDSWriter<WRITERSIG2>::publishItem(
        DDSTYPE* pItem) {

    guard_t guard(_mutex);

    _inQueue.push_back(pItem);

    _condition.broadcast();
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
void
DDSWriter<WRITERSIG2>::waitForItem() {

    guard_t guard(_mutex);

    while (_inQueue.size() == 0)
    _condition.wait();

}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
void 
DDSWriter<WRITERSIG2>::on_offered_deadline_missed(DDS::DataWriter_ptr writer, 
        const DDS::OfferedDeadlineMissedStatus&)
    ACE_THROW_SPEC (( ::CORBA::SystemException )) {
    std::cout << "DDSWriter: offered deadline missed for topic " << 
        _topicName << std::endl;
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
void 
DDSWriter<WRITERSIG2>::on_offered_incompatible_qos(DDS::DataWriter_ptr writer,
        const DDS::OfferedIncompatibleQosStatus& status)
    ACE_THROW_SPEC (( ::CORBA::SystemException ))
{
    std::cout << "DDSWRiter: incompatible QoS was offered for topic " << 
        _topicName << std::endl;
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
void 
DDSWriter<WRITERSIG2>::on_liveliness_lost(DDS::DataWriter_ptr writer,
        const DDS::LivelinessLostStatus& status)
    ACE_THROW_SPEC (( ::CORBA::SystemException )) {
    std::cout << "DDSWriter: liveliness lost for topic " << _topicName << 
        std::endl;
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
void 
DDSWriter<WRITERSIG2>::on_publication_match(DDS::DataWriter_ptr writer,
        const DDS::PublicationMatchStatus& status)
    ACE_THROW_SPEC (( ::CORBA::SystemException )) {

    std::cout << "DDSWriter: Got new subscriber for topic " << _topicName <<
        " (id " << status.last_subscription_handle << ")" << std::endl;
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
void 
DDSWriter<WRITERSIG2>::on_publication_disconnected(DDS::DataWriter_ptr writer,
        const ::OpenDDS::DCPS::PublicationDisconnectedStatus& status)
    ACE_THROW_SPEC (( ::CORBA::SystemException )) 
{
    DDS::InstanceHandleSeq subscribers(status.subscription_handles);
    std::cout << "DDSWriter: subscriber disconnected from topic " << 
        _topicName << " (id ";
    if (subscribers.length() == 1)
        std::cout << subscribers[0];
    else
        std::cout << "[multiple ids]";
    std::cout << ")" << std::endl;
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
void 
DDSWriter<WRITERSIG2>::on_publication_reconnected(DDS::DataWriter_ptr writer,
        const ::OpenDDS::DCPS::PublicationReconnectedStatus& status)
    ACE_THROW_SPEC (( ::CORBA::SystemException )) 
{
    DDS::InstanceHandleSeq subscribers(status.subscription_handles);
    std::cout << "DDSWriter: subscriber reconnected to topic " << 
        _topicName << " (id ";
    if (subscribers.length() == 1)
        std::cout << subscribers[0];
    else
        std::cout << "[multiple ids]";
    std::cout << ")" << std::endl;
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
void 
DDSWriter<WRITERSIG2>::on_publication_lost(DDS::DataWriter_ptr writer,
        const ::OpenDDS::DCPS::PublicationLostStatus& status)
    ACE_THROW_SPEC (( ::CORBA::SystemException )) 
{
    DDS::InstanceHandleSeq subscribers(status.subscription_handles);
    std::cout << "DDSWriter: subscriber lost for topic " << _topicName << 
        " (id ";
    if (subscribers.length() == 1)
        std::cout << subscribers[0];
    else
        std::cout << "[multiple ids]";
    std::cout << ")" << std::endl;
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
void 
DDSWriter<WRITERSIG2>::on_connection_deleted(DDS::DataWriter_ptr writer)
    ACE_THROW_SPEC (( ::CORBA::SystemException )) 
{
    std::cout << "DDSWriter: a connection was deleted for topic " << 
        _topicName << std::endl;
}

////////////////////////////////////////////////////////////

template<WRITERSIG1>
bool
DDSWriter<WRITERSIG2>::publish(
        InstanceHandle_t handle) {

    guard_t guard(_mutex);

    if (_inQueue.size() == 0)
    return false;

    DDSTYPE* pItem = _inQueue[0];

    _inQueue.erase(_inQueue.begin());

    DDS::ReturnCode_t ret;

    ret = _specificWriter->write(*pItem, handle);

    if (ret != DDS::RETCODE_OK) {
        ACE_ERROR ((LM_ERROR,
                        ACE_TEXT("(%P|%t)ERROR  EldoraWriter::publish, ")
                        ACE_TEXT ("write() returned %d.\n"),
                        ret));
        if (ret == DDS::RETCODE_TIMEOUT) {
            timeout_writes_ ++;
        }
    }

    // put the item back on the avalable queue
    _outQueue.push_back(pItem);

    return (_inQueue.size() != 0);
}

/// @todo These instantiations really belong somewhere else

using namespace EldoraDDS;
template class DDSWriter<Ray, RayTypeSupportImpl,
RayTypeSupport_var, RayDataWriter,
RayDataWriter_var>;

template class DDSWriter<TimeSeries, TimeSeriesTypeSupportImpl,
TimeSeriesTypeSupport_var, TimeSeriesDataWriter,
TimeSeriesDataWriter_var>;

template class DDSWriter<Products, ProductsTypeSupportImpl,
ProductsTypeSupport_var, ProductsDataWriter,
ProductsDataWriter_var>;
