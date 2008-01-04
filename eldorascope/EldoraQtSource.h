#ifndef ELDORASCOPESOURCE_H_
#define ELDORASCOPESOURCE_H_

#include <vector>
#include <QObject>
#include <QTimer>
#include <QThread>

// $Copyright$

/// A basic data source for an Eldora Qt applications. This class 
//  acts as a data provider, implementing the basic Qt mechanisms
/// that the qt application will use to communicate its data requirements.
/// For instance, when the user hits a button on the display
/// to select the forward or aft radar, the scope can send a signal 
/// to the appropriate EldoraQtSource slot in order to tell the
/// source which data to send.
///
/// In usage, a specific data source type will dervie from EldoraQtSource,
/// to provide the desired data feed. For instance, the derived class
/// might combine a DDS data reader with EldoraQtSource in order
/// to provide DDS data reception with Qt applications.
///
/// EldoraQtSource is derived from QThread. The run() method
/// starts the event loop.  Call quit() to terminate the event loop,
/// and wait() to wait for the QThread to finish.
///
/// The data source for a scope is supposed to provide the data
/// throttling; i.e. it sends data to the scope at the correct rate.
/// The scope does not care what that rate is, except that if it is too
/// fast, the system will be consumed by the display. Thus, EldoraQtSource
/// provides a rate control feature. It s very simple. A timer signal,
/// occurring at the desired scope update rate, sets the _capture variable
/// to true. The derived EldoraQtSource classes can use the _capture
/// flag to determine when to collect incoming data and send it on
/// to the connected scope. When _capture is false, the incoming
/// data is ignored.
///
/// The scope can request data either along a beam for a single
/// pulse, or along a gate in time. If along a beam, then values
/// for all gates are provided. If along a gate, in time, then 
/// the scope specifies how many samples in the timesereis will be captured.
/// Slots alongBeamSlot() and oneGateSlot() are used to select
/// beam versus one gate mode respectively.
///
/// Some mechanisms for monitoring data throughput are also provided in this 
/// class.
class EldoraQtSource : public QThread {
    Q_OBJECT

    public:
        /// The display can either show all gates along a beam, or
        /// values in time for a selected gate.
        enum GATE_MODE {
            ALONG_BEAM, ///< Display all gates along a beam
            ONE_GATE ///< Display values in time for a selected gate
        };

        EldoraQtSource(double outputRate=20.0);
        
        virtual ~EldoraQtSource();
       /// Return the number of bytes received. The counter
       /// is reset to zero whenever numBytes() is called.
        unsigned long numBytes();

        /// Start the thread
        void run();
        
    public slots:
        /// Called when the rate timer times out.
        void rateTimeoutSlot();

        /// Notify that we are shutting down. Derived classes
        /// should override this if they need to handle a shutdown.
        virtual void shutdown();

    protected:
        /// The number of samples received so far.
        unsigned int _readSamples;

        /// The number of data bytes received.
        unsigned long _numBytes;

        /// Timer to use for managing output rate
        QTimer _rateTimer;

        /// timer interval, ms
        int _intervalMS;

        /// The current mode of data delivery.
        GATE_MODE _gateMode;

        /// The number of points when in ONE_GATE mode
        unsigned int _pointsPerGate;

        /// The next data point to be filled in the I and Q vectors.
        unsigned int _pointCounter;

        /// The gate of choice in ONE_GATE mode.
        unsigned int _gate;

        /// Set true by the rate timer timeout, which
        /// enables capturing the next block of data
        int _capture;

        /// the rate at which to deliver data blocks to the client
        double _outputRate;

        /// True if we are looking at the forward radar, false otherwise
        bool _forwardRadar;
        
        /// Set true after the start() function has been called.
        bool _run;
        
};

#endif 
