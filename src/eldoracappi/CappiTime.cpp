#include "CappiTime.h"
///////////////////////////////////////////////////////////////////////
CappiTime::CappiTime(QWidget* parent):
QWidget(parent), _mode(REALTIME), _min(0), _max(1000), _minDelta(seconds(600))
{
	QVBoxLayout* l = new QVBoxLayout;
	parent->setLayout(l);
	QVBoxLayout* l1 = new QVBoxLayout;
	l->addLayout(l1);
	QHBoxLayout* l2 = new QHBoxLayout;
	l->addLayout(l2);
	
	_startText = new QLabel(parent);
	_startSlider = new QSlider(Qt::Horizontal, parent);
	_startSlider->setValue(_startSlider->minimum());
	_startSlider->setMinimum(_min);
	_startSlider->setMaximum(_max);
	_startSlider->setValue(_min);
	
	_stopText = new QLabel(parent);
	_stopSlider = new QSlider(Qt::Horizontal, parent);
	_stopSlider->setValue(_stopSlider->maximum());
	_stopSlider->setMinimum(_min);
	_stopSlider->setMaximum(_max);
	_stopSlider->setValue(_max);
	
	l1->addWidget(_startText);
	l1->addWidget(_startSlider);
	l1->addWidget(_stopText);
	l1->addWidget(_stopSlider);
	
	_modeText = new QLabel("Mode", parent);
	l2->addWidget(_modeText);
	
	_applyButton = new QPushButton("Apply", parent);
	l2->addWidget(_applyButton);
	
	_timeFacet = new time_facet("%m-%d-%y %H:%M");
	_timeString.imbue(std::locale(std::locale::classic(), _timeFacet));
	
	_startText->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	_startSlider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	_stopText->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	_stopSlider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	_modeText->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	_applyButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	
	connect(_startSlider, SIGNAL(sliderMoved(int)), this, SLOT(startSliderMoved(int)));
	connect(_stopSlider, SIGNAL(sliderMoved(int)), this, SLOT(stopSliderMoved(int)));
	connect(_applyButton, SIGNAL(released()), this, SLOT(applyReleased()));
	
}

///////////////////////////////////////////////////////////////////////
CappiTime::~CappiTime(){
	delete _startText;
	delete _startSlider;
	delete _stopText;
	delete _stopSlider;
	delete _timeFacet;
}

///////////////////////////////////////////////////////////////////////
void CappiTime::applyReleased() {
	
	// make sure that the start and stop times have ben set 
	if (_startTime == boost::posix_time::not_a_date_time)
		startSliderMoved(_startSlider->value());
	
	if (_stopTime == boost::posix_time::not_a_date_time)
		startSliderMoved(_stopSlider->value());
	
	emit apply(_mode, _startTime, _stopTime);
}

///////////////////////////////////////////////////////////////////////
void CappiTime::setStartTime(ptime t){
	_startTime = t;
	_startSlider->setValue(sliderPos(t));
	displayTime(_startText, t);		
}

///////////////////////////////////////////////////////////////////////
void CappiTime::setStopTime(ptime t){
	_stopTime = t;
	_stopSlider->setValue(sliderPos(t));
	displayTime(_stopText, t);		
}

///////////////////////////////////////////////////////////////////////
void CappiTime::startSliderMoved(int value){

	_startTime = sliderTime(value);
	
	// see if the start slider is pushing the stop slider forward
	if (_startTime > _stopTime - _minDelta) {
		// start slider is within the minimum distance from stop slider
		if (_startTime > _latest - _minDelta) {
			// start slider pushes stop slider past the end of the range
			// set the stop slider to maximum
			_startTime = _latest - _minDelta;
			_stopTime = _latest;
		} else {
			// start slider pushes stop slider but stays within range
			// bump up the stop slider
			_stopTime = _startTime + _minDelta;
		}
	} 
	
	_startSlider->setValue(sliderPos(_startTime));
	_stopSlider->setValue(sliderPos(_stopTime));
	
	displayTime(_startText, _startTime);
	displayTime(_stopText, _stopTime);
	
	// if stop slider is at the maximum, we are in realtime mode
	if (_stopSlider->value() == _stopSlider->maximum())
		_mode = REALTIME;
	else
		_mode = FIXED;
	
	std::string modeText = _mode == REALTIME ? "Real-time" : "Fixed";
	_modeText->setText(modeText.c_str());
}

///////////////////////////////////////////////////////////////////////
void CappiTime::stopSliderMoved(int value){
	

	_stopTime = sliderTime(value);
	// see if the stop slider is pushing the start slider backward
	if (_stopTime < _startTime + _minDelta) {
		// start slider is within the minimum distance from stop slider
		if (_stopTime < _earliest + _minDelta) {
			// stop slider pushes start slider past the end of the range
			// set the start slider to maximum
			_startTime = _earliest;
			_stopTime = _earliest + _minDelta;
		} else {
			// stop slider pushes start slider but stays within range
			// bump up the start slider
			_startTime = _startTime - seconds(600.0);
		}
	} 
	
	_startSlider->setValue(sliderPos(_startTime));
	_stopSlider->setValue(sliderPos(_stopTime));
	
	displayTime(_startText, _startTime);
	displayTime(_stopText, _stopTime);
	
}
	
///////////////////////////////////////////////////////////////////////
void CappiTime::displayTime(QLabel* w, ptime t) {
	
	_timeString.str("");
	_timeString << t;
	w->setText(_timeString.str().c_str());
	
	// if stop slider is at the maximum, we are in realtime mode
	if (_stopSlider->value() == _stopSlider->maximum())
		_mode = REALTIME;
	else
		_mode = FIXED;
	
	std::string modeText = _mode == REALTIME ? "Real-time" : "Fixed";
	_modeText->setText(modeText.c_str());

}

///////////////////////////////////////////////////////////////////////
void CappiTime::setLimits(ptime earliest, ptime latest, time_duration minDelta) {
	_earliest = earliest;
	_latest = latest;
	_minDelta = minDelta;
}

///////////////////////////////////////////////////////////////////////
ptime CappiTime::sliderTime(int pos) {
	
	time_duration range = _latest - _earliest;
	double ms = range.total_milliseconds()*(((double)(pos-_min))/(_max-_min));
	return _earliest + millisec(ms);
}

///////////////////////////////////////////////////////////////////////
int CappiTime::sliderPos(ptime time) {
	
	// sanity check
	if (_earliest == not_a_date_time ||
		_latest == not_a_date_time || 
		time > _latest ||
		time < _earliest) 
	{
		return _min + (_min+_max)/2;
	}
	
	double denom = time_duration(_latest-_earliest).total_seconds();
	double num = time_duration(time-_earliest).total_seconds();
	
	return _min+((int)((num/denom)*(_max-_min)));
}