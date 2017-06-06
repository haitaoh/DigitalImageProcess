#pragma once

#include<iostream>
#include<cmath>

#include "opencv\cv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;

typedef struct circleContent {
	double x;//圆心坐标
	double y;
	double r;//半径
	double compatibility;//找到圆的契合度
}circleContent;

typedef struct ellipseContent {
	double x;//圆心坐标
	double y;
	double a;//a轴
	double b;//b轴
	double xtheta;//相对于x轴的夹角
	double compatibility;//找到椭圆的契合度
}ellipseContent;

typedef struct circleArcContent {
	circleContent circ;
	vector<Point> points;
	bool flag;//记录是否使用过，默认为false
}circleArcContent;

typedef struct ellipseArcContent {
	ellipseContent elli;
	vector<Point> points;
	bool flag;//记录是否使用过，默认为false
}ellipseArcContent;

class Circles
{
public:
	int cannyLowThreshold = 14,//canny低阈值
		cannyHighThreshold = 40,//canny高阈值
		circleNumberThreshold = 150,//用于过滤圆弧和圆的点数大小
		ellipseNumberThreshold = 150,//用于过滤椭圆弧和椭圆的点数大小
		spotNumberThreshold = 30,//用于过滤污点的点数大小
		minThreshold = 30;//最小阈值
	Mat src,//输入图像
		img;//做预处理图像
	vector<vector<Point>> contour, circleContour, ellipseContour, spotContour,
		circleArcContour, ellipseArcContour;
	vector<vector<Point>>	circleArcCandidateContour,//圆弧候选轮廓
		ellipseArcCandidateContour;//椭圆弧候选轮廓
	vector<circleContent> circ;//存入圆信息
	vector<ellipseContent> elli;//存入椭圆信息
	vector<ellipseArcContent> ellipseArcs;//椭圆弧信息
	vector<circleArcContent> circleArcs;//圆弧信息

	Circles();
	Circles(Mat img);
	~Circles();

	void init(Mat img);//初始化，图像转换
	void getContours();//获取contour

	circleContent circleLeastFit(const vector<Point> points);//最小二乘拟合圆
	float computeVariance(const vector<Point> points, const circleContent circ);//计算均方差
	float computeDistance(const Point point, const circleContent circ);//计算点到圆心的距离

	ellipseContent ellipseLeastFit(const vector<Point> points);//奇异值分解 最小二乘拟合椭圆
	int SVD(float *a, int m, int n, float b[], float x[], float esp);
	int gmiv(float a[], int m, int n, float b[], float x[], float aa[], float eps, float u[], float v[], int ka);
	int gniv(float a[], int m, int n, float aa[], float eps, float u[], float v[], int ka);
	int muav(float a[], int m, int n, float u[], float v[], float eps, int ka);
	float computeEllipseVariance(const vector<Point> points, const ellipseContent elli);//计算均方差

	/**通过判断是不是椭圆来判断**/
	void getCircle();//获取圆
	void getEllipse();//获取椭圆
	void getSpot();//获取污点
	bool hasRepeat(const vector<Point> points, const vector<vector<Point>> contour);//判断轮廓是否已经被排除

	/**绘制图形方法**/
	void drawCircle(Mat &img);//绘制圆
	void drawEllipse(Mat &img);//绘制椭圆
	void drawSpot(Mat &img);//绘制污点

	/**通过圆弧和椭圆弧找圆和椭圆**/
	void getCircleFromArc();
	void getEllipseFromArc();

};