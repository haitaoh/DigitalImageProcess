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
	int circleNumberThreshold = 50;
	Mat src, img;
	vector<vector<Point>> contour, circleContour, ellipseContour;
	vector<circleContent> circ;
	vector<ellipseContent> elli;

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
	float computeEllipseVariance(const vector<Point> points, const ellipseContent elli);//计算均方差

	/**通过判断是不是椭圆来判断**/
	void getCircle();//获取圆
	bool hasRepeat(const vector<Point> points, const vector<vector<Point>> contour);//判断轮廓是否已经被排除
	void getEllipse();//获取椭圆
};