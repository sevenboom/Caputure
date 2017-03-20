#include "ScreenCapture.h"

#include <QtWidgets/QApplication>
#include <QScreen>
#include <QWindow>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
//	a.setWindowIcon(QIcon(":/ScreenCapture/APPICON.ico"));
    //


    QScreen *screen = QGuiApplication::primaryScreen();

    ScreenCapture w;
    if (screen){
        w.setTarget(screen->grabWindow(0));
    }
    w.show();
	return a.exec();
}
