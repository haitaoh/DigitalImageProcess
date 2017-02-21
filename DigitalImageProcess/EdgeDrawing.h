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
	//G �ݶ�ͼ��D ����ͼ��E �ڰ�ͼ��
	Mat G, D, E;
	int GradientThreshold, AnchorThreshold;//�ݶ���ֵ��ê��ѡ����ֵ
	int Horizon, Vertical, Color,AnchorColor;//ˮƽ����ֱ��������ɫ

	//��ʼ������
	void SetGradientThreshold(int gradientThreshold);
	void SetAnchorThreshold(int anchorThreshold);
	void SetE(Mat image);
	void SetHorizon(int horizon);
	void SetVertical(int vertical);
	void SetColor(int color);

	//���߷���
	void GenerateGradientAndDirection(Mat image);//����G��D
	void GenerateAnchor();//����ê��
	bool IsAnchor(int x, int y);//�ǲ���ê�㣬�Ƿ���true
	void LinkAnchor();
	void GoLeft(int x, int y);
	void GoRight(int x, int y);
	void GoUp(int x, int y);
	void GoDown(int x,int y);
	int static GetPoint(int x, int y,Mat image);
	void static SetPoint(int x, int y, Mat image,int value);
};