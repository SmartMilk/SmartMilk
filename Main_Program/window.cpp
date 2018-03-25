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
   if (isCelsius)
   {
	   *a = tempC / 1000;
   }
   else {
	   *a = (tempC / 1000) * 9 / 5 + 32
   }
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
		pen.setColor("orange");
		pen.setWidth(2);
		curve->setPen(pen);
	}
	else 
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
	TempScale = new QGroupBox(tr("Temp. Scale"));

	// NEED TO FIX THIS
	//TempScale->setStyleSheet("QGroupBox {border-image: url(./pics/Smart.png)} "); //placeholder background

	QVBoxLayout *layout = new QVBoxLayout;
	QPushButton *Button1 = new QPushButton(tr("Celsius"));
	QPushButton *Button2 = new QPushButton(tr("Farenheit"));

	//Adding components to the layout
	layout->addWidget(Button1);
	layout->addWidget(Button2);
	TempScale->setLayout(layout);

	//Functionality of Buttons
	connect(Button1, SIGNAL(clicked()), SLOT(setDegC())); //clicking this activates function setDegC()
	connect(Button2, SIGNAL(clicked()), SLOT(setDegF())); //activates setDegF

	//Buttons Design
	Button1->setStyleSheet("QWidget {border-image: url(./pics/orangepaint.png) }");
	Button2->setStyleSheet("QWidget {border-image: url(./pics/greenpaint.png) }");
}


void Window::createMessageBox()
{
	MessageBox = new QGroupBox(tr("User Messages"));
	
	// NEED TO FIX THIS STYLE
	//MessageBox->setStyleSheet("QGroupBox {border-image: url(./pics/Milk.png)} "); 
	
	QVBoxLayout *layout = new QVBoxLayout;

	//This label takes the countdown to sending a message
        //Initialising timer
        QTimer       *timer = new QTimer;
        connect(timer, SIGNAL(timeout()), SLOT(startCountdown()));
        timer->setInterval(1000);
        timer->start();
		
	/*------------------------------*/
	// MESSAGE LABELS
	// Initialise labels with messages
	
	// Dynamic user warning label. Different from message labels - inform user when T thresholds exceeded.
	// Changes message instead of changing colour.
	QLabel* reading = new QLabel;
	
	// Static message labels
	QLabel* message1 = new QLabel;
		message1->setText("Message 1 sent");
	QLabel* message2 = new QLabel;
		message2->setText("Message 2 sent");
	QLabel* message3 = new QLabel;
		message3->setText("Message 3 sent");
	
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

	// Call in temperature values 
	double a;
	double inVal = tempreadbuster(&a); //intake values for temp.
	
	//--------------------------
	/* MESSAGE 1: */

	if (inVal > fridgeTemp)
	{
		running = true;
		reading->setText("Milk is out of the fridge."); // warning message to real-time plot that Tm > Tf
	}
	else
	{
		time_outoffridge = 30; //reset timer
		reading->setText("Milk temperature is OK."); // message that milk is in correct storage conditions
		running = false;
		running2 = false;
	}

	//if (time_outoffridge >= 1 && running)
	//{
	//	QString s = QString::number(time_outoffridge);
	//	reading->setText(s);
	
	
	if (time_outoffridge == 0 && running)			//Displays countdown on QT
	{
		system("./Prowl_Scripts/shellScript1.sh"); // run prowl1.pl through shellScript1.sh
		message1->setStyleSheet("QLabel { background-color : red}"); // change label colour upon condition being met	
		running = false; //use this to stop this timer from triggering message 1 
	
	}
	//time_outoffridge--;
	//}
	
	//---------------------------
	/* MESSAGE 2 */
	
	if (inVal >= roomTempLow && inVal <= roomTempHigh)
	{
		reading->setText("Milk is near room temperature"); // Tm will fluctuate, so create bandwidth
		running2 = true;
	}
	
	//if (time_atroomtemp >= 1   && running2)
	//{
	//QString k = QString::number(time_atroomtemp);
       // reading->setText(k);   
		
	if (time_atroomtemp == 25 && running2) // delay 5secs to allow time for Tm to stay at Tr. If Tm still Tr, send message
	{
		system("./Prowl_Scripts/shellScript2.sh"); // run prowl2.pl through shellScript2.sh
		message2->setStyleSheet("QLabel { background-color : red}"); // indicate message activation
	}
	
	//---------------------------
	/* MESSAGE 3 */
	else if (time_atroomtemp == 1 && running2) // if Tm=Tr for certain length of time, send 3rd message
	{
		system("./Prowl_Scripts/shellScript3.sh"); // run prowl3.pl through shellScript3.sh
		message3->setStyleSheet("QLabel { background-color : red}"); // indicate message activation
	}
	
	//time_atroomtemp--;
	//}
}



//Button1 function - Display in deg. C
void Window::setDegC()
{
	isCelsius = true;
}

//Button2 function - Display in deg. F
void Window::setDegF()
{
	isCelsius = false;
}

