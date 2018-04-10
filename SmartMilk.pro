QT += widgets
TEMPLATE = app
QT       += core gui
CONFIG          += qt warn_on debug
TARGET = SmartMilkv6
greaterThan(QT_MAJOR_VERSION, 4): LIBS += -lqwt-qt5 -lm
lessThan(QT_MAJOR_VERSION, 5): LIBS += -lqwt -lm

SOURCES += window.cpp
SOURCES += main.cpp
SOURCES += tempread.cpp
HEADERS += window.h
HEADERS += tempread.h
