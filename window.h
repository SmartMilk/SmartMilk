//This file defines all the classes and objects used in window.cpp (or temp_read_file.cpp)

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

class Window : public QWidget
{
	// must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
	Q_OBJECT

public:
	Window(); // default constructor - called when a Window is declared without arguments

	void timerEvent(QTimerEvent *);

	// internal variables for the window class
	private slots:
	void setDegC();  //Set the temperature to degrees C
	void setDegF();  //Set the temperature to degrees F
	void countdown(): //Countdown to Prowl Message send


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

	bool flag;
};

#endif // WINDOW_H
