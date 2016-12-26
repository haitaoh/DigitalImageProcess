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
	void houghCircles(cv::Mat& image);
	void findContours(cv::Mat& image);

private://申明私有变量
	Ui::MainWindowClass ui;
	cv::Mat image;
	QString imageFilePath;
	cv::Mat result;

private slots://申明槽函数
	void openImage();
	void saveImage();
	void showWindow();
	void positiveButtonClicked();
	void negativeButtonClicked();
	
signals://申明信号函数
	void showMainWindow1();
};
