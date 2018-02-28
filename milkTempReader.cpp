
/*
 The C++ file containing the innards to the tempReader class. 
*/  

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "milkTempReader.h"
#include "CppTimer.h" // include this to call upon the timerEvent() function for different events?? 

using namespace std;


void tempReader::setPath() {
  
  DIR *dir;
  struct dirent *dirent;
  dir = opendir (path);
  
  if (dir != NULL)
  {
    while ((dirent = readdir (dir)))
      // 1-wire devices are links beginning with 28-
      if (dirent->d_type == DT_LNK && strstr(dirent->d_name, "28-0317606edbff") != NULL) 
      { 
        strcpy(dev, dirent->d_name);
        printf("\nDevice: %s\n", dev);
      }
      (void) closedir (dir);
  }
  
  else 
  {
    perror ("Couldn't open the w1 devices directory");
    return 1;
  }
  // Assemble path to OneWire device
  sprintf(devPath, "%s/%s/w1_slave", path, dev);
}


/* getTemp() function to read data from w1 device (ie temp sensor) */
float tempReader::getTemp() {
  
  // Read temp continuously.
  // Opening the device's file triggers new reading.
  while(1) {

        int fd = open(devPath, O_RDONLY);

    if(fd == -1) {
      perror ("Couldn't open the w1 device.");
      return 1;   
    }

    while((numRead = read(fd, buf, 256)) > 0) {
      strncpy(tmpData, strstr(buf, "t=") + 2, 5); 
      float tempC = strtof(tmpData, NULL);
      printf("Device: %s  - ", dev); 
      printf("Temp: %.3f C  ", tempC / 1000);
      float tempF = (tempC / 1000) * 9 / 5 + 32;
      printf("%.3f F\n\n", tempF);
      
      /* should I put the interval coding for CppTimer in here?? */
     
    }

    close(fd);

  } 
  /* return 0; --never called due to loop */
}

/* should I create a class for the timerEvent() stuff?
 * eg
 * 
 * For timing the first 30mins after fridge removal 
 *
 * class Msg1Timer : CppTimer{
 * 
 *       void timerEvent(){
 *            if ( tempC > fridgeTemp ) {
 *               Msg1Timer msg1Timer;
 *               msg1Timer.start();
 *               // how do you then store that timer value into a variable to use later??
 *            }
 *       }
 * 
 * };
 * 
 * class Msg2Timer : CppTimer{
 * 
 *       void timerEvent(){
 *            if ( tempC = roomTemp ) {
 *               Msg2Timer msg2Timer;
 *               msg2Timer.start();
 *            }
 *       }
 * 
 * };
 */
 


int main (void) {
  
    // or should I put the interval coding in here???
    
  
    char dev[16];      // Dev ID
    char devPath[128]; // Path to device
    char buf[256];     // Data from device
    char tmpData[6];   // Temp C * 1000 reported by device 
    char path[] = "/sys/bus/w1/devices"; 
    float tempC;
    float tempF;
    ssize_t numRead;
  
    
}
