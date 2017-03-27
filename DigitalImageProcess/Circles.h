#pragma once

#include<iostream>
#include<cmath>

#include "opencv\cv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;

typedef struct circleContent{
	double x;//圆心坐标
	double y;
	double r;//半径
}circleContent;

typedef struct ellipseContent{
	double x;//圆心坐标
	double y;
	double a;//a轴
	double b;//b轴
	double xtheta;//相对于x轴的夹角
}ellipseContent;

class Circles
{
public:
	Mat src, img;
	vector<vector<Point>> contour,circleContour;

	Circles();
	Circles(Mat img);
	~Circles();

	void init(Mat img);//初始化，图像转换
	void getContours();//获取contour
	
	circleContent circleLeastFit(const vector<Point> points);//最小二乘拟合圆
	float computeVariance(const vector<Point> points, const circleContent circ);//计算均方差
	float computeDistance(const Point point,const circleContent circ);//计算点到圆心的距离

	ellipseContent ellipseLeastFit(const vector<Point> points);//奇异值分解 最小二乘拟合椭圆
	int SVD(float *a, int m, int n, float b[], float x[], float esp);
	int gmiv(float a[], int m, int n, float b[], float x[], float aa[], float eps, float u[], float v[], int ka);
	int gniv(float a[], int m, int n, float aa[], float eps, float u[], float v[], int ka);
	int muav(float a[], int m, int n, float u[], float v[], float eps, int ka);
	void ppp(float a[],float e[],float s[],float v[],int m,int n);
	void sss(float fg[], float cs[]);


};