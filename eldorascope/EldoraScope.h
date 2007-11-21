/** @page eldorascope-overview The EldoraScope Program

 EldoraScope provides a traditional real-time Ascope display of 
 eldora time series data and computed products. Data are delivered
 to EldoraScope via newTimeSeriesSlot() and newProductSlot().

 The display uses the QtToolbox ScopePlot component. The time series
 data are displayed by either the timerseries, i versus q, or power
 spectrum mode of the ScopePlot. The computed products are displayed
 by the product mode of ScopePlot.
 
 Data may be displayed either for all gates in a beam,
 or a timeseries of data for a selected gate.

 EldoraScope is configured via EldoraScope.ini.
 **/

#ifndef ELDORASCOPE_H
#define ELDORASCOPE_H

#include <QDialog>
#include <QPalette>
#include <qevent.h>
#include <deque>
#include <set>
#include <map>
// the fastest fft in the west; used for power spectrum calcs.
#include <fftw3.h>

// Coponents from the QtToolbox
#include <ScopePlot.h>
#include <Knob.h>
#include <QtConfig.h>
#include "ui_EldoraScope.h"

// PlotInfo knows the characteristics of a plot
#include "PlotInfo.h"

/// types of plots available in the scope plot.
enum SCOPE_PLOT_TYPES {
    SCOPE_PLOT_TIMESERIES,
    SCOPE_PLOT_IVSQ,
    SCOPE_PLOT_SPECTRUM,
    SCOPE_PLOT_PRODUCT
};

/// Time series plot types.
enum TS_PLOT_TYPES {
    TS_TIMESERIES_PLOT, ///<  time series I and Q plot
    TS_IVSQ_PLOT, ///<  time series I versus Q plot
    TS_SPECTRUM_PLOT ///<  time series power spectrum plot 
};

/// Product plot types.
enum PRODUCT_PLOT_TYPES {
    PROD_S_DBMHC, ///< S-band dBm horizontal co-planar
    PROD_S_DBMVC, ///< S-band dBm vertical co-planar
    PROD_S_DBZ, ///< S-band dBz
    PROD_S_SNR, ///< S-band SNR
    PROD_S_VEL, ///< S-band velocity
    PROD_S_WIDTH, ///< S-band spectral width
    PROD_S_RHOHV, ///< S-band rhohv
    PROD_S_PHIDP, ///< S-band phidp
    PROD_S_ZDR, ///< S-band zdr
    PROD_X_DBMHC, ///< X-band dBm horizontal co-planar
    PROD_X_DBMVX, ///< X-band dBm vertical cross-planar
    PROD_X_DBZ , ///< X-band dBz
    PROD_X_SNR, ///< X-band SNR
    PROD_X_VEL, ///< X-band velocity
    PROD_X_WIDTH, ///< X-band spectral width
    PROD_X_LDR ///< X-band LDR
};

/// Provides a real time display of either pulse data or a product
/// extracted from beams. Only a fraction of the incoming data stream is
/// displayed, since the human eye could not discern the whole bandwidth,
/// and would use up the cpu anyway.
class EldoraScope : public QDialog, public Ui::EldoraScope {
    Q_OBJECT
    /// The display can either show all gates along a beam, or
    /// values in time for a selected gate.
    enum GATE_MODE {
        ALONG_BEAM, ///< Display all gates along a beam
        ONE_GATE    ///< Display values in time for a selected gate
    };
    
    public:
        EldoraScope(
                QDialog* parent = 0);
        ~EldoraScope();

        signals:
        
