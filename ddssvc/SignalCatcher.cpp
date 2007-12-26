#include "SignalCatcher.h"
#include <signal.h>


SignalCatcher* _pinstance = 0;
//////////////////////////////////////////////////////////////////////

void
signalHandlerFunction(int signo) {
    // if it was a segv, produce a core dump
    if (signo == SIGSEGV)
        abort();

    _pinstance->setFlags(signo);
}
//////////////////////////////////////////////////////////////////////

SignalCatcher::SignalCatcher() {

    // install signal handler for abort signals
    struct sigaction new_action, old_action;

    /* Set up the structure to specify the new action. */
    new_action.sa_handler = signalHandlerFunction;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction(SIGINT, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGINT, &new_action, NULL);
    sigaction(SIGHUP, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGHUP, &new_action, NULL);
    sigaction(SIGTERM, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGTERM, &new_action, NULL);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGSEGV, &new_action, NULL);

    
}

//////////////////////////////////////////////////////////////////////
SignalCatcher*
SignalCatcher::instance() {
    if (_pinstance == 0) {
        _pinstance = new SignalCatcher;
    }
    
    return _pinstance;
}

//////////////////////////////////////////////////////////////////////
void
SignalCatcher::addSignalFlag(int* signalFlag) {
    *signalFlag = 0;
    _signalFlags.push_back(signalFlag);
}

//////////////////////////////////////////////////////////////////////
void
SignalCatcher::setFlags(int signo) {
    for (unsigned int i = 0; i < _signalFlags.size(); i++)
        *(_signalFlags[i]) = signo;
}

//////////////////////////////////////////////////////////////////////
