#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "qlabel.h"
#include "qcheckbox.h"

#include "opencv\cv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include<time.h>
#include"Circles.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = Q_NULLPTR);
	~MainWindow();
	void init();
	std::string longToString(long l);
	void houghLines(cv::Mat &image, cv::vector<cv::Vec4i> &lines);
	void houghCircles(cv::Mat& image, cv::Mat &img, cv::vector<cv::Vec3f> &circles,
		int dp, int min_dist, int canny_threshold_low, int canny_threshold_high,
		int acc_threshold, int min_radius, int max_radius);
	void findContours(cv::Mat& image, cv::vector<cv::vector<cv::Point>> &contours);
	void showImage(cv::Mat& image);
	void setImage(cv::Mat image);
	void pretreatmentImage(cv::Mat &sourceImage, cv::Mat &treatmentImage);
	void doFindPositiveCircles(cv::Mat &image);
	void doFindPositiveLine(cv::Mat &image);

private://申明私有变量
	Circles circles;
	Ui::MainWindowClass ui;
	cv::Mat image, circleImage, lineImage;
	QString imageFilePath;
	cv::Mat result;
	QLabel *label;


	private slots://申明槽函数
	void openImage();
	void saveImage();
	void showWindow();
	void positiveButtonClicked();
	void negativeButtonClicked();
	void checkBox1(int state);
	void checkBox2(int state);

signals://申明信号函数
	void showMainWindow1();
};

namespace myHough {
	void HoughCircles(cv::InputArray _image, cv::OutputArray _circles,
		int method, double dp, double min_dist,
		double canny_threshold_low, double canny_threshold_high, double acc_threshold,
		int minRadius = 0, int maxRadius = 0);
}
