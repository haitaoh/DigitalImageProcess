#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "MainWindow1.h"

cv::Mat image, imageShow;

/*char *windowName = "hehe";
char *trackbarMinDist = "min_dist";
char *trackbarCannyThresholdLow = "CannyThresholdLow";
char *trackbarCannyThresholdHigh = "CannyThresholdHigh";
char *trackbarAccThreshold = "param2_AccThreshold";
char *trackbarMinRadius = "min_radius";
char *trackbarMaxRadius = "max_radius";
int min_dist = 1,max_min_dist = 100;
int canny_threshold_low = 15,max_canny_threshold_low = 300;
int canny_threshold_high = 44, max_canny_threshold_high = 300;
int param2 = 115,max_param2 = 400;
int min_radius = 68, max_min_radius = 1000;
int max_radius = 189, max_max_radius = 1000;
void HoughCircles(int,void*);*/
/*
 * 外圈：1 15 44 115 70 189
 * 内圈：1 15 44 122 177 189
 * 底面：1 15 44 74 74 177
 */
 /*int canny_threshold_low = 15, max_canny_threshold_low = 300;
 int canny_threshold_high = 44, max_canny_threshold_high = 300;
 char *windowName = "hehe";
 char *CannyThresholdLow = "CannyThresholdLow";
 char *CannyThresholdHigh = "CannyThresholdHigh";
 void Canny(int, void*);*/

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	MainWindow1 q;
	//界面切换信号、槽函数
	QObject::connect(&w, SIGNAL(showMainWindow1()), &q, SLOT(showWindow()));
	QObject::connect(&q, SIGNAL(showMainWindow()), &w, SLOT(showWindow()));
	image = cv::imread("0.jpg");
	w.setImage(image);
	w.init();//初始化circleImage和lineImage
	w.showImage(image);
	w.show();

	/*cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	cv::createTrackbar(trackbarMinDist, windowName, &min_dist, max_min_dist, HoughCircles);
	cv::createTrackbar(trackbarCannyThresholdLow, windowName, &canny_threshold_low, max_canny_threshold_low, HoughCircles);
	cv::createTrackbar(trackbarCannyThresholdHigh, windowName, &canny_threshold_high, max_canny_threshold_high, HoughCircles);
	cv::createTrackbar(trackbarAccThreshold, windowName, &param2, max_param2, HoughCircles);
	cv::createTrackbar(trackbarMinRadius, windowName, &min_radius, max_min_radius, HoughCircles);
	cv::createTrackbar(trackbarMaxRadius, windowName, &max_radius, max_max_radius, HoughCircles);
	HoughCircles(0,0);*/
	/*cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	cv::createTrackbar(CannyThresholdLow, windowName, &canny_threshold_low, max_canny_threshold_low, Canny);
	cv::createTrackbar(CannyThresholdHigh, windowName, &canny_threshold_high, max_canny_threshold_high, Canny);
	Canny(1, 0);*/

	return a.exec();
}

/*void Canny(int,void*)
{
	cv::cvtColor(image, imageShow, CV_BGR2GRAY);//转换成灰度图
	cv::GaussianBlur(imageShow, imageShow, cv::Size(5, 5), 1, 1);//模糊降噪
	cv::Canny(imageShow, imageShow, canny_threshold_low, canny_threshold_high);
	cv::imshow(windowName, imageShow);
}*/

/*void HoughCircles(int,void*)
{
	cv::cvtColor(image, imageShow, CV_BGR2GRAY);//转换成灰度图
	cv::GaussianBlur(imageShow, imageShow, cv::Size(5, 5), 1, 1);//高斯模糊，降噪处理
	cv::vector<cv::Vec3f> circles;
	myHough::HoughCircles(imageShow, circles, CV_HOUGH_GRADIENT, 1, min_dist,
		canny_threshold_low,canny_threshold_high, param2,min_radius,max_radius);
	cv::Scalar centerScalar(237, 62, 62), radiusScalar(0, 0, 255);
	int centerRadius = 3;
	for (int i = 0; i < circles.size(); i++)
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		cv::circle(imageShow, center, centerRadius, centerScalar, -1, 8, 0);//圆心
		cv::circle(imageShow, center, radius, radiusScalar, 1, 8, 0);//圆边
	}
	cv::imshow("hehe", imageShow);
}*/