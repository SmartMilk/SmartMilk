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
#include <QThread>

class Window : public QWidget //Derive class 'window' from the class 'Qwidget'
{
	// must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
	Q_OBJECT

public:
	Window(); // default constructor - called when a Window is declared without arguments

    const double fridgeTemp = 24.5; //temperature threshold for activating first message
	const double roomTempLow = 26.5; //temperature threshold for activating second and third messages
	const double roomTempHigh = 27.0;
	double Tf = 24.5; //Fridge temp. threshold plotted on QT
	double Tr = 27.0; //Room temp. threshold plotted on QT
    int time_outoffridge = 20; //Countdown timer default values
	int time_atroomtemp = 30; 
    bool running = false;
	bool running2 = false;
	bool isCelsius = true;

 private slots:
    void setCelsius();  //Set the temperatures to degrees C
    void setFarenheit();  //Set the temperatures to degrees F
    void timerEvent(QTimerEvent *);
    void startCountdown();


private:
	//These functions are for creating all the components in the GUI. The components are divided by groups(Qt Groups)

	void createTempScale();  //This function creates celsius and farenheit push button GUIs
	void createMessageBox();  //Creates a GUI slot for a countdown timer for sending Prowl Messages
	void createTempCountdownVertSplit();   //Function splits the tempscale and countdown boxes vertically

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

	//Qt Groups. Each group is created inside of its own function
	QGroupBox *TempScale;
	QGroupBox *MessageBox;
	QGroupBox *TempCountdownVertSplit;

	//The main Layout which will contain all the GUI elements
	QHBoxLayout  *mainLayout;  // horizontal layout

	static const int plotDataSize = 60;

	// data arrays for the plot
	double xData[plotDataSize];
	double yData[plotDataSize];
	double y1Data[plotDataSize];
	double y2Data[plotDataSize];

	bool flag;
};

#endif // WINDOW_H
