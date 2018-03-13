#include <window.h>
#include <QApplication>

int main(int argc, char *argv[])
{
        QApplication app(argc, argv);
        // create the window
        Window window;
        window.showMaximized();
        // call the window.timerEvent function every 50ms
        window.startTimer(50);
        // execute the application
        return app.exec();
}

//note to self: to inherit from multiple classes:

// class a: public b, public c