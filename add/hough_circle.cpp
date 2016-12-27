#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>
using namespace cv;  
using namespace std;//注意！！
int main( )  
{  
    Mat srcImage = imread("F:\\code\\DigitalImageProcess\\DigitalImageProcess\\3.jpg");  
	imshow("【原始图】", srcImage);
	int rows=srcImage.rows,cols=srcImage.cols;

	Mat grayImage,binaryImage;
	cvtColor(srcImage,grayImage, CV_BGR2GRAY);
	threshold(grayImage,binaryImage,200,255,THRESH_BINARY);

	Mat temp,outImage;
	binaryImage.copyTo(temp);
	binaryImage.copyTo(outImage);

	//Temp是一张全白的图像
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols;j++)
			temp.at<unsigned char>(i,j)=0;

	temp.copyTo(outImage);
	
    cvtColor(srcImage,grayImage, CV_BGR2GRAY);
	threshold(grayImage,binaryImage,200,255,THRESH_BINARY);
	imshow("【二值图像】",binaryImage);
	Canny(binaryImage,binaryImage,100,200,3);
	imshow("【边缘图像】",binaryImage);
	
    //【4】进行霍夫圆变换  
    vector<Vec3f> circles0;
	vector<Vec3f> circles1;
	HoughCircles(binaryImage, circles0, CV_HOUGH_GRADIENT,3, 10, 200, 250, 10, 200 ); 
	HoughCircles(binaryImage, circles1, CV_HOUGH_GRADIENT,2.9, 300, 200, 120, 10, 100 ); 
  
    //【5】依次在图中绘制出圆  
    for( size_t i = 0; i < circles1.size(); i++ )  
    {  
        Point center(cvRound(circles1[i][0]), cvRound(circles1[i][1]));  
        int radius = cvRound(circles1[i][2]);   
        circle( temp, center, radius, Scalar(255,255,255), 0.5, 8, 0 );  
    }  
	for( size_t i = 0; i < circles0.size(); i++ )  
	{  
		Point center(cvRound(circles0[i][0]), cvRound(circles0[i][1]));  
		int radius = cvRound(circles0[i][2]);   
		circle( temp, center, radius, Scalar(255,255,255), 0.5, 8, 0 );  
	}  
	threshold(temp,temp,200,255,THRESH_BINARY);
	imshow("【初次检测图】", temp);

	//将得到的二值图像和原图产生的二值图像相与，取出相应的圆
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols;j++)
		{
			if((binaryImage.at<unsigned char>(i,j)==255) && (temp.at<unsigned char>(i,j)==255))
				outImage.at<unsigned char>(i,j)=255;
			else 
				outImage.at<unsigned char>(i,j)=0;
		}	
	imshow("【二次检测图】",outImage);
    waitKey(0);  
    return 0;    
}  