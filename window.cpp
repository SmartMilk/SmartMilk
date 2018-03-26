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

 *a = tempC / 1000;
  double result = *a;
  return result;
  }
  close(fd);
 }
} //tempreadbuster function ends here

Window::Window() : Tf(24.5), Tr(27.0)
// Function calls upon the window header and continues to define elements
{
	//These functions creates all the GUI elements except the main Layout

	createTempScale();
	createMessageBox();
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
	if (isCelsius)
	{
	        curve->setPen(QPen(Qt::yellow, 2));
	}
	else if (isCelsius == false)
	{
		curve->setPen(QPen(Qt::green, 2));
	}

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
	//plot->setStyleSheet("QWidget {border-image: url(./pics/milkhasgonebad.png) }");

	plot->setAxisTitle(QwtPlot::xBottom, QString::fromUtf8("Data Index"));
	plot->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("Temperature"));

	//Set up the main layout
	mainLayout = new QHBoxLayout;	     //horizontal layout
	mainLayout->addWidget(plot);         //set up the plot as first slot
	mainLayout->addWidget(TempCountdownVertSplit);   //set up the tempscale + countdown timer as second slot
	setLayout(mainLayout);
}

void Window::createTempScale()
{
	TempScale = new QGroupBox(tr("Temperature Scale"));

	//TempScale->setStyleSheet("QGroupBox {border-image: url(./pics/Smart.png)} "); //placeholder background

	QVBoxLayout *layout = new QVBoxLayout;
	QPushButton *Button1 = new QPushButton(tr("Celsius"));
	QPushButton *Button2 = new QPushButton(tr("Farenheit"));

	//Adding components to the layout
	layout->addWidget(Button1);
	layout->addWidget(Button2);
	TempScale->setLayout(layout);

	//Functionality of Button1
	connect(Button1, SIGNAL(clicked()), SLOT(setFridgeDegC())); 
	connect(Button1, SIGNAL(clicked()), SLOT(setRoomDegC()));
	
	// Functionality of Button2
	connect(Button2, SIGNAL(clicked()), SLOT(setFridgeDegF())); 
	connect(Button2, SIGNAL(clicked()), SLOT(setRoomDegF())); 

	//Buttons Design
	//Button1->setStyleSheet("QWidget {border-image: url(./pics/orangepaint.png) }");
	//Button2->setStyleSheet("QWidget {border-image: url(./pics/greenpaint.png) }");
}


void Window::createMessageBox()
{
	MessageBox = new QGroupBox(tr("User Messages"));
	//MessageBox->setStyleSheet("QGroupBox {border-image: url(./pics/Milk.png)} "); //Placeholder Background
	QVBoxLayout *layout = new QVBoxLayout;

        //Initialising timer
        QTimer       *timer = new QTimer;
        connect(timer, SIGNAL(timeout()), SLOT(startCountdown()));
        timer->setInterval(1000);
        timer->start();

	// Message labels
	// Dynamic user warning label. Inform user when T thresholds exceeded
	reading = new QLabel;

	// Static message labels
	message1 = new QLabel;
		message1->setText("Message 1 Sent");
        message2 = new QLabel;
		message2->setText("Message 2 Sent");
        message3 = new QLabel;
		message3->setText("Message 3 Sent");

	// Set layout for each label
	layout->addWidget(reading);
	layout->addWidget(message1);
	layout->addWidget(message2);
	layout->addWidget(message3);

	// Set overall layout
	MessageBox->setLayout(layout);
}

void Window::createTempCountdownVertSplit()
{
	TempCountdownVertSplit = new QGroupBox(tr("SmartMilk Monitor"));
	QVBoxLayout *layout = new QVBoxLayout;		//Vertical Box Layout
	layout->addWidget(TempScale);	//First slot of the Box
	layout->addWidget(MessageBox);	//Second slot of the Box
	TempCountdownVertSplit->setLayout(layout);
}

void Window::timerEvent(QTimerEvent *)
{
	double a;
	double inVal = tempreadbuster(&a);	//inVal takes the temperature's value from the test function
	double truput; //resulting output
	if (isCelsius == false)
	{
		truput = inVal *9/5 + 32;
	}
	else
	{
		truput = inVal;
	}
	//Leaving this in as an option to set up an LED
	//wiringPiSetup();			//Set up WiringPI library
	//pinMode(1, OUTPUT);			//Set up GPIO 18 as OUTPUT

	//Add new reading to the plot
	memmove(yData, yData + 1, (plotDataSize - 1) * sizeof(double));
	yData[plotDataSize - 1] = truput;


	curve->setSamples(xData, yData, plotDataSize);
	plot->replot();
	printf("%.3f C\n", inVal); //Print current temperature in terminal

}

void Window::startCountdown()
{

	double a;
	double inVal = tempreadbuster(&a); //intake values for temp.
	//QString s = QString::number(time_outoffridge);
	//QString k = QString::number(time_atroomtemp);
	//reading->setText(s);			//Displays countdown on QT

	//-------------------------
	// MESSAGE 1

	if (inVal > fridgeTemp)
	{
		running = true;
		reading->setText("Milk is out of the fridge");
	}
	else
	{
		time_outoffridge = 20; //reset timer
		//s = QString::number(time_outoffridge);
		reading->setText("Milk temperature is OK");
		running = false;
		running2 = false;
	}

	if (time_outoffridge >= 1 && running)
	{
	//	QString s = QString::number(time_outoffridge);
	//	reading->setText(s);
		if (time_outoffridge == 1 && running)			//Displays countdown on QT
			{
				system("./shellScript1.sh"); // run prowl1.pl through shell script from .cpp file
				message1->setStyleSheet("QLabel {background-color : red}");
				running = false; //use this to stop this timer from triggering message 1 
			}
	time_outoffridge--;
	}

	//-----------------------
	// MESSAGE 2

	if (inVal >= roomTempLow && inVal <= roomTempHigh)
	{
		reading->setText("Milk is near room temperature");
		running2 = true;
	}
	if (time_atroomtemp >= 1   && running2)
	{
	//QString k = QString::number(time_atroomtemp);
        //reading->setText(k);                    //Displays countdown on
		if (time_atroomtemp == 25 && running2) //delay to allow temp tp settle 
		{
			system("./shellScript2.sh"); // run prowl$
			message2->setStyleSheet("QLabel {background-color : red}");
		}
		else if (time_atroomtemp == 1 && running2)
		{
		 	system("./shellScript3.sh"); // run prowl$
			message3->setStyleSheet("QLabel {background-color : red}");
		}
	 time_atroomtemp--;
	}
}


/* Threshold functions */

//Button1 function - Display Tf, Tm, Tr in deg. C
// Return fridge temperature threshold in degC
void Window::setFridgeDegC()
{
	isCelsius = true;
	Tf = fridgeTemp;
	return Tf;
}

//Button1 function - Display Tf, Tm, Tr in deg. C
// Return room temperature threshold in degC
void Window::setRoomDegC()
{
	isCelsius = true;
	Tr = roomTempHigh;
	return Tr;
}

//Button2 function - Display Tf, Tm, Tr in deg. F
// Return fridge temperature threshold in degF
void Window::setFridgeDegF()
{
	isCelsius = false;
	Tf = fridgeTemp*9/5 + 32;
	return Tf;
}

//Button2 function - Display Tf, Tm, Tr in deg. F
// Return room temperature threshold in degF
void Window::setRoomDegF()
{
	isCelsius = false;
	Tr = roomTempHigh*9/5 + 32;
	return Tr;
}

