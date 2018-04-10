#include "tempread.h"
#include <QDebug>
using namespace std;

double defInput = 0.0;

//temperature reading function
void Tempread::run() //temperature reading function adapted from: http://bradsrpi.blogspot.co.uk/2013/12/c-program-to-read-temperature-from-1.html
{
	char path[] = "/sys/bus/w1/devices/28-0317607d35ff/w1_slave";
	char buf[256];
	int fd = -1;
	char *temp;
	double value = 0.0;

	runningX = true;
	while (runningX)
	{


		fd = open(path, O_RDONLY);
		if (fd == -1)
		{
			perror("Couldn't open the w1 device.");
			exit(1);
		}
		read(fd, buf, sizeof(buf));
		temp = strchr(buf, 't');

		sscanf(temp, "t=%s", temp);

		value = atof(temp) / 1000;

		defInput = value;
		close(fd);
	}
}

void Tempread::finish()
{
	runningX = false;
	exit(0);
}

double Tempread::signalData()
{
	output = defInput;
	return output;
}
