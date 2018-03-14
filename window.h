//This file defines all the classes and objects used in window.cpp 

//for timer, trying to merge with timer-shutdown ( https://github.com/raymon1/timer-shutdown )
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

class Window : public QWidget //Derive class 'window' from the class 'Qwidget'
{
	// must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
	Q_OBJECT

public:
	Window(); // default constructor - called when a Window is declared without arguments
       // void decrementCountdown();

        const double critTemp = 24.0; //temperature threshold to activate countdown
        int time_seconds = 180; //countdown starting value, is currently placeholder 3 minutes
        bool running = false;

 private slots:
//	void setDegC();  //Set the temperature to degrees C
//	void setDegF();  //Set the temperature to degrees F
        void timerEvent(QTimerEvent *);
	void startCountdown();


private:
	//These functions are for creating all the components in the GUI. The components are divided by groups(Qt Groups)

	void createTempScale();  //This function creates celsius and farenheit push button GUIs
	void createCountdownBox();  //Creates a GUI slot for a countdown timer for sending Prowl Messages
	void createTempCountdownVertSplit();   //Function splits the tempscale and countdown boxes vertically

	//Declaring pointers

	QwtThermo    *thermo;
	QwtPlot      *plot;
	QwtPlotCurve *curve;
	QLabel       *reading;
	QLabel       *space;
	QLabel       *manual;
	QPushButton  *heaterLed;
	QPushButton  *heaterOff;

	//Qt Groups. Each group is created inside of its own function
	QGroupBox *TempScale;
	QGroupBox *CountdownBox;
	QGroupBox *TempCountdownVertSplit;

	//The main Layout which will contain all the GUI elements
	QHBoxLayout  *mainLayout;  // horizontal layout

	static const int plotDataSize = 60;

	// data arrays for the plot
	double xData[plotDataSize];
	double yData[plotDataSize];

	// countdown variables:
//	const double critTemp = 24.0; //temperature threshold to activate countdown
//	int time_seconds = 180; //countdown starting value, is currently placeholder 3 minutes
//	bool running = false;

	bool flag;
};

#endif // WINDOW_H
