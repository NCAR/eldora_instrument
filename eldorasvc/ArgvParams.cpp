#include "ArgvParams.h"

/////////////////////////////////////////////////////////////////////

ArgvParams::ArgvParams(std::string appName):
_argv0(appName)
{
}

/////////////////////////////////////////////////////////////////////

ArgvParams::~ArgvParams() {
	// erase the current list
	for (int i = 0; i < _argv.size(); i++) {
		delete [] _argv[i];
	}
	_argv.clear();
}

/////////////////////////////////////////////////////////////////////

int ArgvParams::argc() {
	int n = 1;
	std::map<std::string, std::string>::iterator it;
	for (it = this->begin(); it != this->end(); it++) {
		if (!it->second.compare("")) {
			// if argument value is empty
			n += 1;
		} else {
			// is argument value is present
			n += 2;
		}
	}
	return n;
}

/////////////////////////////////////////////////////////////////////

char** 
ArgvParams::argv() {

	// erase the current list
	for (int i = 0; i < _argv.size(); i++) {
		delete [] _argv[i];
	}
	_argv.clear();

	// build a new one
	
	// put in argv0 so that it always appears at the front
	_argv.push_back(new char[_argv0.size()+1]);
	for (int i = 0; i < _argv0.size(); i++)
		_argv[0][i] = _argv0[i];
	_argv[0][_argv0.size()] = 0;
	
	// add the other args. They will be ordered by the order of the map.
	
	std::map<std::string, std::string>::iterator it;
	for (it = this->begin(); it != this->end(); it++) {
		// save the argument name
		char* pC = new char[it->first.size()+1];
		for (int i = 0; i < it->first.size(); i++)
			pC[i] = it->first[i];
		pC[it->first.size()] = 0;
		_argv.push_back(pC);
		if (it->second.compare("")) {
			// save the argument value, if specified
			char* pC = new char[it->second.size()+1];
			for (int i = 0; i < it->second.size(); i++)
			pC[i] = it->second[i];
			pC[it->second.size()] = 0;
			_argv.push_back(pC);
		}
	}

	return &_argv[0];
}
