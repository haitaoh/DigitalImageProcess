#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>
using namespace cv;  
using namespace std;//ע�⣡��
int main( )  
{  
    Mat srcImage = imread("F:\\code\\DigitalImageProcess\\DigitalImageProcess\\3.jpg");  
	imshow("��ԭʼͼ��", srcImage);
	int rows=srcImage.rows,cols=srcImage.cols;

	Mat grayImage,binaryImage;
	cvtColor(srcImage,grayImage, CV_BGR2GRAY);
	threshold(grayImage,binaryImage,200,255,THRESH_BINARY);

	Mat temp,outImage;
	binaryImage.copyTo(temp);
	binaryImage.copyTo(outImage);

	//Temp��һ��ȫ�׵�ͼ��
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols;j++)
			temp.at<unsigned char>(i,j)=0;

	temp.copyTo(outImage);
	
    cvtColor(srcImage,grayImage, CV_BGR2GRAY);
	threshold(grayImage,binaryImage,200,255,THRESH_BINARY);
	imshow("����ֵͼ��",binaryImage);
	Canny(binaryImage,binaryImage,100,200,3);
	imshow("����Եͼ��",binaryImage);
	
    //��4�����л���Բ�任  
    vector<Vec3f> circles0;
	vector<Vec3f> circles1;
	HoughCircles(binaryImage, circles0, CV_HOUGH_GRADIENT,3, 10, 200, 250, 10, 200 ); 
	HoughCircles(binaryImage, circles1, CV_HOUGH_GRADIENT,2.9, 300, 200, 120, 10, 100 ); 
  
    //��5��������ͼ�л��Ƴ�Բ  
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
	imshow("�����μ��ͼ��", temp);

	//���õ��Ķ�ֵͼ���ԭͼ�����Ķ�ֵͼ�����룬ȡ����Ӧ��Բ
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols;j++)
		{
			if((binaryImage.at<unsigned char>(i,j)==255) && (temp.at<unsigned char>(i,j)==255))
				outImage.at<unsigned char>(i,j)=255;
			else 
				outImage.at<unsigned char>(i,j)=0;
		}	
	imshow("�����μ��ͼ��",outImage);
    waitKey(0);  
    return 0;    
}  