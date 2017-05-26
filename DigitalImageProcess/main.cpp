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
	Mat image = cv::imread("0.jpg");
	cvtColor(image, image, CV_BGR2GRAY);
	for (int i = 0; i < 10; i++) {
		GaussianBlur(image, image, Size(5, 5), 1, 1);
		imshow("123456789"+i, image);
	}
	/*w.setImage(image);
	w.showImage(image);
	w.show();*/

	return a.exec();
}