        /// Emitted to announce that data should be delivered in ONE_GATE mode.
        /// @param channel The selected channel
        /// @param gate The selected gate
        /// @param n The number of points to deliver for the selected gate
        void oneGateSignal(int channel, int gate, int n);
        /// emmited to indicate that data should be delivered for 
        /// all gates along a beam
        /// @param channel The selected channel
        void alongBeamSignal(int channel);
    public slots:
        /// Call when new timeseries data is available.
        void timeSeriesSlot(
                std::vector<double> I,
                    std::vector<double> Q,
                    double sampleRateHz,
                    double tuningFreqHz);
        /// Call when data is available on the product data socket.
        void productSlot();
        /// Call when the plot type is changed. This function 
        /// must determine which of the two families of
        /// plots, _tsPlotInfo, or _prodPlotInfo, the
        /// previous and new plot types belong to.
        virtual void plotTypeSlot(
                int plotType);
        /// call to save the current plotting parameters for the
        /// current plot type, and reload the parameters for the 
        /// the new plot type. It handles both pulse and beam
        /// displays. pulsePlot is used to differentiate between the
        /// two.
        void plotTypeChange(
                PlotInfo* pi,
                    TS_PLOT_TYPES plotType,
                    PRODUCT_PLOT_TYPES prodType,
                    bool pulsePlot);
        /// A different tab has been selected. Change the plot type to the
        /// currently selected button on that tab.
        void tabChangeSlot(
                QWidget* w);
        /// The gain knob value has changed.
        virtual void gainChangeSlot(
                double);
        /// slide the plot up.
        virtual void upSlot();
        /// Slide the plot down.
        virtual void dnSlot();
        /// Initiate an autoscale. A flag is set; during the next 
        /// pulse reception an autoscale computation is made.
        virtual void autoScaleSlot();
        /// Save the scope display to a PNG file.
        void saveImageSlot();
        /// Pause the plotting. Any received data are ignored.
        /// @param p True to enable pause.
        void pauseSlot(bool p);
        /// Set the gate display mode
        /// @param m The gate mode, either ALONG_BEAM or ONE_GATE
        void gateModeSlot(int m);

