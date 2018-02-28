/* The header file for the milkTempReader.cpp script. 
 Contains the milkTempReader class, and lists the 
 functions used in this class, as well as the variables
 used.
*/

#ifndef MILKTEMPREADER
#define MILKTEMPREADER
#include <QThread>

class tempReader : public QThread {
  
    public:
      
      // call the two functions from milkTempReader.cpp
      void  setPath();
      float getTemp();
      // need to design a function to getTime(). Maybe use the CppTimer code from Bernd?
      // this will let me send off times to prowl.c to determine specific messages.
    
      
    private:
      
      // bring in the degC and degF data from the .cpp file. Use these to send to window.cpp
      float tempC;
      float tempF;
      // also need to include the times recorded from CppTimer.h for the prowl messages 
      
      
}

#endif