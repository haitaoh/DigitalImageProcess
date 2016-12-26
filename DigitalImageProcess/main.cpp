/*
#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "MainWindow1.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
	MainWindow1 q;
	QObject::connect(&w, SIGNAL(showMainWindow1()), &q, SLOT(showWindow()));
	QObject::connect(&q, SIGNAL(showMainWindow()), &w, SLOT(showWindow()));
    w.show();
    return a.exec();
}
*/

#include <iostream>    
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>    
#include <opencv2/highgui/highgui.hpp>    
using namespace cv;
using namespace std;

int Otsu(Mat src)
{
	int height = src.rows;
	int width = src.cols;


	//histogram          
	float histogram[256] = { 0 };
	for (int i = 0; i < height; i++)
	{
		unsigned char* p = (unsigned char*)src.ptr<uchar>(i);
		for (int j = 0; j < width; j++)
		{
			histogram[*p++]++;
		}
	}
	//normalize histogram          
	int size = height * width;
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = histogram[i] / size;
	}


	//average pixel value          
	float avgValue = 0;
	for (int i = 0; i < 256; i++)
	{
		avgValue += i * histogram[i];  //整幅图像的平均灰度        
	}


	int threshold;
	float maxVariance = 0;
	float w = 0, u = 0;
	for (int i = 0; i < 256; i++)
	{
		w += histogram[i];  //假设当前灰度i为阈值, 0~i 灰度的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例        
		u += i * histogram[i];  // 灰度i 之前的像素(0~i)的平均灰度值： 前景像素的平均灰度值        


		float t = avgValue * w - u;
		float variance = t * t / (w * (1 - w));
		if (variance > maxVariance)
		{
			maxVariance = variance;
			threshold = i;
		}
	}
	return threshold;
}

