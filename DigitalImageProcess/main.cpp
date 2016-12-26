#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "MainWindow1.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
	MainWindow1 q;
	QObject::connect(&w, SIGNAL(w.showMainWindow1()), &q, SLOT(q.showWindow()));
	QObject::connect(&q, SIGNAL(q.showMainWindow()), &w, SLOT(w.showWindow()));
    w.show();
    return a.exec();
}
