#include "CappiTime.h"
///////////////////////////////////////////////////////////////////////
CappiTime::CappiTime(QWidget* parent):
QWidget(parent)
{
	QVBoxLayout* l = new QVBoxLayout;
	parent->setLayout(l);
	_startText = new QLabel(parent);
	_startSlider = new QSlider(Qt::Horizontal, parent);
	_startSlider->setValue(_startSlider->minimum());
	_stopText = new QLabel(parent);
	_stopSlider = new QSlider(Qt::Horizontal, parent);
	_stopSlider->setValue(_stopSlider->maximum());
	
	l->addWidget(_startText);
	l->addWidget(_startSlider);
	l->addWidget(_stopText);
	l->addWidget(_stopSlider);
	
	_timeFacet = new time_facet("%m-%d-%y %H:%M");
	_timeString.imbue(std::locale(std::locale::classic(), _timeFacet));
	
	connect(_startSlider, SIGNAL(valueChanged(int)), this, SLOT(startSliderMoved(int)));
	connect(_stopSlider, SIGNAL(valueChanged(int)), this, SLOT(stopSliderMoved(int)));
	
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
void CappiTime::setStartTime(ptime t){
	_startTime = t;
	displayTime(_startText, _startSlider->minimum());		
}

///////////////////////////////////////////////////////////////////////
void CappiTime::setStopTime(ptime t){
	_stopTime = t;
	displayTime(_stopText, _stopSlider->maximum());		
}

///////////////////////////////////////////////////////////////////////
void CappiTime::startSliderMoved(int value){

	// what is the minimum difference between the start and stop sliders
	long delta = time_duration(_stopTime-_startTime).total_seconds();
	int minDeltaValue = (600.0/delta)*(_startSlider->maximum() - _startSlider->minimum());
	
	// see if the start slider is pushing the stop slider forward
	if (_stopSlider->value() < value+minDeltaValue) {
		// start slider is within the minimum distance from stop slider
		if (_stopSlider->maximum()<= value + minDeltaValue) {
			// start slider pushes stop slider past the end of the range
			// set the stop slider to maximum
			displayTime(_stopText, _stopSlider->maximum(), _stopSlider);
			// set the start slider to its maximum
			displayTime(_startText, _startSlider->maximum()-minDeltaValue, _startSlider);
		} else {
			// start slider pushes stop slider but stays within range
			// bump up the stop slider
			displayTime(_stopText, value+minDeltaValue, _stopSlider);
		}
	} else {
		displayTime(_startText, value);		
	}
}

///////////////////////////////////////////////////////////////////////
void CappiTime::stopSliderMoved(int value){
	
	// what is the minimum difference between the start and stop sliders
	long delta = time_duration(_stopTime-_startTime).total_seconds();
	int minDeltaValue = (600.0/delta)*(_stopSlider->maximum() - _stopSlider->minimum());
	
	// see if the stop slider is pushing the start slider backward
	if (value-minDeltaValue <= _startSlider->value()) {
		// start slider is within the minimum distance from stop slider
		if (_startSlider->minimum()>= value - minDeltaValue) {
			// stop slider pushes start slider past the end of the range
			// set the start slider to minimum
			displayTime(_startText, _startSlider->minimum(), _startSlider);
			// set the stop slider to minimum
			displayTime(_stopText, _stopSlider->minimum()+minDeltaValue, _stopSlider);
		} else {
			// stop slider pushes start slider but stays within range
			// bump down the start slider
			displayTime(_startText, value-minDeltaValue, _startSlider);
		}
	} else {
		displayTime(_stopText, value);		
	}
}
	
///////////////////////////////////////////////////////////////////////
void CappiTime::displayTime(QLabel* w, int sliderValue, QSlider* slider) {
	
	double fraction = ((double)sliderValue)/(_stopSlider->maximum() - _startSlider->minimum());
	
	long delta = time_duration(_stopTime-_startTime).total_seconds();
	delta *= fraction;
	ptime newtime = _startTime + seconds(delta);
	
	_timeString.str("");
	_timeString << newtime;
	w->setText(_timeString.str().c_str());
	
	if (slider)
		slider->setValue(sliderValue);
}