int Otsu2(Mat src)
{
	int height = src.rows;
	int width = src.cols;
	int x = 0, y = 0;
	int pixelCount[256];
	float pixelPro[256];
	int i, j, pixelSum = width * height, threshold = 0;
	//初始化  
	for (i = 0; i < 256; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}
	//统计灰度级中每个像素在整幅图像中的个数  
	for (i = y; i < height; i++)
	{
		for (j = x; j <width; j++)
		{
			pixelCount[src.at<uchar>(i, j)]++;
		}
	}
	//计算每个像素在整幅图像中的比例  
	for (i = 0; i < 256; i++)
	{
		pixelPro[i] = (float)(pixelCount[i]) / (float)(pixelSum);
	}
	//经典ostu算法,得到前景和背景的分割  
	//遍历灰度级[0,255],计算出方差最大的灰度值,为最佳阈值  
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < 256; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;

		for (j = 0; j < 256; j++)
		{
			if (j <= i) //背景部分  
			{
				//以i为阈值分类，第一类总的概率  
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else       //前景部分  
			{
				//以i为阈值分类，第二类总的概率  
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;        //第一类的平均灰度  
		u1 = u1tmp / w1;        //第二类的平均灰度  
		u = u0tmp + u1tmp;      //整幅图像的平均灰度  
								//计算类间方差  
		deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);
		//找出最大类间方差以及对应的阈值  
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}
	//返回最佳阈值;  
	return threshold;
}

int main()
{
	Mat q_MatImage;
	Mat q_MatImageGray;
	Mat q_MatImageShow;
	Mat q_MatImageShow2;
	q_MatImage = imread("3.jpg");//读入一张图片  
	q_MatImage.copyTo(q_MatImageShow);
	q_MatImage.copyTo(q_MatImageShow2);
	cvtColor(q_MatImage, q_MatImageGray, CV_RGB2GRAY);
	double q_dEpsilon = 10E-9;
	unsigned int q_iReturn = 0;

	int q_iX, q_iY, q_iWidth, q_iHeight;
	q_iX = 20;
	q_iY = 40;
	q_iWidth = 600;
	q_iHeight = 420;

	double q_dThresholdSimilarity = 60;
	double q_dThresholdMin = 35;
	double q_dThresholdMax = 75;

	//      Rect q_RectROI = Rect(q_iX,q_iY,q_iWidth,q_iHeight);  
	//      Mat q_MatROI = q_MatImageGray(q_RectROI);  
	//    
	//      threshold(q_MatROI, q_MatROI, 150, 255, CV_THRESH_BINARY);
	int thresh = Otsu(q_MatImageGray);
	threshold(q_MatImageGray, q_MatImageGray, thresh, 255, CV_THRESH_BINARY);

	for (int i = 0; i < q_MatImageGray.rows; i++)
	{
		for (int j = 0; j < q_MatImageGray.cols; j++)
		{
			q_MatImageGray.at<uchar>(i, j) = 255 - q_MatImageGray.at<uchar>(i, j);
		}
	}

	namedWindow("Test1");       //创建一个名为Test窗口  
	imshow("Test1", q_MatImageGray);         //窗口中显示图像  

	vector<vector<Point>> q_vPointContours;

	//findContours(q_MatROI, q_vPointContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE,Point(q_iX,q_iY));  
	findContours(q_MatImageGray, q_vPointContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

	size_t q_iAmountContours = q_vPointContours.size();

	for (size_t i = 0; i < q_iAmountContours; i++)
	{
		size_t q_perNum = q_vPointContours[i].size();
		for (size_t j = 0; j < q_iAmountContours; j++)
		{
			circle(q_MatImageGray, q_vPointContours[i][j], 3, CV_RGB(0, 255, 0), 1, 8, 3);
		}
	}

	namedWindow("findContours");
	imshow("findContours", q_MatImageGray);

	std::vector<cv::Point2f> q_vPointCentersContours(q_iAmountContours);
	std::vector<double> q_vdRadiusesContours(q_iAmountContours);
	std::vector<double> q_vdSimilarityContours(q_iAmountContours);
	std::vector<bool> q_vbFlagCircles(q_iAmountContours);

	std::vector<double> q_vdRadiusesContour;
	double q_dRadiusMax, q_dRadiusMin;
	double q_dSumX, q_dSumY;
	size_t q_iAmountPoints;

	for (size_t q_iCycleContours = 0; q_iCycleContours<q_iAmountContours; q_iCycleContours++)
	{
		q_dSumX = 0.0;
		q_dSumY = 0.0;
		q_iAmountPoints = q_vPointContours[q_iCycleContours].size();
		if (0 >= q_iAmountPoints)
		{
			continue;
		}
		for (size_t q_iCyclePoints = 0; q_iCyclePoints<q_iAmountPoints; q_iCyclePoints++)
		{
			q_dSumX += q_vPointContours[q_iCycleContours].at(q_iCyclePoints).x;
			q_dSumY += q_vPointContours[q_iCycleContours].at(q_iCyclePoints).y;
		}

		q_vPointCentersContours[q_iCycleContours].x = (float)(q_dSumX / q_iAmountPoints);//均值中心点X</span>  
		q_vPointCentersContours[q_iCycleContours].y = (float)(q_dSumY / q_iAmountPoints);//均值中心点Y</span>  


		q_vdRadiusesContour.resize(q_iAmountPoints);
		double q_dDifferenceX, q_dDifferenceY;
		double q_dSumRadius = 0.0;
		q_dRadiusMax = 0.0;
		q_dRadiusMin = DBL_MAX;;
		for (size_t q_iCyclePoints = 0; q_iCyclePoints<q_iAmountPoints; q_iCyclePoints++)
		{
			q_dDifferenceX = q_vPointCentersContours[q_iCycleContours].x - q_vPointContours[q_iCycleContours].at(q_iCyclePoints).x;
			q_dDifferenceY = q_vPointCentersContours[q_iCycleContours].y - q_vPointContours[q_iCycleContours].at(q_iCyclePoints).y;
			q_vdRadiusesContour[q_iCyclePoints] = sqrt(q_dDifferenceX*q_dDifferenceX + q_dDifferenceY*q_dDifferenceY);

			if (q_vdRadiusesContour[q_iCyclePoints]>q_dRadiusMax)
			{
				q_dRadiusMax = q_vdRadiusesContour[q_iCyclePoints];
			}
			if (q_vdRadiusesContour[q_iCyclePoints]<q_dRadiusMin)
			{
				q_dRadiusMin = q_vdRadiusesContour[q_iCyclePoints];
			}

			q_dSumRadius += q_vdRadiusesContour[q_iCyclePoints];
		}
		q_vdRadiusesContours[q_iCycleContours] = q_dSumRadius / q_iAmountPoints;   //均值半径  

		q_vdSimilarityContours[q_iCycleContours] = 100.0*q_dRadiusMin / q_dRadiusMax;  //相似度  
		if ((q_dThresholdSimilarity<q_vdSimilarityContours[q_iCycleContours]) &&
			(q_dThresholdMin<q_vdRadiusesContours[q_iCycleContours]) &&
			(q_dThresholdMax>q_vdRadiusesContours[q_iCycleContours]))    //判断是否是圆  
		{
			q_vbFlagCircles[q_iCycleContours] = true;
		}
		else
		{
			q_vbFlagCircles[q_iCycleContours] = false;
		}
	}


	if (q_dEpsilon < 10)
	{
		cv::Point q_PointCenterCurrent;
		for (size_t q_iCycleContours = 0; q_iCycleContours<q_iAmountContours; q_iCycleContours++)
		{
			if (q_vbFlagCircles[q_iCycleContours])
			{
				q_PointCenterCurrent.x = (int)(q_vPointCentersContours[q_iCycleContours].x);
				q_PointCenterCurrent.y = (int)(q_vPointCentersContours[q_iCycleContours].y);
				circle(q_MatImageShow, q_PointCenterCurrent, 3, Scalar(0.0, 0.0, 255.0), 0);
			}
		}
	}

	int q_iIndexResultBegin = 4;
	int q_iAmountCircleResult = 4;
	int q_iIndexCiecleCurrent;

	int q_iCountCircles = 0;

	for (size_t q_iCycleContours = 0; q_iCycleContours<q_iAmountContours; q_iCycleContours++)
	{
		if (q_vbFlagCircles[q_iCycleContours])
		{
			q_iIndexCiecleCurrent = q_iIndexResultBegin + q_iAmountCircleResult*q_iCountCircles;
			//          match_result[q_iIndexCiecleCurrent]=(float)(q_vdSimilarityContours[q_iCycleContours]);  
			//          match_result[q_iIndexCiecleCurrent+1]=(float)(q_vdRadiusesContours[q_iCycleContours]);  
			//          match_result[q_iIndexCiecleCurrent+2]=(float)(q_vPointCentersContours[q_iCycleContours].x);  
			//          match_result[q_iIndexCiecleCurrent+3]=(float)(q_vPointCentersContours[q_iCycleContours].y);  
			q_iCountCircles++;
		}
	}
	cout << "总共找到 " << q_iCountCircles << "个圆！" << endl;

	namedWindow("Test");        //创建一个名为Test窗口  
	imshow("Test", q_MatImageShow);//窗口中显示图像  
	waitKey();              //等待5000ms后窗口自动关闭  
}