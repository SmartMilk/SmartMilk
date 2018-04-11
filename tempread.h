//header file for tempread cpp thread class 

#ifndef TEMPREAD_H
#define TEMPREAD_H

#include <QThread>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

class Tempread : public QThread //since this is a QApplication, we use QThreads
{
public:

	Tempread() {runningX = 0;}; //default constructor, which is empty otherwise 
	void run();
	void finish();
	double signalData();

private:

	double output;
	bool runningX;
};

#endif
