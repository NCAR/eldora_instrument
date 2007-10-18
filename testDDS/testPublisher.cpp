#include <iostream>

#include "EldoraPublisher.h"
#include "EldoraWriter.h"

using namespace EldoraDDS;

int main(int argc, char* argv[]) {

	int nWrites = 5;

	ACE_Time_Value small(0, 100);
	DDSPublisher publisher(argc, argv);

	int pubStatus = publisher.status();

	if (pubStatus) {
		std::cout << "Unable to run the publsher, return status from run is "
				<< pubStatus << std::endl;
		return pubStatus;
	}

	DDSWriter<Pulse, PulseTypeSupportImpl, PulseTypeSupport_var, PulseDataWriter, PulseDataWriter_var>
			pulseWriter(publisher, "testtopic");

	short timestamp = 0;
	int numPulses = 0;
	int gates = 1500;

	while ( !pulseWriter.is_finished()) {

		while ((pulseWriter.itemsAvailable()) < nWrites) {
			ACE_OS::sleep(small);
		}
		for (int i = 0; i < nWrites; i++) {
			EldoraDDS::Pulse* pPulse;

			// get an available empty pulse from the publisher
			pPulse = pulseWriter.getEmptyItem();
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
			pulseWriter.publishItem(pPulse);

		}
		ACE_OS::sleep(small);
	}

	return 0;
}
