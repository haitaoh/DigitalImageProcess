#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "MainWindow1.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
	MainWindow1 q;
	QObject::connect(&w, SIGNAL(showMainWindow1()), &q, SLOT(showWindow()));
	QObject::connect(&q, SIGNAL(showMainWindow()), &w, SLOT(showWindow()));
    w.show();
    return a.exec();
}
