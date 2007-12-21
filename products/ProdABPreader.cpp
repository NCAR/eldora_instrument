#include "ProdABPreader.h"
#include <iostream>
/////////////////////////////////////////////////////////////////////

ProdABPreader::ProdABPreader(
        DDSSubscriber& subscriber,
            std::string abpTopic) :
    PulseReader(subscriber, abpTopic) {

}

/////////////////////////////////////////////////////////////////////

ProdABPreader::~ProdABPreader() {

}

/////////////////////////////////////////////////////////////////////

void
ProdABPreader::notify() {
    while (Pulse* pulse = getNextItem()) {
        std::cout << "Pulse " << pulse->timestamp << "\n";
        returnItem(pulse);
    }
}