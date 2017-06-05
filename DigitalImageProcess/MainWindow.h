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
	void showImage(cv::Mat& image);
	void setImage(cv::Mat image);
	void pretreatmentImage(cv::Mat &sourceImage, cv::Mat &treatmentImage);
	double distance(double x, double y);
	void showCompatibility(QString str);

private://����˽�б���
	Circles circles;
	Ui::MainWindowClass ui;
	cv::Mat image, circleImage, lineImage,displayImage;//display������Ϊ���ɣ�������qtչʾ�У������image���ᵼ��ԭͼ���޸�
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

signals://�����źź���
	void showMainWindow1();
};

