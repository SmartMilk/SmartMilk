# SmartMilk: an automated milk monitoring system
 
SmartMilk is a simple temperature dependent system for monitoring the condition of your milk. When milk has been left out of the fridge, SmartMilk will notify the user at predefined time intervals to prevent the milk from spoiling over time.
This repository contains the software, and associated instructions, required to implement the SmartMilk application for your own use. 

For a detailed description of the project, and information on the hardware, libraries and packages required for getting it working, please refer to our Wiki. Installation instructions are provided below.

SmartMilk was developed as part of the Real Time Embedded Programming module at the University of Glasgow.

# Making it work
To clone this git repository: 

```
git clone https://github.com/SmartMilk/SmartMilk.git
```

To build:

```
cd SmartMilk
qmake
make
```

To run(assuming you are logged into the RPi over ssh and no X-server is running):

```
startx ./SmartMilk
```

or with X running:

```
./SmartMilk
`
