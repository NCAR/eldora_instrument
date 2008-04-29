#ifndef SIGNALCATCHER_H_
#define SIGNALCATCHER_H_

#include <vector>

/// Use this singleton class to arrange signal handling. A 
/// signal handler is registered to be called when signals are caught.
/// A flag variable and a callback function can be specified in 
/// configure(). When a signal occurs, the flag variable is set true
/// and the user function is called. The only exception
/// is when the signal is SEGV, in which case abort() is called 
/// to terminate the process and generate a core dump. Note that
/// typically you need to enable core file generation using
/// 'ulimit -c unlimited'
///
/// The configure() capability should be used, otherwise 
/// using the class is pointless.
class SignalCatcher {
    public:
        /// A type that defines a pointer to a function taking an int and a 
        /// void* as arguments.
        typedef void (*pf)(int signo, void* userData);
        /// @return The instance of this class. 
        static SignalCatcher* instance();
        /// Configure signal catching behavior. User flags
        /// and callback functions can be specified.
        /// Add a variable which will be set to the
        /// the signal type when a signal is detected.
        /// Multiple variables can be added through succesive
        /// calls to addsignalFlag.
        /// @param signalFlag The variable which will be 
        /// set to the signal type when detected. It will
        /// be initialized to zero by this call. If zero, no signal
        /// flag finctionality is provided.
        /// @param userFunction A user function to be called when 
        /// a signal is caught. The signal and the user data will 
        /// be passed to the function. Multiple calls to configure() can specify
        /// multiple functions. Zero specifies that no function is called.     
        void configure(int* signalFlag = 0, pf userFunction = 0, void* userData = 0);
        /// Set the signal flags.This function should not be called
        /// the user; it is public so that it can be accessed by 
        /// the signal catching function.
        /// @param signal The value to set the flag to.
        void setFlags(int signal);
    protected:
        SignalCatcher();
    private:
        /// The user variables which will be set to the signal number.
        std::vector<int*> _signalFlags;
        /// The user callback funtions to be called.
        std::vector<pf> _userFunctions;
        /// The user data to be passed to the callback functions.
        std::vector<void*> _userData;
};
#endif /*SIGNALCATCHER_H_*/
