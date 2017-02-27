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


	static bool isNearby(Point point,Line line);//�жϵ��Ƿ�����߽ӽ�
	static void getLine(Line* lines,int noLine);//�ݹ�ʵ�ֻ�ȡֱ��
};