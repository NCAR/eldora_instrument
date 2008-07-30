#include "EldoraPlaybackMain.h"
#include "QtConfig.h"

// simply a wrapper for EldoraPlaybackMain, so that the latter can be
// instantiated as a class

int main(int argc, char** argv) {
    
    // create the products generator
    EldoraPlaybackMain playback(argc, argv);
    
    // start the products playback
    playback.run();
    
    return 0;
    
}
