/*
  A draft of the window.cpp code for the Qt plot. 
  In here you'll find the room temperature (Tr) and fridge temperature (Tf) thresholds
  being declared and called upon by setThreshold() classes nearer the bottom. This
  is for when the user clicks the degC or degF buttons to change the threshold values.
*/


#include "window.h"
#include <ctime>
#include <cmath>
#include "wiringPi.h"
#include "milkTempReader.h"

Window::Window() : Tf(4), Tr(18)
{
	// set up the initial plot data
	for( int index=0; index<plotDataSize; ++index )
	{
		xData[index]  = index;
		yData[index]  = 10;
		y1Data[index] = Tf; // low threshold is fridge temp (blue line)
		y2Data[index] = Tr; // high threshold is room temp (red line)
	}

	//Initialize curves
	curve  = new QwtPlotCurve;
	curve  -> setPen(QPen(Qt::green,2));
	curve1 = new QwtPlotCurve;
	curve1 -> setPen(QPen(Qt::red,2));
	curve2 = new QwtPlotCurve;
	curve2 -> setPen(QPen(Qt::blue,2));
	
	/* --------------------
	Initialise plots, buttons, labels and progressbar. For our application, we want:
	- a label for the message box
	  -> "Milk in fridge"" (while Tm <= Tf)
	  -> "Milk out of fridge (while Tm > Tf)
	  -> "Milk at room temperature" (when Tm = Tr)
	  -> "Milk now unsafe to drink" (if Tm = Tr for t = 120mins)
	- a button for switching to degC 
	- a button for switching to degF
	*/ --------------------
	
	// Call upon internal private variables from windowHeader for plotting, setting labels and buttons
	plot    = new QwtPlot;
	celsButton = new QPushButton("Degrees Celsius");
	FahrButton = new QPushButton("Degrees Fahrenheit");
	Label1  = new QLabel(this);

	//Connect buttons to their functions    
  connect( fahrButton, SIGNAL(clicked()), SLOT(setLowerFahrThreshold()) );
	connect( celsButton, SIGNAL(clicked()), SLOT(setLowerCelsThreshold()) );

	connect( fahrButton, SIGNAL(clicked()), SLOT(setHigherFahrThreshold()) );
	connect( celsButton, SIGNAL(clicked()), SLOT(setHigherCelsThreshold()) );

	// make a plot curve from the data and attach it to the plot
	curve->setSamples(xData, yData, plotDataSize);
	curve1->setSamples(xData,y1Data,plotDataSize);
	curve2->setSamples(xData,y2Data,plotDataSize);
  
	//attach curves to plot
	curve->attach(plot);
  curve1->attach(plot);
	curve2->attach(plot);	

	//Label axis 
	plot->setAxisTitle(QwtPlot::xBottom, "Time (minutes)");
	plot->setAxisTitle(QwtPlot::yLeft, "Milk Temperature");
	plot->replot();
	plot->show();
	
	// set up the vertical and horizontal layout 
	vLayout = new QVBoxLayout;
	vLayout->addWidget(Label1); // message box
	
	vLayout->addWidget(celsButton); // degC button
	vLayout->addWidget(fahrButton); // degF button
	
	hLayout = new QHBoxLayout;
	hLayout->addLayout(vLayout);
	hLayout->addWidget(plot);
	setLayout(hLayout);

}

Window::~Window() {
}

void Window::timerEvent( QTimerEvent * ) {
  
  //initialize output pin
  wiringPiSetup ();
  pinMode (1, OUTPUT);
  
  //call getData() function to get data points from temp sensor
  double temp = getTemp();
  double Tm = temp / 1000;
  
  if (Tm <= Tf) // if milk temp <= fridge temp (ie in fridge) NEED TO FIGURE OUT HOW TO SWITCH Tf BETWEEN DEGF AND DEGC
    {
      // display this message when the milk is still in the fridge
      Label1->setText("Milk is in the fridge");
    }
  
  else if (Tm > Tf) // if milk temp > fridge temp
    {
    
    // start timer when milk temperature begins to rise. HOW TO START A TIMER WITHIN QT???
    
    
    // code for starting timer -- when t = 30mins send message to users phone!
    // display this message when the milk is still in the fridge
      Label1->setText("Milk is out of the fridge");
    }
  
  else if (Tm = Tr) // if milk temp = room temp
    {
    // display this message when the milk reaches room temperature (bacteria now thrives)
    Label1->setText("Warning: milk is at room temperature!");
    
    //start another timer for the next message (ie when t = 120)
    }

  else if (timeActual = 120)
    {
    // 
    // display this message when the milk left out for longer than two hours
    Label1->setText("Highly recommend that you don't drink this milk now");
    // fire off a notification to the perl script to then send off to the app
    // SOME CODE
    }
 
      
	// add the new input to the plot
	memmove( yData, yData+1, (plotDataSize-1) * sizeof(double) );
	yData[plotDataSize-1] = Tm;
	curve->setSamples(xData, yData, plotDataSize);
	
	memmove( y1Data, y1Data+1, (plotDataSize-1) * sizeof(double) );
	y1Data[plotDataSize-1] = Tf;
	curve1->setSamples(xData, y1Data, plotDataSize);
       
	memmove( y2Data, y2Data+1, (plotDataSize-1) * sizeof(double) );
	y2Data[plotDataSize-1] = Tr;
	curve2->setSamples(xData, y2Data, plotDataSize);
	
	plot->setAxisTitle(QwtPlot::xBottom, "Time (x10 ms)");
    	plot->setAxisTitle(QwtPlot::yLeft, "Dryness (%)");
	plot->replot();

}

//functions defining the 4 (2x2) thresholds

double Window::setLowerFahrThreshold()
{
  Tf = 39.2; // the fridge temperature in fahrenheit
  return Tf;
}

double Window::setHigherFahrThreshold()
{
 Tr = 64.4; // the room temperature in fahrenheit
  return Tr;
}

int Window::setLowerCelsThreshold()
{
  Tf = 4; // fridge temp is celsius
  return Tf;
}

int Window::setHigherCelsThreshold()
{
  Tr = 18; // room temp in celsius
  return Tr;
}


