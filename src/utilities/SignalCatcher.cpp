#include "SignalCatcher.h"
#include <cstdlib>
#include <csignal>
#include <iostream>


SignalCatcher* _pinstance = 0;
//////////////////////////////////////////////////////////////////////

/// The function that is called to set the signal flag.
void
signalHandlerFunction(int signo) {
    std::cout << "signalHandlerFunction(" << signo << ") called" << std::endl;
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
SignalCatcher::configure(int* signalFlag, pf userFunction, void* userData)
{
    if (signalFlag) {
        *signalFlag = 0;
        _signalFlags.push_back(signalFlag);
    }
    
    if (userFunction) {
        _userFunctions.push_back(userFunction);
        _userData.push_back(userData);
    }
}

//////////////////////////////////////////////////////////////////////
void
SignalCatcher::setFlags(int signo) {
    // set the signal flags
    for (unsigned int i = 0; i < _signalFlags.size(); i++)
        *(_signalFlags[i]) = signo;
    
    // call the user callback functions
    for (unsigned int i = 0; i < _userFunctions.size(); i++) {
        pf f = _userFunctions[i];
        (*f)(signo, _userData[i]);
    }
}

//////////////////////////////////////////////////////////////////////
