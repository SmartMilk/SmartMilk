#include "window.h"
#include "tempread.h"
#include <QtGui>
#include <QFont>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>


Window::Window() //constructor
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
	curve->setPen(QPen(Qt::yellow, 2)); //temperature plot
	curve1 = new QwtPlotCurve;
        curve1->setPen(QPen(Qt::blue,2)); //fridge threshold plot
	curve2 = new QwtPlotCurve;
	curve2->setPen(QPen(Qt::red,2)); //room temperature plot
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
	plot->setStyleSheet("QWidget {color: qlineargradient(spread:pad, x1:0 y1:0, x2:1 y2:0, stop:0 rgba(0,0,0,255), stop:1 rgba(255,255,255,255));"
				      "background: qlineargradient(x1:0 y1:0, x2:1 y2:0, stop:0 lightGray , stop:1 darkGray); }");
	//^ gives a nice gradient colour background (light -> dark gray)

	plot->setAxisTitle(QwtPlot::xBottom, QString::fromUtf8("Data Index"));
	plot->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("Temperature"));

	//Set up the main layout
	mainLayout = new QHBoxLayout;	     //horizontal layout
	mainLayout->addWidget(plot);         //set up the plot as first slot
	mainLayout->addWidget(TempCountdownVertSplit);   //set up the tempscale + countdown timer as second slot
	setLayout(mainLayout);

	//Initialising timers
	//timerP: controls when the plotUpdate function is executed (every 500ms)
	//timerCD: controls when the startCountdown function is executed (every 1000ms) 

	timerP = new QTimer;
	connect(timerP, SIGNAL(timeout()), SLOT(plotUpdate()));
	timerP->setInterval(500);
	timerP->start();

	timerCD = new QTimer;
	connect(timerCD, SIGNAL(timeout()), SLOT(startCountdown()));
	timerCD->setInterval(1000);
	timerCD->start();

	t.start(); //start tempread thread


}

Window::~Window() //destructor
{
	t.quit(); //terminates tempread thread
}

void Window::createTempScale() //Creates buttons which switch temperature plot from celsius to farenheit and vice versa
{

	TempScale = new QGroupBox(tr("Temperature Scale"));

	TempScale->setStyleSheet("background-color: pink"); //background

	QVBoxLayout *layout = new QVBoxLayout;
	QPushButton *Button1 = new QPushButton(tr("Celsius"));
	QPushButton *Button2 = new QPushButton(tr("Farenheit"));

	//Adding components to the layout
	layout->addWidget(Button1);
	layout->addWidget(Button2);
	TempScale->setLayout(layout);

	//Functionality of Button1
	connect(Button1, SIGNAL(clicked()), SLOT(setCelsius()));
	//Functionality of Button2
	connect(Button2, SIGNAL(clicked()), SLOT(setFarenheit()));

	//Button colours
	Button1->setStyleSheet("background-color:yellow");
	Button2->setStyleSheet("background-color:green");

}


void Window::createMessageBox() //status indicators for milk and messages, these light up red when a message has been sent
{

	MessageBox = new QGroupBox(tr("User Messages"));
	MessageBox->setStyleSheet("background-color: pink"); //Background
	QVBoxLayout *layout = new QVBoxLayout;

	// Message labels
	// Dynamic user warning label. Inform user when T thresholds exceeded
	reading = new QLabel;
	reading->setText("OK");

	//QTime countdown
	timer1 = new QTime(0, 15, 0);
	timer1label = new QLabel;
	timer1label->setText(timer1->toString());

	timer2 = new QTime(2, 0, 0);
	timer2label = new QLabel;
	timer2label->setText(timer2->toString());

	//message indicators
	message1 = new QLabel;
	message1->setText("Message 1 Sent");
	message2 = new QLabel;
	message2->setText("Message 2 Sent");
	message3 = new QLabel;
	message3->setText("Message 3 Sent");

	reading->setStyleSheet("QLabel {background-color : white}");
	timer1label->setStyleSheet("QLabel {background-color : white}");
	timer2label->setStyleSheet("QLabel {background-color : white}");
	message1->setStyleSheet("QLabel {background-color : white}");
	message2->setStyleSheet("QLabel {background-color : white}");
	message3->setStyleSheet("QLabel {background-color : white}");


	// Set layout for each label
	layout->addWidget(reading);
	layout->addWidget(timer1label);
	layout->addWidget(timer2label);
	layout->addWidget(message1);
	layout->addWidget(message2);
	layout->addWidget(message3);

	// Set overall layout
	MessageBox->setLayout(layout);

}

void Window::createTempCountdownVertSplit() //separates the status box from the temperature scale box vertically
{

	TempCountdownVertSplit = new QGroupBox(tr("SmartMilk Monitor"));
	QVBoxLayout *layout = new QVBoxLayout;		//Vertical Box Layout
	layout->addWidget(TempScale);	//First slot of the Box
	layout->addWidget(MessageBox);	//Second slot of the Box
	TempCountdownVertSplit->setLayout(layout);

}

