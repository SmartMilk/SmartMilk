#include <window.h>
#include <QApplication>
#include <unistd.h>
#include <tempread.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	//create splash screen
	QPixmap pixmap ("./pics/logo.jpg");
	QSplashScreen splash (pixmap);
	splash.show();
	app.processEvents();
	usleep(3000000);
    // create the window
    Window window;
    window.showMaximized();
	//close splash screen
	splash.finish(&window);

//		QThread thread;
//		QObject::connect(&thread, SIGNAL(started()), &window, SLOT(plotUpdate()));
//		QObject::connect(&thread, SIGNAL(finished()), &app, SLOT(quit()));


//		window.moveToThread(&thread);
//		thread.start();

        // call the window.timerEvent function every _ms
    window.startTimer(50);
        // execute the application
    return app.exec();
}
