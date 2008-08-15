#ifndef CAPPITIME_H_
#define CAPPITIME_H_

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <sstream>
#include <locale>

#include <boost/date_time/posix_time/posix_time.hpp>
using boost::posix_time::ptime;
using boost::posix_time::time_facet;
using boost::posix_time::time_duration;
using boost::posix_time::seconds;

/// A class for managing the cappi time selection widgets.
class CappiTime : public QWidget {
Q_OBJECT
public:
	CappiTime(QWidget* parent);
	virtual ~CappiTime();
	void setStartTime(ptime t);
	void setStopTime(ptime t);

public slots:
	void startSliderMoved(int value);
	void stopSliderMoved(int value);

protected:
	void displayTime(QLabel* w, int sliderValue, QSlider* slider = 0);
	QLabel* _startText;
	QLabel* _stopText;
	QSlider* _startSlider;
	QSlider* _stopSlider;
	time_facet* _timeFacet;
	std::ostringstream _timeString;

	ptime _startTime;
	ptime _stopTime;

};
#endif /*CAPPITIME_H_*/
