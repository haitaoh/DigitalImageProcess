#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.openAction,SIGNAL(triggered()),this,SLOT(openImage()));
	connect(ui.saveAction, SIGNAL(triggered()), this, SLOT(saveImage()));
}

MainWindow::~MainWindow()
{
	
}

/*
 * long 类型转 std::string 类型
 */
std::string longToString(long l)
{
	std::ostringstream os;
	os << l;
	std::string result;
	std::istringstream is(os.str());
	is >> result;
	return result;
}

void MainWindow::openImage()
{
	//调用系统资源管理器，打开文件。
	QString filePath = QFileDialog::getOpenFileName(this ,tr("Open Image"),".",tr("Image Files (*.jpg *.png)"));
	if (filePath.length() == 0) return;
	std::string imagePath = filePath.toStdString();
	image = cv::imread(imagePath);
	if(!image.data) return;
	std::clock_t start, end;
	start = std::clock();
	houghCircles(image);
	end = std::clock();
	std::string printMessage = "time consuming:" + longToString(end - start) + " ms";
	cv::Scalar scalar(255,122,122);
	cv::putText(image, printMessage, cv::Point(0, image.cols/2), 1, 1.0, scalar, 1);
	/*cv::namedWindow("Hough Circle", CV_WINDOW_AUTOSIZE);
	cv::imshow("Hough Circle", image);*/
	cv::cvtColor(image, image, CV_RGB2RGBA);//图像在qt显示，必须先转换
	QImage img = QImage((const unsigned char*)(image.data), image.cols, image.rows, QImage::Format_RGB32);
	QLabel *label = new QLabel(this);
	label->move(0, 23);
	label->setPixmap(QPixmap::fromImage(img));
	label->resize(label->pixmap()->size());
	label->show();
}

void MainWindow::saveImage()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Save Image"), ".", tr("Image Files (*.jpg *.png)"));
	if (filePath.length() == 0) return;
}

/*
 * 霍夫圆变换
 */
void MainWindow::houghCircles(cv::Mat& image)
{
	cv::Mat imageGray;
	cv::cvtColor(image, imageGray, CV_BGR2GRAY);//转换成灰度图
	cv::GaussianBlur(imageGray,imageGray,cv::Size(9,9),2,2);//高斯模糊，降噪处理
	cv::vector<cv::Vec3f> circles;
	cv::HoughCircles(imageGray, circles, CV_HOUGH_GRADIENT, 1, imageGray.rows / 8, 200, 100, 0, 0);// 霍夫圆变换
	for(size_t i = 0;i < circles.size();i++)
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		cv::circle(image, center, 3, cv::Scalar(237,62,62), -1, 8, 0);//圆心
		cv::circle(image, center, radius, cv::Scalar(255, 255, 255), 3, 8, 0);//圆边
	}
}