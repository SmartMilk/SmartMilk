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
#include "tempread.h"

class Window : public QWidget //Derive class 'window' from the class 'Qwidget'
{
	// must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
	Q_OBJECT

public:
	Window(); // class constructor
	~Window(); //class destructor
    const double fridgeTemp = 25.0; //temperature threshold for activating first message
	const double roomTempLow = 27.5; //temperature threshold for activating second and third messages
	const double roomTempHigh = 28.0;
	double Tf = 25.0; //Fridge temp. threshold plotted on QT
	double Tr = 28.0; //Room temp. threshold plotted on QT
    int time_outoffridge = 10; //Countdown timer default values
	int time_atroomtemp = 15;
    bool running = false;
	bool running2 = false;
	bool isCelsius = true;

 private slots:
    void setCelsius();  //Set the temperatures to degrees C
    void setFarenheit();  //Set the temperatures to degrees F
    void plotUpdate();
    void startCountdown();

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
	QLabel	     *timer1status;
	QLabel       *timer2status;
	QPushButton  *Button1;
	QPushButton  *Button2;
	QTimer       *timerCD;
	QTimer	     *timerP;

	//The main Layout which will contain all the GUI elements
	QHBoxLayout  *mainLayout;  // horizontal layout

	static const int plotDataSize = 60;

	// data arrays for the plot
	double xData[plotDataSize];
	double yData[plotDataSize];
	double y1Data[plotDataSize];
	double y2Data[plotDataSize];
	Tempread t; // temperature reading thread

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
