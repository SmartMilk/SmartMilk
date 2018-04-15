#//This file defines all the classes and objects used in window.cpp
#ifndef WINDOW_H
#define WINDOW_H

#include <qwt/qwt_thermo.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <QBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <qwidget.h>
#include <qfont.h>
#include <QMainWindow>
#include <QTimer>
#include <QPen>
#include <QSplashScreen>
#include <QTime>
#include "tempread.h"

class Window : public QWidget //Derive class 'window' from the class 'Qwidget'
{
	// must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
	Q_OBJECT

public:
    	Window(); // class constructor
    	~Window(); //class destructor
    	const double fridgeTemp = 5.0; //temperature threshold for activating first message
    	const double roomTempLow = 19.5; //temperature threshold for activating second and third messages
   	const double roomTempHigh = 21.0;
    	double Tf = 5.0; //Fridge temp. threshold plotted on QT
    	double Tr = 21.0; //Room temp. threshold plotted on QT
    	int time_outoffridge = 15*60; // 15mins for countdown timer1 - message1
    	int time_atroomtemp = 2*60*60; // 2hrs countdown for timer2-message3
    	bool running = false; //boolean variables which control timer activations
    	bool running2 = false;
    	bool isCelsius = true; //boolean variable which controls whether plot is done in Celsius or Farenheit

 private slots:
    	void setCelsius();  //Set the temperatures to degrees C
    	void setFarenheit();  //Set the temperatures to degrees F
    	void plotUpdate(); //updates QT plot
    	void startCountdown(); //countdown to sending prowl messages

private:
	//These functions are for creating all the components in the GUI. The components are divided by groups(Qt Groups)

	void createTempScale();  //This function creates celsius and farenheit push button GUIs
	void createMessageBox();  //Creates a GUI slot for a countdown timer for sending Prowl Messages
	void createTempCountdownVertSplit();   //Function splits the tempscale and countdown boxes vertically

	//Qt Groups. Each group is created inside of its own function
	QGroupBox *TempScale;
	QGroupBox *MessageBox;
	QGroupBox *TempCountdownVertSplit;

	//Declaring pointers

	QwtThermo    *thermo;
	QwtPlot      *plot;
	QwtPlotCurve *curve;
	QwtPlotCurve *curve1;
	QwtPlotCurve *curve2;
	QLabel       *reading;
	QLabel       *message1;
	QLabel	     *message2;
	QLabel       *message3;
	QPushButton  *Button1;
	QPushButton  *Button2;
	QTimer       *timerCD;
	QTimer	     *timerP;
	QTime        *timer1;
	QTime        *timer2;
	QLabel       *timer1label;
	QLabel       *timer2label;

	//The main Layout which will contain all the GUI elements
	QHBoxLayout  *mainLayout;  // horizontal layout

	static const int plotDataSize = 60;

	// data arrays for the plot
	double xData[plotDataSize];
	double yData[plotDataSize];
	double y1Data[plotDataSize];
	double y2Data[plotDataSize];

	Tempread t; // temperature reading Qthread

};

#endif // WINDOW_H

/*Create each of the shell scripts*/
#define shellScript1 "\
#!/bin/bash \n\
perl prowl1.pl \n\
clear\
"
#define shellScript2 "\
#!/bin/bash \n\
perl prowl2.pl \n\
clear\
"
#define shellScript3 "\
#!/bin/bash \n\
perl prowl3.pl \n\
clear\
"
