#ifndef WINDOW_H
#define WINDOW_H

#include <QLabel>
#include <QPushButton>
#include <QPen>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <QBoxLayout>
#include "milkTempReader.h"

/* The tempRead.h file contains the class definitions, and int & char variables to be used in the window.cpp script. 
   It should also contain the 
*/

// class definition 'Window'
class Window : public QWidget {
  
  // must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
  Q_OBJECT

 public:
  Window(); // default constructor - called when a Window is declared without arguments

  ~Window();

  void timerEvent( QTimerEvent * );

  public slots: // for the changes to the plot upon pressing a specific button
    /*
     * Thresholds for the fridge temperature (Tf) and room temperature (Tr). 
     * These functions will be implemented and call upon the thresholds 
     * defined below when a button is pressed.
     */
    double setLowerFahrThreshold(); 
    double setHigherFahrThreshold(); 
    int setLowerCelsThreshold(); 
    int setHigherCelsThreshold();
    
    // Internal variables for the window class
 private:
   
   // plots: lower threshold; milk temp in real-time; upper threshold
    QwtPlot      *plot;
    QwtPlotCurve *curve;  
    QwtPlotCurve *curve1;
    QwtPlotCurve *curve2;

    // buttons: for converting the temperature from degC to degF
    QPushButton *celsButton; // degC
    QPushButton *fahrButton; // degF
    
    // label: for displaying the messages on the milks condition over time
    QLabel       *Label1; 

    // layout elements from Qt itself http://qt-project.org/doc/qt-4.8/classes.html
    QVBoxLayout  *vLayout;  // vertical layout
    QHBoxLayout  *hLayout;  // horizontal layout

    static const int plotDataSize = 100;

    // data arrays for the plot
    double xData[plotDataSize];
    double yData[plotDataSize];
    double y1Data[plotDataSize];
    double y2Data[plotDataSize];
    
    //Thresholds for the Qt plot
    double fahrThresholdLow;
    double fahrThresholdHigh;
    int celsThresholdLow;
    int celsThresholdHigh;

};

#endif // WINDOW_H

