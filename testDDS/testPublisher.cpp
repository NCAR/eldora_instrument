#include <iostream>

#include "EldoraPublisher.h"
#include "EldoraWriter.h"

using namespace EldoraDDS;

int main(int argc, char* argv[]) {

	int nWrites = 5;

	ACE_Time_Value small(0, 100);
	EldoraPublisher publisher(argc, argv);

	int pubStatus = publisher.status();

	if (pubStatus) {
		std::cout << "Unable to run the publsher, return status from run is "
				<< pubStatus << std::endl;
		return pubStatus;
	}

	EldoraWriter<Pulse, PulseTypeSupportImpl, PulseTypeSupport_var, PulseDataWriter>
			writer(publisher);

	short timestamp = 0;
	int numPulses = 0;
	int gates = 1500;

	while ( !writer.is_finished()) {

		while ((writer.itemsAvailable()) < nWrites) {
			ACE_OS::sleep(small);
		}
		for (int i = 0; i < nWrites; i++) {
			EldoraDDS::Pulse* pPulse;

			// get an available empty pulse from the publisher
			pPulse = writer.getEmptyItem();
			pPulse->abp.length(3*gates);

			// bump the timestamp on alternating pulses
			// so that forward and aft share a common timestamp
			timestamp += (numPulses % 2);

			numPulses++;

			for (int n = 0; n < 3*gates; n += 3) {
				pPulse->abp[n ] = timestamp + n;
				pPulse->abp[n+1] = timestamp + n+1;
				pPulse->abp[n+2] = timestamp + n+2;
			}

			// set the timestamp
			pPulse->timestamp = timestamp;

			// alternate the radar id between forward and aft
			pPulse->radarId = (numPulses % 2) ? EldoraDDS::Forward
					: EldoraDDS::Aft;

			// send the pulse to the publisher
			writer.publishItem(pPulse);

		}
		ACE_OS::sleep(small);
	}

	return 0;
}
