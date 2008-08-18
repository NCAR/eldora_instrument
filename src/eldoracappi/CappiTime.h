#ifndef CAPPITIME_H_
#define CAPPITIME_H_

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <sstream>
#include <locale>

#include <boost/date_time/posix_time/posix_time.hpp>
using boost::posix_time::ptime;
using boost::posix_time::time_facet;
using boost::posix_time::time_duration;
using boost::posix_time::seconds;
using boost::posix_time::time_duration;
using boost::posix_time::millisec;
using boost::posix_time::not_a_date_time;

/// A class for managing the cappi time selection widgets.
class CappiTime : public QWidget {
Q_OBJECT
public:
	enum MODE {FIXED, REALTIME};
	
	CappiTime(QWidget* parent);
	virtual ~CappiTime();
	void setLimits(ptime earliest, ptime latest, time_duration minDelta);
	void setStartTime(ptime t);
	void setStopTime(ptime t);

public slots:
	void startSliderMoved(int value);
	void stopSliderMoved(int value);
	void applyReleased();
	
	signals:
	void apply(CappiTime::MODE mode, ptime, ptime);

protected:
	/// Determine the slider position, based on the given time.
	/// It is determined from the slider limits, and the earliest 
	/// and latest times. If earliest or latest are not valid
	/// times, it is the middle of the range.
	/// @param time The time to be interpolated.
	/// @return The slider value
	int sliderPos(ptime time);
	ptime sliderTime(int pos);
	/// Display the time in a widget
	/// @param w The widget to display the time
	/// @param t The time
	void displayTime(QLabel* w, ptime t);
	QLabel* _startText;
	QLabel* _stopText;
	QSlider* _startSlider;
	QSlider* _stopSlider;
	QLabel* _modeText;
	QPushButton* _applyButton;
	
	
	time_facet* _timeFacet;
	std::ostringstream _timeString;

	/// slider minimm value
	int _min;
	/// slider maximum value
	int _max;
	
	ptime _startTime;
	ptime _stopTime;
	
	ptime _earliest;
	ptime _latest;
	time_duration _minDelta;
	
	MODE _mode;

};
#endif /*CAPPITIME_H_*/
