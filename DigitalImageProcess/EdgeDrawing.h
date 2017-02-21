#pragma once

#include<iostream>
#include<cmath>

#include "opencv\cv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
using namespace cv;

class EdgeDrawing
{
	EdgeDrawing();
	~EdgeDrawing();

public:
	//G 梯度图，D 方向图，E 黑白图像
	Mat G, D, E;
	int GradientThreshold, AnchorThreshold;//梯度阈值和锚点选择阈值
	int Horizon, Vertical, Color,AnchorColor;//水平，垂直，联线颜色

	//初始化参数
	void SetGradientThreshold(int gradientThreshold);
	void SetAnchorThreshold(int anchorThreshold);
	void SetE(Mat image);
	void SetHorizon(int horizon);
	void SetVertical(int vertical);
	void SetColor(int color);

	//工具方法
	void GenerateGradientAndDirection(Mat image);//计算G和D
	void GenerateAnchor();//计算锚点
	bool IsAnchor(int x, int y);//是不是锚点，是返回true
	void LinkAnchor();
	void GoLeft(int x, int y);
	void GoRight(int x, int y);
	void GoUp(int x, int y);
	void GoDown(int x,int y);
	int static GetPoint(int x, int y,Mat image);
	void static SetPoint(int x, int y, Mat image,int value);
};