#ifndef SIGNALCATCHER_H_
#define SIGNALCATCHER_H_

#include <vector>

/// Use this singleton class to arrange signal handling. A 
/// signal handler is registered to be called when signals are caught.
/// A flag variable is provided to the class constructor. When a signal
/// occurs, the flag variable is set true. The only exception
/// is when the signal is SEGV, in which case abort() is called 
/// to terminate the process and generate a core dump. Note that
/// typically you need to enable core file generation using
/// 'ulimit -c unlimited'
//
class SignalCatcher {
    public:
        /// @return The instance of this class. 
        static SignalCatcher* instance();
        /// Add a variable which will be set to the
        /// the signal type when a signal is detected.
        /// Multiple variables can be added through succesive
        /// calls to addsignalFlag.
        /// @param signalFlag The variable which will be 
        /// set to the signal type when detected. It will
        /// be initialized to zero by this call.
        void addSignalFlag(int* signalFlag);
        /// Set the signal flags.Thiiiiiiiiiiiis   should not be called
        /// the useeer; it is public so that it can be accessed by 
        /// the signal catching function.
        /// @param signal The value to set the flag to.
        void setFlags(int signal);
    protected:
        SignalCatcher();
    private:
        std::vector<int*> _signalFlags;
};
#endif /*SIGNALCATCHER_H_*/
