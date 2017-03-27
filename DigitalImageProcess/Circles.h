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
	Mat src, img;
	vector<vector<Point>> contour,circleContour;

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
	void ppp(float a[],float e[],float s[],float v[],int m,int n);
	void sss(float fg[], float cs[]);


};