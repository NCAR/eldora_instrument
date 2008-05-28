#ifndef ELDORAPRODUCTS_INC
#define ELDORAPRODUCTS_INC
#include <string>
#include "RayTypeSupportC.h"
#include "ProductsTypeSupportC.h"
#include "DDSPublisher.h"
#include "DDSWriter.h"

/// A structure to hold intermediate terms and final values 
/// for the moments calculations. A vector is used which can
/// be resized for the number of gates being processed. For
/// some variables, the vector of gates is contained within a vector
/// of channels. These variables have _k suffixed to the member name.
/// Indexing into the fields observes the following conventions:
/// <br> Praw_k[channel][gate]
/// <br> Praw[gate]
/// <br> Some of the precomputed factors are indexed only by channel, such as:
/// <br> lambda_k[channel]
struct ProductsTerms
{
        void init(int gates,
                  double xBandGain,
                  double freqs[4],
                  double rxGain[4],
                  double lossIn[4],
                  double prt,
                  double prtShort,
                  double prtLong);
        /// Data accross all gates or all channels
        typedef std::vector<double> TermData1D;
        /// Data across all gates and all channels
        typedef std::vector<TermData1D> TermData2D;
        TermData2D Praw_k; ///< Raw power at receiver, by channel.
        TermData2D Pant_k; ///< Total power at antenna, by channel
        TermData1D Pant; ///< Total power at antenna
        TermData2D Psig_k; ///< Signal power at receiver, by channel
        TermData1D Psig; ///< Signal power at receiver
        TermData2D V_k; ///< Velocity, by channel
        TermData2D Psigs_k; ///< Signal power at receiver, short pulse, by channel
        TermData2D Psigl_k; ///< Signal power at receiver, long pulse, by channel
        TermData1D Psigs; ///< Signal power at receiver, short pulse
        TermData1D Psigl; ///< Signal power at receiver, long pulse
        TermData1D Thetas; ///< Accumulated phase change of short prts.
        TermData1D Thetal; ///< Accumulated phase change of long prt
        TermData1D DeltaTheta; ///< Phase difference between short and long prts.
        TermData1D Dbz; ///< Reflectivity
        TermData1D W; ///< Spectral width
        TermData1D Ncp; ///< Normalized coherent power
        TermData1D Vs; ///< Short pulse velocity
        TermData1D Vr; ///< Long pulse velocity

};

/// A number of calculation factors are precomputed once, when the first ray is received.
/// If operating in dual prt mode, we will need a short prt ray and a long prt ray 
/// in order to calculate these constants.
class EldoraProducts
{
        typedef std::vector<std::vector<EldoraDDS::Ray*> > RayData;
    public:
        EldoraProducts(DDSPublisher& publisher,
                       std::string productsTopic,
                       bool dualPrt);
        virtual ~EldoraProducts();

        void newRayData(RayData& ray);

        int numRays();

    protected:
        /// Initialize the fixed fields in a Products item.
        /// @param p the products item to be initialized.
        void initProducts(EldoraDDS::Products* p);
        /// Initalize the entries in the Productsterms structure.
        /// Precomputed factors are initialized and vectors are resized.
        /// The ray data provides the header information.
        /// @rays Ray data with housekeeping.
        void initTerms(RayData& rays);
        /// Calculate raw power at receiver
        void rawPower(RayData& rays);
        /// Calculate power at antenna
        void antennaPower(RayData& rays);
        /// The number of rays that have been received.
        int _rays;
        /// The DDS publisher
        DDSPublisher& _publisher;
        /// The topic name that the Products will be published under.
        std::string _productsTopic;
        /// The DDS Products writer.
        ProductsWriter _productsWriter;
        /// The number of rays which were not processed due to unavailable
        /// empty Products.
        int _droppedRays;
        /// True if we are in dual prt mode
        bool _dualPrt;
        /// Is filled in with calculation results as each 
        /// step in the products computations are completed.
        ProductsTerms _terms;
        /// Contains the number of gates in the current set of rays
        int _gates;
        /// set to false if _terms still needs to be initialized.
        /// Used by initterms().
        bool _termsInitialized;
};
#endif
