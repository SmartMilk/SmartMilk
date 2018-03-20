#include "window.h"
#include <QtGui>
#include <dirent.h>
#include <string.h>
#include <QFont>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <thread>
#include <iostream> //testing purposes only

//#include <wiringPi.h>
//#define BUFSIZE 256  //experiment with different buffer sizes
//#define PIN RPI_GPIO_P1_12

//temperature reading function
double tempreadbuster(double *a)
{

 DIR *dir;
 struct dirent *dirent;
 char dev[16];      // Dev ID
 char devPath[128]; // Path to device
 char buf[256];     // Data from device
 char tmpData[6];   // Temp C * 1000 reported by device
 char path[] = "/sys/bus/w1/devices";
 ssize_t numRead;

 dir = opendir (path);
 if (dir != NULL)
 {
  while ((dirent = readdir (dir)))
   // 1-wire devices are links beginning with 28-
   if (dirent->d_type == DT_LNK &&
     strstr(dirent->d_name, "28-0317604cafff") != NULL) {
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
 // Read temp continuously
 // Opening the device's file triggers new reading
 while(1) {
  int fd = open(devPath, O_RDONLY);
  if(fd == -1)
  {
   perror ("Couldn't open the w1 device.");
   return 1;
  }
  while((numRead = read(fd, buf, 256)) > 0)
  {
   strncpy(tmpData, strstr(buf, "t=") + 2, 5);
   double tempC = strtof(tmpData, NULL);
  // printf("Device: %s  - ", dev);
  // printf("Temp: %.3f C  ", tempC / 1000);
  // printf("%.3f F\n\n", (tempC / 1000) * 9 / 5 + 32);
  *a = tempC/1000;
  double result = *a;
  return result;
  }
  close(fd);
 }
} //tempreadbuster function ends here

Window::Window() : Tf(15.0), Tr(16.0)
// Function calls upon the window header and continues to define elements
{
	//These functions creates all the GUI elements except the main Layout

	createTempScale();
	createCountdownBox();
	createTempCountdownVertSplit();

	//Set up the initial plot data
	for (int index = 0; index<plotDataSize; ++index)
	{
		xData[index] = index;
		yData[index] = 0;
		y1Data[index] = Tf; // fridge temp threshold
		y2Data[index] = Tr; // room temp threshold
	}

	curve = new QwtPlotCurve;
        curve->setPen(QPen(Qt::green,2));
	curve1 = new QwtPlotCurve;
        curve1->setPen(QPen(Qt::blue,2));
	curve2 = new QwtPlotCurve;
	curve2->setPen(QPen(Qt::red,2));
	plot = new QwtPlot;

	//Make a plot curve from the data and attach it to the plot
	curve->setSamples(xData, yData, plotDataSize);
	curve->attach(plot);
	curve1->setSamples(xData, y1Data, plotDataSize);
        curve1->attach(plot);
	curve2->setSamples(xData, y2Data, plotDataSize);
        curve2->attach(plot);


	plot->replot();
	plot->show();
	plot->setStyleSheet("QWidget {border-image: url(./pics/milkhasgonebad.png) }");

	plot->setAxisTitle(QwtPlot::xBottom, QString::fromUtf8("Time (ms)"));
	plot->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("Temperature"));

	//Set up the main layout
	mainLayout = new QHBoxLayout;	     //horizontal layout
	mainLayout->addWidget(plot);         //set up the plot as first slot
	mainLayout->addWidget(TempCountdownVertSplit);   //set up the tempscale + countdown timer as second slot
	setLayout(mainLayout);
}

void Window::createTempScale()
{
	TempScale = new QGroupBox(tr("Temp. Scale"));

	TempScale->setStyleSheet("QGroupBox {border-image: url(./pics/Smart.png)} "); //placeholder background

	QVBoxLayout *layout = new QVBoxLayout;
	QPushButton *Button1 = new QPushButton(tr("Celsius"));
	QPushButton *Button2 = new QPushButton(tr("Farenheit"));

	//Adding components to the layout
	layout->addWidget(Button1);
	layout->addWidget(Button2);
	TempScale->setLayout(layout);

	//Functionality of Buttons
//	connect(Button1, SIGNAL(clicked()), SLOT(setDegC())); //clicking this activates function setDegC()
//	connect(Button2, SIGNAL(clicked()), SLOT(setDegF())); //activates setDegF

	//Buttons Design
	Button1->setStyleSheet("QWidget {border-image: url(./pics/orangepaint.png) }");
	Button2->setStyleSheet("QWidget {border-image: url(./pics/greenpaint.png) }");
}


void Window::createCountdownBox()
{
	CountdownBox = new QGroupBox(tr("Countdown to message"));
	CountdownBox->setStyleSheet("QGroupBox {border-image: url(./pics/Milk.png)} "); //Placeholder Background
	QVBoxLayout *layout = new QVBoxLayout;

	//This label takes the countdown to sending a message
        //Initialising timer
        QTimer       *timer = new QTimer;
        connect(timer, SIGNAL(timeout()), SLOT(startCountdown()));
        timer->setInterval(1000);
        timer->start();

	reading = new QLabel;
	layout->addWidget(reading);

	CountdownBox->setLayout(layout);
}

void Window::createTempCountdownVertSplit()
{
	TempCountdownVertSplit = new QGroupBox(tr("Smart Milk Monitor"));
	QVBoxLayout *layout = new QVBoxLayout;		//Vertical Box Layout
	layout->addWidget(TempScale);	//First slot of the Box
	layout->addWidget(CountdownBox);		//Second slot of the Box
	TempCountdownVertSplit->setLayout(layout);
}

void Window::timerEvent(QTimerEvent *)
{
	double a;
	double inVal = tempreadbuster(&a);	//inVal takes the temperature's value from the test function

	//Leaving this in as an option to set up an LED
	//wiringPiSetup();			//Set up WiringPI library
	//pinMode(1, OUTPUT);			//Set up GPIO 18 as OUTPUT

								//Add new reading to the plot
	memmove(yData, yData + 1, (plotDataSize - 1) * sizeof(double));
	yData[plotDataSize - 1] = inVal;


	curve->setSamples(xData, yData, plotDataSize);
	plot->replot();
	printf("%.3f C\n", inVal);		//Print current temperature in terminal

}

void Window::startCountdown()
{

//	double a;
//	double inVal = tempreadbuster(&a); //intake values for temp.
//	QString s = QString::number(time_outoffridge);
//	QString k = QString::number(time_atroomtemp);
//	reading->setText(s);			//Displays countdown on QT
	// In future iterations will try to implement as a proper countdown clock with minutes and seconds

	//testing countdown function: for t > 24C (holding sensor in hand),
//	if (inVal > fridgeTemp)
//	{
//		running = true;
//	}
//	else
//	{
//		time_outoffridge = 30; //reset timer
//		s = QString::number(time_outoffridge);
//		reading->setText(s);
//		running = false;
//		running2 = false;
//	}
//
//	if (time_outoffridge >= 1 && running)
//	{
//		QString s = QString::number(time_outoffridge);
//		reading->setText(s);
//		if (time_outoffridge == 1 && running)			//Displays countdown on QT
//			{
//				std::cout << "Message 1 sent" << std::endl; //replace action with executing prowl script
//				system("./shellScript1.sh"); // run prowl1.pl through shell script from .cpp file
//				//temp will stay > fridgeTemp so this condition always true -> message 1 will be repeatedly sent
//				running = false; //use this to stop this timer from triggering message 1 
//			}
//	time_outoffridge--;
//	}
//	if (inVal >= roomTempLow && inVal <= roomTempHigh)
//	{
//		running2 = true;
//	}
//	if (time_atroomtemp >= 1   && running2)
//	{
//	QString k = QString::number(time_atroomtemp);
//        reading->setText(k);                    //Displays countdown on
//		if (time_atroomtemp == 25 && running2) // delay to allow temp tp settle 
//		{
//                std::cout << "Message 2 sent" << std::endl; //replace action wi$
//		 system("./shellScript2.sh"); // run prowl$
//
//		}
//		else if (time_atroomtemp == 1 && running2)
//		{
//		 std::cout << "Message 3 sent" << std::endl; //replace action
//		 system("./shellScript3.sh"); // run prowl$
//
//		}
//	 time_atroomtemp--;
//	}
}



//Button1 function - Display in deg. C
//void Window::setDegC()
//{
// Code here
//}

//Button2 function - Display in deg. F
//void Window::setDegF()
//{
// Code here
//}

// Function to turn an LED ON when milk temp is high and timer has passed enough seconds
//if [condition 1] && [condition 2] {
//	digitalWrite(1, 1); // turns on LED
//else
//	digitalWrite(1, 0); // turns off LED
//}
