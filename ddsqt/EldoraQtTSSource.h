#ifndef ELDORAQTTSSOURCE_H_
#define ELDORAQTTSSOURCE_H_

#include <vector>
#include "EldoraQtSource.h"
#include "DDSReader.h"
#include "TimeSeriesTypeSupportC.h"
#include "TimeSeriesTypeSupportImpl.h"
using namespace EldoraDDS;

/// A timeseries DDS subscriber for Eldora data,
/// which can feed Qt clients..
/// It is derived from EldoraQtSource and TSReader.
/// When the DDS data notification is received via the notify()
/// method, a Qt signal is emitted in order to deliver the
/// data to other Qt components. Note that this scheme is utilizing 
/// Qt4's capabilty to send signals between threads. See the 
/// Qt4 docuentation on threading and QObjects.
///
/// The EldoraQtSource rate limiting mechanism (the _capture variable)
/// is used to control the rate of data delivery to the qt client.
class EldoraQtTSSource : public EldoraQtSource, public TSReader {
Q_OBJECT
    public:
        EldoraQtTSSource(
                DDSSubscriber& subscriber,
                    std::string topicName,
                    double outputRate=20.0);
        virtual ~EldoraQtTSSource();
        /// Subclass DDSReader::notify(), which wil be called
        /// whenever new samples are added to the DDSReader available
        /// queue. Process the samples here.
        virtual void notify();

    signals:
        /// This signal is emitted when new IQ data is available.
        /// I and Q will be of the same length, and will have at least 1 
        /// value in them.
        /// @param I I data values.
        /// @param Q q data values.
        /// @param sampleRateHz The sample rate of the timeseries, in Hz.
        /// @param tuningFreqHz The frequency that the receiver is tuned to, in Hz.
        void newData(
                std::vector<double> I,
                    std::vector<double> Q,
                    double sampleRateHz,
                    double tuningFreqHz);
        
         signals:
        /// This signal is emitted to announce the list of possible 
        /// time series gate choices.
       void tsGateList(
                std::vector<int> gates, unsigned short firstgate);

    public slots:
        /// Set the gate mode to ONE_GATE.
        /// @param channel The channel number (1-4)
        /// @param forwardRadar True if forward radar, false if aft
        /// @param gate The gate to send in
        /// @param n The number of points
        virtual void oneGateSlot(
                int channel,
                    bool forwardRadar,
                    int gate,
                    int n);

        /// Set the gate mode to along beam.
        /// @param channel The channel number (1-4)
        /// @param forwardRadar True if forward radar, false if aft
        virtual void alongBeamSlot(
                int channel,
                    bool forwardRadar);

    protected:
        /// The selected radar, either forward or aft
        EldoraDDS::RadarChoice _radarId;

        /// The selected channel to send to the client
        unsigned int _channel;

        /// The collection of available gates in the timeseries
        std::vector<int> _gates;

        /// Buffer for saving one gate of I data over successive pulses
        std::vector<double> I;

        /// Buffer for saving one gate of Q data over successive pulses
        std::vector<double> Q;

};

#endif 