void Window::plotUpdate() //controls each new plot point of temperature
{
	double inVal = t.signalData(); //intakes the temperature value from the tempread class
	double truput; //resulting QT y data point

	//changing temperature depending on whether the celsius or farenheit buttons have been clicked (default: celsius)
	if (isCelsius == false)
	{
		truput = inVal *9/5 + 32;
		Tf = fridgeTemp *9/5 + 32;
		Tr = roomTempHigh *9/5 + 32;
		curve->setPen(QPen(Qt::green, 2));

	}
	else
	{
		truput = inVal;
		Tf = fridgeTemp;
		Tr = roomTempHigh;
		curve->setPen(QPen(Qt::yellow, 2));
	}

	//sometimes the temperature value scales by a factor of 10 or 100, these if statements corrects this error
	if (truput > 1000.0)
	{
	truput = truput/100 ;
	}
	if (truput > 100.0 && truput < 100.0)
	{
	truput = truput/10 ;
	}

	//Add new reading to the plot
	memmove(yData, yData + 1, (plotDataSize - 1) * sizeof(double));
	memmove(y1Data, y1Data + 1, (plotDataSize -1) *sizeof(double));
	memmove(y2Data, y2Data + 1, (plotDataSize -1) *sizeof(double));
	yData[plotDataSize - 1] = truput;
	y1Data[plotDataSize -1] = Tf;
	y2Data[plotDataSize -1] = Tr;


	curve->setSamples(xData, yData, plotDataSize);
	curve1->setSamples(xData,y1Data, plotDataSize);
	curve2->setSamples(xData,y2Data, plotDataSize);
	plot->replot();
	printf("%.3f C\n", truput); //Print current temperature in terminal

}

void Window::startCountdown()
{
  // 2 timers to activate prowl messages: first timer activates when milk is out of fridge, sending message 1
  //	second  timer activates when milk approaches room tempterature, sending messages 2 and 3


	double inVal = t.signalData(); //intake values for temp.


	//-------------------------
	// MESSAGE 1

	if (inVal > fridgeTemp && inVal <= roomTempLow) //When Milk has been left out of fridge, timer 1 initiates
	{
		running = true;
		reading->setText("Milk out of Fridge");

		running2 = false; //If milk falls below room temperature (stored in fridge after 1st warning, 2nd timer is stopped
		time_atroomtemp = 2*60*60;
                timer2->setHMS(2,0,0); 
                timer2label->setText(timer2->toString());
                timer2label->setStyleSheet("QLabel {background-color : white}");
		message2 -> setStyleSheet("QLabel {background-color : white}");
		message3 -> setStyleSheet("QLabel {background-color : white}");


		if (time_outoffridge >= 1 && running)
		{

			if (time_outoffridge == 1 && running) //activates once timer has reached 1
			{
				std::cout << "Message 1 sending..." << std::endl;
				system(shellScript1); // run prowl1.pl through shell script from .cpp file
				message1->setStyleSheet("QLabel {background-color : red}");
				timer1label->setStyleSheet("QLabel {background-color : yellow}");
				running = false; //use this to stop this timer from re-triggering message 1

			}

		time_outoffridge--;
		timer1->setHMS(0, timer1->addSecs (-1).minute (), timer1->addSecs (-1).second());
		timer1label->setText(timer1->toString()); //updates QT timer

		}
	}
	else if (inVal <= fridgeTemp) //activates when milk is back at fridge temperature
	{
		// Reset timers and labels  when milk temp below fridgeTemp
		time_outoffridge = 15*60; //reset timer1 to 15mins
		time_atroomtemp = 2*60*60; // reset timer2 to 2hrs
		reading->setText("Milk OK");
		running = false;
		message1->setStyleSheet("QLabel {background-color : white}");
		message2->setStyleSheet("QLabel {background-color : white}");
		message3->setStyleSheet("QLabel {background-color : white}");
		timer1->setHMS(0,15,0);
		timer1label->setText(timer1->toString());
		timer1label->setStyleSheet("QLabel {background-color : white}");
	}



	//-----------------------
	// MESSAGE 2 & 3

	if (inVal > roomTempLow && inVal <= roomTempHigh)
	{
		reading->setText("Milk at room temp.");
		running2 = true;
		if (time_atroomtemp >= 1 && running2)
		{
			if (time_atroomtemp == (2*60*60) - 20 && running2) // delay for 20s to make sure milk stays at room temp. THEN send message 2. 
			{
				std::cout << "Message 2 sending..." << std::endl;
				system(shellScript2); // run prowl2.pl
				message2->setStyleSheet("QLabel {background-color : red}");
				timer2label->setStyleSheet("QLabel {background-color : yellow}");
			}
			else if (time_atroomtemp == 1 && running2) //activates at end of second timer
			{
				std::cout << "Message 3 sending..." << std::endl;
				system(shellScript3); // run prowl3.pl
				message3->setStyleSheet("QLabel {background-color : red}");
				timer2label->setStyleSheet("QLabel {background-color : red}");
				running2 = false;
			}
		 time_atroomtemp--;
                 timer2->setHMS(timer2->addSecs(-1).hour (), timer2->addSecs (-1).minute (), timer2->addSecs (-1).second());
                 timer2label->setText(timer2->toString()); //update 2nd QT timer


		}
	}

}


/* Threshold functions */

//Button1 function - Display Tf, Tm, Tr in deg. C
// Return fridge temperature threshold in degC
void Window::setCelsius()
{
	isCelsius = true;
}

//Button2 function - Display Tf, Tm, Tr in deg. F
// Return fridge temperature threshold in degF
void Window::setFarenheit()
{
	isCelsius = false;
}
