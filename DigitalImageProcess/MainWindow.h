#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "qlabel.h"

#include "opencv\cv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include<time.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = Q_NULLPTR);
	~MainWindow();
	std::string longToString(long l);
	cv::vector<cv::Vec3f> houghCircles(cv::Mat& image);
	void findContours(cv::Mat& image);
	void showImage(cv::Mat& image);

private://����˽�б���
	Ui::MainWindowClass ui;
	cv::Mat image,circleImage,lineImage;
	QString imageFilePath;
	cv::Mat result;
	QLabel *label;

private slots://�����ۺ���
	void openImage();
	void saveImage();
	void showWindow();
	void positiveButtonClicked();
	void negativeButtonClicked();
	void checkBox1(int state);
	void checkBox2(int state);
	
signals://�����źź���
	void showMainWindow1();
};
