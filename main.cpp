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
	
    return app.exec();
}
