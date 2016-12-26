#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow1.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "qlabel.h"

#include "opencv\cv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include<time.h>

class MainWindow1 : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow1(QWidget* parent = Q_NULLPTR);
	~MainWindow1();
	std::string longToString(long l);
	void houghCircles(cv::Mat& image);
	void findContours(cv::Mat& image);

private://����˽�б���
	Ui::mainWindow ui;
	cv::Mat image;
	QString imageFilePath;
	cv::Mat result;

private slots://�����ۺ���
	void openImage();
	void saveImage();
	void showWindow();
	void positiveButtonClicked();
	void negativeButtonClicked();

signals://�����źź���
	void showMainWindow();
};
