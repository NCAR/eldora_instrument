#ifndef ELDORA_PLAYBACK_H
#define ELDORA_PLAYBACK_H

#include <string>
#include <vector>

/// @todo This resulted from needing to create a class for EldoraPlaybackMain.
/// Needs to be refactored into a more standard arrangement.

class EldoraPlaybackMain {
    public:

        EldoraPlaybackMain(int argc,
                           char** argv);

        virtual ~EldoraPlaybackMain();

        /// Run the products calculations.
        /// @return Non-zero if there was an error
        int run();
        
        /// Get the current statistics
        /// @param numAbpBeams     Return the number of incoming ABP beams (both for and aft)
        /// @param numProductBeams Return the number of completed product beams (both for and aft)
        /// @param discardBeamsFor Return the number of discarded for ABP beams
        /// @param discardBeamsAft Return the number of discarded aft ABP beams.
        void status(int& numAbpBeams, int& numProductBeams, int &discardBeamsFor, int &discardBeamsAft);

    protected:
        void parseArgs( std::string& inputFileName,
                        std::string& inputDirectory,
                        int& dwellUs,
                       std::string& productsTopic,
                       std::string& ORB,
                       std::string& DCPS,
                       std::string& DCPSInfoRepo,
                       bool& dualPrt,
		       int & DCPSDebugLevel,
		       int & DCPSTransportDebugLevel);

        /// argc from main()
        int _argc;
        /// argv from main()
        char** _argv;
        /// The number of beams produced by the product generator
        int _numProductRays;

};


#endif // ELDORA_PLAYBACK_H
