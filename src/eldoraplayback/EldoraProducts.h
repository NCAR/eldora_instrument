#ifndef ELDORAPRODUCTS_INC
#define ELDORAPRODUCTS_INC
#include <string>
// #include "RayTypeSupportC.h"
#include "ProductsTypeSupportC.h"
#include "DDSPublisher.h"
#include "DDSWriter.h"

#include "DoradeHeader.h"
#include "DoradeASIB.h"
#include "DoradeVOLD.h"
#include "DoradeRYIB.h"
#include "DoradeFRAD.h"
#include "DoradeRADD.h"
#include "DoradeRADD.h"
#include "DoradeFRIB.h"
#include "DoradeSITU.h"
#include "DoradePARM.h"
#include "DoradeRAWD.h"
#include "DoradeCSPD.h"
#include "DoradeWAVE.h"

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
                  float lnaLoss,
                  float freqs[4],
                  float rxGain[4],
                  float prt,
                  float prtShort,
                  float prtLong,
                  float firstGate,
                  float cellWidth,
                  bool dualPrt);
        /// Single constant
        typedef double TermData;
        /// Data accross all gates or all channels
        typedef std::vector<double> TermData1D;
        /// Data across all gates and all channels
        typedef std::vector<TermData1D> TermData2D;

        TermData radarConstant;
        TermData1D lambda_k;
        TermData lambda;
        TermData1D a_k;
        TermData1D a10_k;
        TermData1D b_k;
        TermData1D b10_k;
        TermData1D r; ///< 20.0*log10(range), by gate (km) @todo is this at the center of the range gate, or an edge?
        TermData1D Vscale_k;
        TermData Vscale;
        TermData VscaleShort;
        TermData VscaleLong;
        TermData Wscale;
        TermData WscaleShort;
        TermData WscaleLong;
        TermData1D phaseShort; ///< Short prt phase
        TermData1D phaseLong;  ///< Long prt phase
        TermData2D SumA; ///< Sum of the A's, by prt
        TermData2D SumB; ///< Sum of the B's, by prt
        TermData2D SumP; ///< Sum of the P's, by prt
        TermData2D Praw_k; ///< Raw power at receiver, by channel.
        TermData2D Pant_k; ///< Total power at antenna, by channel
        TermData1D Pant; ///< Total power at antenna
        TermData2D Psig_k; ///< Signal power at receiver, by channel
        TermData1D Psig; ///< Signal power at receiver
        TermData1D Psigant; ///< Signal power at antenna
        //TermData2D V_k;          ///< Velocity, by channel
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
        TermData1D Vr; ///< Radial velocity
        TermData1D Vs; ///< Short pulse velocity
        TermData1D Vl; ///< Long pulse velocity

};

/// Scale and bias values to be used for the data compression 
/// in the DDS stream.
struct ProductsScaling
{
        double dmScale;  ///< scale for dm
        double dmBias;   ///< Bias for dm
        
        double pScale;   ///< scale for p1-p4
        double pBias;    ///< scale for p1-p4
        
        double dbzScale; ///< scale for dbz
        double dbzBias;  ///< bias for dbz   

        double swScale;  ///< scale for sw
        double swBias;   ///< bias for sw     

        double ncpScale; ///< scale for ncp
        double ncpBias;  ///< bias for ncp     

        double vsScale;  ///< scale for vs
        double vsBias;   ///< bias for vs

        double vlScale;  ///< scale for vl
        double vlBias;   ///< bias for vl

        double vrScale;  ///< scale for vr
        double vrBias;   ///< bias for vr
};

