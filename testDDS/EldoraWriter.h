// -*- C++ -*-
//
// $Id: Writer.h 29 2005-07-18 18:21:16Z calabrese_p $
#ifndef WRITER_H
#define WRITER_H

#include <dds/DdsDcpsPublicationC.h>
#include <ace/Task.h>


class EldoraWriter : public ACE_Task_Base
{
public:

  EldoraWriter (::DDS::DataWriter_ptr writer, int sleepUsecs = 10000);

  void start ();

  void end ();

  /** Lanch a thread to write. **/
  virtual int svc ();

  bool is_finished () const;

  int get_timeout_writes () const;


private:

  ::DDS::DataWriter_var writer_;
  ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> finished_instances_;
  ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> timeout_writes_;
  int _sleepUsecs;
};

#endif /* WRITER_H */