    protected:
        /// Send the data for the current plot type to the ScopePlot.
        void displayData();
        /// Initialize the pulse and product sockets. The
        /// notifiers will be created, and connected
        /// to the data handling slots.
        void initSockets();
        ///	cumulative error count
        int _errorCount[3];
        ///  last pulse number
        long long _lastPulseNum[3];
        double _knobGain;
        double _knobOffset;
        double _xyGraphRange;
        double _xyGraphCenter;
        double _specGraphRange;
        double _specGraphCenter;
        double _xFullScale;
        /// Set true to cause an autoscale 
        /// to take place on the next data series
        bool _performAutoScale;
        /// set the _graphRange and _graphOffset based
        /// on the single data series.
        /// @param data The data series to be analyzed.
        void autoScale(
                std::vector<double>& data);
        /// set the _graphRange and _graphOffset based
        /// on the two data series.
        /// @param data1 The first data series to be analyzed.
        /// @param data2 The second data series to be analyzed.
        void autoScale(
                std::vector<double>& data1,
                    std::vector<double>& data2);
        /// set the _specGraphRange and _specGraphCenter based
        /// on the single data series.
        /// @param data The spectral series to be analyzed.
        void specAutoScale(
                std::vector<double>& data);
        /// Adjust the _graphRange and _graphOffset values.
        /// @param min Desired scale minimum
        /// @param max Desired scale maximum
        void adjustGainOffset(
                double min,
                    double max);
        /// Holds I data from a pulse for time series and I vs. Q 	
        std::vector<double> I;
        /// Holds Q data from a pulse for time series and I vs. Q display
        std::vector<double> Q;
        /// Used to collect the spectrum values calculated from pulses
        std::vector<double> _spectrum;
        /// Used to collect product data from beams
        std::vector<double> _ProductData;
        // how often to update the statistics (in seconds)
        int _statsUpdateInterval;
        /// Set true if time series plots are selected, false for product type plots
        bool __timeSeriesPlot;
        /// The current selected plot type.
        TS_PLOT_TYPES _tsPlotType;
        /// The current selected product type.
        PRODUCT_PLOT_TYPES _productPlotType;
        // The builtin timer will be used to calculate beam statistics.
        void timerEvent(
                QTimerEvent*);
        /// The hamming window coefficients
        std::vector<double> _hammingCoefs;
        ///	The fftw plan. This is a handle used by
        ///	the fftw routines.
        fftw_plan _fftwPlan;
        ///	The fftw data array. The fft will
        //	be performed in place, so both input data 
        ///	and results are stored here.
        fftw_complex* _fftwData;
        ///	fixed block size for initial cut
        /// @todo implement selectable or automatic
        /// fft size specification. Perhaps can choose the
        /// largest power of two less than the delivered data size?
        /// Would required pre- or dynamic creation of multiple
        /// fftw plans.
        unsigned int _fftBlockSize;
        //	power correction factor applied to (uncorrected) powerSpectrum() output
        double _powerCorrection;
        /// Set true if the Hamming window should be applied
        bool _doHamming;
        double _az;
        /// Process time series data.
        /// @param Idata The I values
        /// @param Qdata The Q values
        void processTimeSeries(
                std::vector<double>& Idata,
                    std::vector<double>& Qdata);
        /// Process product data
        void processProduct();
        /// Counter of time series, used for decimating 
        /// the timeseries (and I/Q and Spectrum)
        /// display updates.
        int _tsDisplayCount;
        /// Counter of product cacluations, used
        /// for decimating the product display updates.
        int _productDisplayCount;
        /// Compute the power spectrum. The input values will come
        /// I[]and Q[], the power spectrum will be written to 
        /// _spectrum[]
        /// @return The zero moment
        double powerSpectrum();
        /// For each TS_PLOT_TYPES, there will be an entry in this map.
        std::map<TS_PLOT_TYPES, PlotInfo> _tsPlotInfo;
        /// For each PRODUCT_PLOT_TYPES, there will be an entry in this map.
        std::map<PRODUCT_PLOT_TYPES, PlotInfo> _prodPlotInfo;
        /// This set contains PLOTTYPEs for all timeseries plots
        std::set<TS_PLOT_TYPES> _timeSeriesPlots;
        /// This set contains PLOTTYPEs for all raw data plots
        std::set<TS_PLOT_TYPES> _pulsePlots;
        /// This set contains PLOTTYPEs for all S band moments plots
        std::set<PRODUCT_PLOT_TYPES> _sMomentsPlots;
        /// This set contains PLOTTYPEs for all X band moments plots
        std::set<PRODUCT_PLOT_TYPES> _xMomentsPlots;
        /// save the button group for each tab,
        /// so that we can find the selected button
        /// and change the plot type when tabs are switched.
        std::vector<QButtonGroup*> _tabButtonGroups;
        /// initialize all of the book keeping structures
        /// for the various plots.
        void initPlots();
        /// add a rw plot tab to the plot type selection tab widget.
        /// Radio buttons are created for all of specified
        /// plty types, and grouped into one button group.
        /// _tsPlotInfo provides the label information for
        /// the radio buttons.
        /// @param tabName The title for the tab.
        /// @param types A set of the desired TS_PLOT_TYPES types 
        /// @return The button group that the inserted buttons
        /// belong to.
        QButtonGroup* addProductTypeTab(
                std::string tabName,
                    std::set<PRODUCT_PLOT_TYPES> types);
        /// add a products tab to the plot type selection tab widget.
        /// Radio buttons are created for all of specified
        /// plty types, and grouped into one button group.
        /// _tsPlotInfo provides the label information for
        /// the radio buttons.
        /// @param tabName The title for the tab.
        /// @param types A set of the desired TS_PLOT_TYPES types 
        /// @return The button group that the inserted buttons
        /// belong to.
        QButtonGroup* addPlotTypeTab(
                std::string tabName,
                    std::set<TS_PLOT_TYPES> types);
        /// The configuration for EldoraScope
        QtConfig _config;
        /// Palette for making the leds green
        QPalette _greenPalette;
        /// Platette for making the leds red
        QPalette _redPalette;
        /// Set true if the plot graphics are paused
        bool _paused;
        /// The selected gate mode
        GATE_MODE _gateMode;
};

#endif