/// Caclulate radar products from ABP rays, and publish the result. The formulation of the
/// products computation are defined in "Eldora Moments" by Eric Loew.
/// Nomenclature and methodology from that text are followed closely here.
///
/// EldoraProducts is delivered a new collection of rays via newRayData. The collection contains 
/// all rays for a single ray number, for all channels and all prt ids. If operating
/// in single prt mode, there will be a single set of rays for that prt. If operating
/// in dual prt mode, there will be two sets of rays. The first set will be for the short 
/// prt. The second set will be for the long prt.
///
/// There are a number of constant factors which do not change for a given radar configuration.
/// These are computed once, when the first set of rays is received. This initialization is performed
/// in initTerms(). The radar configuration is read from the housekeeping data that is included 
/// with the ray data.
///
/// The computation procedure follws the linear sequence described in the Loew document.
/// The ProductsTerms structure is used to collect results, both intermediate and final,
/// as the computations proceed. ProductsTerms also holds the precomputed constants described
/// earlier.
///
/// Once all of the computation steps have been completed, the results are transfered 
/// from the ProductsTerms structure to a DDS Products object. Products is then published
/// via a DDS publisher.
///
/// In keeping with the algorithm documentation in Loew, each computation step
/// will operate either in single or dual (staggered) prt mode.
///
/// EldoraProducts is finished with the supplied ray data as soon as newRayData()
/// returns.
///
/// The computation sequence (lifted from newRayData(), is as follows. Note that
/// the routine names match the sections in Loew:
/// @code
/// powerRaw(rays);
/// powerAntenna(rays);
/// totalPower(rays);
/// signalPower(rays);
/// totalSignalPower(rays);
/// reflectivity(rays);
/// velocity(rays);
/// spectrumWidth(rays);
/// ncp(rays);
/// @endcode
class EldoraProducts
{
        /// Describe a two dimensional array of pointers to rays. The first dmension
        /// selects the prt id. The second id selects the channel number.
        typedef std::vector<std::vector<EldoraDDS::Ray*> > RayData;
    public:
        /// @param publisher The publisher for publishing products.
        /// @param productsTopic The topic that the products wll be published under.
        /// @param dualPrt Set true if we are operating in dual prt mode
        /// @param reverseVelocity Set true if the velocity needs to be reverse. The
        /// phase relationship of A and B from a signal processor is arbitrary,
        /// and this flag is used to make the sign of the velocity have the desired
        /// relationship with the doppler frequency measurement. For instance, 
        /// the standard radar convention is that positive velocity indicates 
        /// motion towards the receiver.
        EldoraProducts(DDSPublisher& publisher,
                       std::string productsTopic,
                       bool reverseVelocity, int usPerBeam=1000);
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
        /// @param ray Two dimnsional vector containing pointers to rays. If in 
        /// staggered prt mode, the first vector, for prtId == 0,
        /// will hold the short prt ray data and the second vector will hold the long 
        /// prt ray data.
        void playback(std::string inputFileName);

        /// Return the number of rays procesed thus far.
        int numRays();

    protected:
        /// Initialize the fixed fields in a Products item,
        /// and resize the vectors.
        /// @param p the products item to be initialized.
        /// @param gates The nmber of gates
        void initProducts(EldoraDDS::Products* p, EldoraDDS::Housekeeping& hskp,
                          int gates);

        void parseDoradeHeader(std::string filename, EldoraDDS::Housekeeping& hskp);
        
        void parseVOLD(DoradeVOLD *vold, EldoraDDS::Housekeeping& hskp);
        void parseRYIB(DoradeRYIB &rhib, EldoraDDS::Housekeeping& hskp);
        void parseASIB(DoradeASIB &asib, EldoraDDS::Housekeeping& hskp);
        void parseFRAD(DoradeFRAD &frad, EldoraDDS::Housekeeping& hskp);
        void parseRADD(DoradeRADD *radd, EldoraDDS::Housekeeping& hskp);
        //        void parseSITU(DoradeSITU *situ, EldoraDDS::Housekeeping& hskp);
        void parseFRIB(DoradeFRIB *frib, EldoraDDS::Housekeeping& hskp);
        void parseWAVE(DoradeWAVE *wave, EldoraDDS::Housekeeping& hskp);
        void parseCSPD(DoradeCSPD *cspd, EldoraDDS::Housekeeping& hskp);
        void parsePARMs(DoradeHeader &doradeHeader, EldoraDDS::Housekeeping& hskp);
        
        

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
        /// number of published rays
        int _publishedRays;
        /// Is filled in with calculation results as each 
        /// step in the products computations are completed.
        ProductsTerms _terms;
        /// Is initialized with scaling factors for the DDS data compression
        ProductsScaling _scaling;
        /// Contains the number of gates in the current set of rays
        int _gates;
        /// set to false if _terms still needs to be initialized.
        /// Used by initterms().
        bool _termsInitialized;
        /// Use this multiplier to reverse velocity if so specified.
        int _velSign;
        /// starting time for volume
        boost::posix_time::ptime _volumeStart;

        /// map parameter name to Product
        std::map<std::string, EldoraDDS::Product *> _prodPtrMap;

        /// maps parameter order to parameter name 
        std::vector<std::string> _prodNames;

        // output array
        Products* _products;
        ACE_Time_Value _beamDelay;
        ACE_Event _event;
        bool _verbose;
};
#endif
