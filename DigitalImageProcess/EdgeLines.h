#pragma once

#include<iostream>

#include "opencv\cv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "EdgeDrawing.h"

struct Line
{
	Vector<Point> line;
};

class EdgeLines : EdgeDrawing
{
	EdgeLines();
	~EdgeLines();

public:


	static bool isNearby(Point point,Line line);//判断点是否距离线接近
	static void getLine(Line* lines,int noLine);//递归实现获取直线
};