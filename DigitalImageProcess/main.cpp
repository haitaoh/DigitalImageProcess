#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "MainWindow1.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	MainWindow1 q;
	//�����л��źš��ۺ���
	QObject::connect(&w, SIGNAL(showMainWindow1()), &q, SLOT(showWindow()));
	QObject::connect(&q, SIGNAL(showMainWindow()), &w, SLOT(showWindow()));
	Mat image = cv::imread("0.jpg");
	w.setImage(image);
	w.showImage(image);
	w.show();

	return a.exec();
}
