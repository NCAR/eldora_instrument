#ifndef ARGVPARAMSINC_
#define ARGVPARAMSINC_

#include <map>
#include <string>

/// A class to provide argv parameter functionality. Use this when
/// you wish to prgramatically construct argv, or you can't 
/// simply use the original argv.
///
/// The arguments are saved as a map of strings, indexed by the
/// argument name (as a string). You must provide the leading
/// dash or dashes to the argument as required by the downstream
/// application.
///
/// If an argument does not include a parameter, the data 
/// element of the map will be an empty string.
///
/// @todo ArgvParams should be refactored to allow a given
/// argument to be specified multiple times, which is 
/// legitimate for some applicatins. Current usage of std::map
/// precludes this.
class ArgvParams: public std::map<std::string, std::string>
 {
public:
	/// @param appName The application name which will be used
	/// as the first parameter.
	ArgvParams(std::string appName);
	virtual ~ArgvParams();
	/// @return The number of parameters.
	int argc();
	/// @return The vector of arguments.
	char** argv();
	
protected:
	/// The vector of arguments to be passed to applications needing argv.
	std::vector<char*> _argv;
};
#endif
