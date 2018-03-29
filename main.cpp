#include <window.h>
#include <QApplication>

int main(int argc, char *argv[])
{
        QApplication app(argc, argv);
        // create the window
        Window window;
        window.showMaximized();

		QThread thread;
		QObject::connect(&thread, SIGNAL(started()), &window, SLOT(plotUpdate()));
		QObject::connect(&thread, SIGNAL(finished()), &a, SLOT(quit()));

		window.moveToThread(&thread);
		thread.start();

        // call the window.timerEvent function every _ms
       // window.startTimer(10);
        // execute the application
        return app.exec();
}
