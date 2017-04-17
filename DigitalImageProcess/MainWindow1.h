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

class MainWindow1 : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow1(QWidget* parent = Q_NULLPTR);
	~MainWindow1();
	void init();
	void showImage(cv::Mat& image);
	void setImage(cv::Mat image);
	void pretreatmentImage(cv::Mat &sourceImage, cv::Mat &treatmentImage);
	std::string doubleToString(double i);
	double distance(double x, double y);

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

signals://申明信号函数
	void showMainWindow();
};
