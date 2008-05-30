//ddsmenu.h
//DDS Menu Class
//Written by Drew and Charlie

#ifndef DDSMENU_H
#define DDSMENU_H
#include <string>
#include "dds.h"
/**
	@author Drew
*/
class DDSMenu
{
public:
  /**
   * Constructor
   * @param dds[] array of dds
   * @param numdds number of dds
   */
  DDSMenu(std::string ipAddress, DDS* dds[], int numdds);

  std::string _ipAddress;
  
  DDS** _dds;

  ///Number of DDS
  int _numdds;

  ///Destructor
  ~DDSMenu();


protected:
  ///Setup of DDS Menu
  void OpenMenu();

  ///Create new DDS text file
  ///@return 1 if create is successful, 0 if failed
  int InitFile();

  ///Write values to DDS Text
  ///@return 1 if write is successful, 0 if failed
  int AppendFile();

  ///Load DDS text File
  ///@return 1 if load successful, 0 if failed
  int LoadFile();

  ///Grabs Line of Input Values and Stores them
  ///@return
  short mygetline();

  ///Counter/Index for Menu
  int _idx;

  ///NRflag array; only for numdds=5;
  int _NRflag[6];

  ///Frequency array; only for numdds=5;
  double _freq[6];
  
  ///Input line array
  char _inbuf[14];

};

#endif
