#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "MainWindow1.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
	MainWindow1 q;
	//界面切换信号、槽函数
	QObject::connect(&w, SIGNAL(showMainWindow1()), &q, SLOT(showWindow()));
	QObject::connect(&q, SIGNAL(showMainWindow()), &w, SLOT(showWindow()));
	cv::Mat image = cv::imread("4.jpg");
	w.showImage(image);
	w.show();
    return a.exec();
}