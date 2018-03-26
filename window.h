#//This file defines all the classes and objects used in window.cpp

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
#include <QPen>

class Window : public QWidget //Derive class 'window' from the class 'Qwidget'
{
	// must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
	Q_OBJECT

public:
	Window(); // default constructor - called when a Window is declared without arguments

        const double fridgeTemp = 24.5; //temperature threshold for activating first message
	const double roomTempLow = 26.5; //temperature threshold for activating second and third messags
	const double roomTempHigh = 27.0;
	const double Tf = 24.5;
	const double Tr = 27.0;
        int time_outoffridge = 20;
	int time_atroomtemp = 30; //countdown starting value, is currently placeholder 3 minutes
        bool running = false;
	bool running2 = false;
	bool isCelsius = true;

 private slots:
    void setFridgeDegC();  //Set the temperature to degrees C
    void setRoomDegC();  //Set the temperature to degrees C
    void setFridgeDegF();  //Set the temperature to degrees F
    void setRoomDegF();  //Set the temperature to degrees F
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
