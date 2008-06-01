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
                  float radarConstant,
                  float xBandGain,
                  float pNoise,
                  float freqs[4],
                  float rxGain[4],
                  float lossIn[4],
                  float prt,
                  float prtShort,
                  float prtLong,
                  bool dualPrt);
        /// Single constant
        typedef double TermData;
        /// Data accross all gates or all channels
        typedef std::vector<double> TermData1D;
        /// Data across all gates and all channels
        typedef std::vector<TermData1D> TermData2D;
        
        //TermData   xBandGain;
        TermData   radarConstant;
        TermData1D lambda_k;
        TermData   lambda;
        TermData1D a_k;
        TermData1D a10_k;
        TermData1D b_k;
        TermData1D b10_k;
        TermData1D r;        ///< 20.0*log10(range), by gate (km) @todo is this at the center of the range gate, or an edge?
        TermData1D Vscale_k;
        TermData   Vscale;
        TermData   VscaleShort;
        TermData   VscaleLong;
        TermData   Wscale;
        TermData   WscaleShort;
        TermData   WscaleLong;
        TermData2D Praw_k;       ///< Raw power at receiver, by channel.
        TermData2D Pant_k;       ///< Total power at antenna, by channel
        TermData1D Pant;         ///< Total power at antenna
        TermData2D Psig_k;       ///< Signal power at receiver, by channel
        TermData1D Psig;         ///< Signal power at receiver
        TermData1D Psigant;      ///< Signal power at antenna
        //TermData2D V_k;          ///< Velocity, by channel
        TermData2D Psigs_k;      ///< Signal power at receiver, short pulse, by channel
        TermData2D Psigl_k;      ///< Signal power at receiver, long pulse, by channel
        TermData1D Psigs;        ///< Signal power at receiver, short pulse
        TermData1D Psigl;        ///< Signal power at receiver, long pulse
        TermData1D Thetas;       ///< Accumulated phase change of short prts.
        TermData1D Thetal;       ///< Accumulated phase change of long prt
        TermData1D DeltaTheta;   ///< Phase difference between short and long prts.
        TermData1D Dbz;          ///< Reflectivity
        TermData1D W;            ///< Spectral width
        TermData1D Ncp;          ///< Normalized coherent power
        TermData1D Vr;           ///< Radial velocity
        TermData1D Vs;           ///< Short pulse velocity
        TermData1D Vl;           ///< Long pulse velocity

};

/// Caclulate radar products and publish them. The formulation of the
/// products computation are defined in "Eldora Moments" by Eric Loew.
/// Nomenclature and methodology from that text are followed closely here.
///
/// A number of calculation factors are precomputed once, when the first set
/// of rays is received.
class EldoraProducts
{
    /// Describe a two dimensional array of pointers to rays. The first dmension
    /// selects the prt id. The second id selects the channel number.
    typedef std::vector<std::vector<EldoraDDS::Ray*> > RayData;
    public:
        /// @param publisher The publisher for publishing products.
        /// @param productsTopic The topic that the products wll be published under.
        EldoraProducts(DDSPublisher& publisher,
                       std::string productsTopic,
                       bool dualPrt);
        virtual ~EldoraProducts();

        /// Called with a new set of rays for one radar for one ray number.
        /// All prt ids are included. Thus all of the rays are provided 
        /// that are needed to compute a product along all gates.
        ///
        /// RayData is a two dmensional array. The first index chooses the prtId.
        /// If in single mode, this index can only be zero. In dual prt
        /// mode, it can be either 0 or 1. The second index covers the 
        /// frequency channels 0-3.
        ///
        /// @param ray Two dimnsional vector containing pointers to rays.
        void newRayData(RayData& ray);

        /// Return the number of rays procesed thus far.
        int numRays();

    protected:
        /// Initialize the fixed fields in a Products item,
        /// and resize the vectors.
        /// @param p the products item to be initialized.
        /// @param gates The nmber of gates
        void initProducts(EldoraDDS::Products* p, int gates);
        /// Initalize the entries in the Productsterms structure.
        /// Precomputed factors are initialized and vectors are resized.
        /// The ray data provides the header information.
        /// @rays Ray data with housekeeping.
        void initTerms(RayData& rays);
        /// Calculate raw power at receiver
        void powerRaw(RayData& rays);
        /// Calculate power at antenna
        void powerAntenna(RayData& rays);
        /// Calculate total power referenced to antenna
        void totalPower(RayData& rays);
        /// Calculate signal power, by channel and at antenna
        void signalPower(RayData& rays);
        /// Calculate total signal power
        void totalSignalPower(RayData& rays);
        /// Calculate reflectivity
        void reflectivity(RayData& rays);
        /// Calculate velocity
        void velocity(RayData& rays);
        
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
