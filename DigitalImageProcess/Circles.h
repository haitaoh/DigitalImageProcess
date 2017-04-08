#pragma once

#include<iostream>
#include<cmath>

#include "opencv\cv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;

typedef struct circleContent{
	double x;//Բ������
	double y;
	double r;//�뾶
}circleContent;

typedef struct ellipseContent{
	double x;//Բ������
	double y;
	double a;//a��
	double b;//b��
	double xtheta;//�����x��ļн�
}ellipseContent;

class Circles
{
public:
	int circleNumberThreshold = 150,
		ellipseNumberThreshold = 150,
		spotNumberThreshold = 30;
	Mat src,//����ͼ��
		img;//��Ԥ����ͼ��
	vector<vector<Point>> contour, circleContour, ellipseContour,spotContour;
	vector<circleContent> circ;
	vector<ellipseContent> elli;

	Circles();
	Circles(Mat img);
	~Circles();

	void init(Mat img);//��ʼ����ͼ��ת��
	void getContours();//��ȡcontour
	
	circleContent circleLeastFit(const vector<Point> points);//��С�������Բ
	float computeVariance(const vector<Point> points, const circleContent circ);//���������
	float computeDistance(const Point point,const circleContent circ);//����㵽Բ�ĵľ���

	ellipseContent ellipseLeastFit(const vector<Point> points);//����ֵ�ֽ� ��С���������Բ
	int SVD(float *a, int m, int n, float b[], float x[], float esp);
	int gmiv(float a[], int m, int n, float b[], float x[], float aa[], float eps, float u[], float v[], int ka);
	int gniv(float a[], int m, int n, float aa[], float eps, float u[], float v[], int ka);
	int muav(float a[], int m, int n, float u[], float v[], float eps, int ka);
	float computeEllipseVariance(const vector<Point> points, const ellipseContent elli);//���������

	/**ͨ���ж��ǲ�����Բ���ж�**/
	void getCircle();//��ȡԲ
	void getEllipse();//��ȡ��Բ
	void getSpot();//��ȡ�۵�
	bool hasRepeat(const vector<Point> points, const vector<vector<Point>> contour);//�ж������Ƿ��Ѿ����ų�

	/**����ͼ�η���**/
	void drawCircle(Mat &img);//����Բ
	void drawEllipse(Mat &img);//������Բ
	void drawSpot(Mat &img);//�����۵�
};