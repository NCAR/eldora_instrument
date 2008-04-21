#ifndef ELDORAPRODUCTSMAIN_H_
#define ELDORAPRODUCTSMAIN_H_

#include <string>
#include <vector>

/// @todo This resulted from needing to create a class for EldoraProductsMain.
/// Needs to be refactored into a more standard arrangement.

class EldoraProductsMain {
    public:

        EldoraProductsMain(int argc,
                           char** argv);

        virtual ~EldoraProductsMain();

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
        void parseArgs(std::string& pulseTopic,
                       std::string& productsTopic,
                       std::string& ORB,
                       std::string& DCPS);

        /// argc from main()
        int _argc;
        /// argv from main()
        char** _argv;
        /// The number of samples that have been read by the ABP source
        int _numAbpBeams;
        /// The number of samples, for each receiver, discarded by the product generator
        std::vector<int> _numAbpDiscards;
        /// The number of beams produced by the product generator
        int _numProductBeams;

};

#endif /*ELDORAPRODUCTSMAIN_H_*/
