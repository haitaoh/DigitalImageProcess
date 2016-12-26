#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.openAction,SIGNAL(triggered()),this,SLOT(openImage()));
	connect(ui.saveAction, SIGNAL(triggered()), this, SLOT(saveImage()));
	connect(ui.positiveButton, SIGNAL(clicked()), this, SLOT(positiveButtonClicked()));
	connect(ui.negativeButton, SIGNAL(clicked()), this, SLOT(negativeButtonClicked()));
}

MainWindow::~MainWindow()
{
	
}

/*
 * long 类型转 std::string 类型
 */
std::string MainWindow::longToString(long l)
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
	imageFilePath = filePath;//赋值，为保存文件
	std::string imagePath = filePath.toStdString();
	image = cv::imread(imagePath);
	if(!image.data) return;
	std::clock_t start, end;
	start = std::clock();
//	houghCircles(image);
	findContours(image);
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
	ui.scrollArea->setWidget(label);
	/*设置窗口最大高度和宽度为840*480*/
	ui.scrollArea->setMaximumHeight(480);
	ui.scrollArea->setMaximumWidth(840);
	ui.scrollArea->resize(label->pixmap()->size());
}

void MainWindow::saveImage()
{
	if (imageFilePath.isEmpty()) return;
	QString suffix = QFileInfo(imageFilePath).suffix();//得到文件后缀
	QString setFilter = "image(*.jpg *.png)";//设置匹配格式
	QString dirString, selectFilter;
	dirString = QFileInfo(imageFilePath).fileName();
	QString saveFileName = QFileDialog::getSaveFileName(this, "Save Image", dirString, setFilter, &selectFilter,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (saveFileName.isEmpty()) return;
	if (!suffix.isEmpty() && setFilter.contains(selectFilter))
		selectFilter = suffix.insert(0, "*.");
	if (!suffix.isEmpty() && !setFilter.contains(selectFilter))
		selectFilter = "*.*";
	QFile file(saveFileName);
	if (selectFilter.compare("*.*") && QFileInfo(saveFileName).suffix().isEmpty()) saveFileName = saveFileName + selectFilter.remove(0,1);
	if(!file.copy(imageFilePath,saveFileName))//如果copy不成功
	{
		QMessageBox::information(this, "Tips", "save file failed!", QMessageBox::Ok, QMessageBox::Ok);
	}else
	{
		QMessageBox::information(this, "Tips", "save file succeed!", QMessageBox::Ok, QMessageBox::Ok);
	}
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
	cv::vector<cv::Vec3f> circles1;
	cv::HoughCircles(imageGray, circles, CV_HOUGH_GRADIENT, 3, 10, 200, 250, 10, 200);// 霍夫圆变换
	for(size_t i = 0;i < circles.size();i++)
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		cv::circle(image, center, 3, cv::Scalar(237,62,62), -1, 8, 0);//圆心
		cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);//圆边
	}
	/*cv::HoughCircles(imageGray, circles1, CV_HOUGH_GRADIENT, 2.9, 300, 200, 120, 10, 100);// 霍夫圆变换
	for (size_t i = 0; i < circles1.size(); i++)
	{
		cv::Point center(cvRound(circles1[i][0]), cvRound(circles1[i][1]));
		int radius = cvRound(circles1[i][2]);
		cv::circle(image, center, 3, cv::Scalar(237, 62, 62), -1, 8, 0);//圆心
		cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);//圆边
	}*/
}

/*
 * 寻找轮廓
 */
void MainWindow::findContours(cv::Mat& image)
{
	cv::Mat image_gray,canny_output;
	cv::cvtColor(image, image_gray, CV_BGR2GRAY);//转换成灰度图 
	cv::blur(image_gray, image_gray, cv::Size(3, 3));//模糊降噪
	cv::vector<cv::vector<cv::Point>> contours;
	cv::vector<cv::Vec4i> hierarchy;
	cv::Canny(image_gray, canny_output, 100, 300);//用canny算子检测边缘
	cv::findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));//寻找轮廓
	cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
	for(int i = 0;i < contours.size();i++)
	{
		cv::drawContours(drawing, contours, i, cv::Scalar(0, 0, 255), 2, 8, hierarchy, 0, cv::Point());
	}
	cv::namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
}

void MainWindow::showWindow()
{
	this->show();
}

void MainWindow::positiveButtonClicked()
{
	//do nothing
}

void MainWindow::negativeButtonClicked()
{
	this->hide();
	emit showMainWindow1();
}