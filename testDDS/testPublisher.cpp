#include "EldoraPublisher.h"

int
main(int argc, char* argv[]) {

  ACE_Time_Value small(0,2000);
  EldoraPublisher publisher;

  int pubStatus = publisher.run(argc, argv);

  if (pubStatus)
    return pubStatus;

  while ( !publisher.isFinished()) {
    ACE_OS::sleep (small);
    publisher.newData(0);
  }

  return 0;
}
