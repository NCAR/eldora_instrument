#ifndef ELDORAWRITER_H
#define ELDORAWRITER_H

#include <ace/OS_NS_unistd.h>
#include <ace/streams.h>
#include <unistd.h>
#include "PulseTypeSupportC.h"
#include <dds/DdsDcpsPublicationC.h>
#include <ace/Task.h>
#include <ace/Synch.h>
#include <vector>

#include "EldoraPublisher.h"

typedef ACE_Thread_Mutex mutex_t;
typedef ACE_Condition_Thread_Mutex condition_t;
typedef ACE_Guard<mutex_t> guard_t;

#define TEMPSIG1 class DDSTYPE, class DDSTYPESUPPORTIMPL, class DDSTYPESUPPORT_VAR, class DDSDATAWRITER
#define TEMPSIG2 DDSTYPE, DDSTYPESUPPORTIMPL, DDSTYPESUPPORT_VAR, DDSDATAWRITER

template <TEMPSIG1>
class EldoraWriter : public ACE_Task_Base {

public:

	
	EldoraWriter(EldoraPublisher& eldoraPub);

	void start();

	void end();

	virtual int svc();

	bool is_finished() const;

	int get_timeout_writes() const;

	void publishItem(DDSTYPE* pItem);

	int itemsAvailable();

	DDSTYPE* getEmptyItem();

private:
	
	void waitForItem();

	bool publish(::DDS::InstanceHandle_t handle);

	DDS::DataWriter_var _basicDw;

	EldoraDDS::PulseDataWriter_var item_dw;

	ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> finished_instances_;

	ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> timeout_writes_;

	mutex_t _mutex;

	condition_t _condition;

	std::vector<DDSTYPE*> _inQueue;

	std::vector<DDSTYPE*> _outQueue;

};

#endif
