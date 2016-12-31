#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.openAction,SIGNAL(triggered()),this,SLOT(openImage()));
	connect(ui.saveAction, SIGNAL(triggered()), this, SLOT(saveImage()));
	connect(ui.positiveButton, SIGNAL(clicked()), this, SLOT(positiveButtonClicked()));
	connect(ui.negativeButton, SIGNAL(clicked()), this, SLOT(negativeButtonClicked()));
	connect(ui.checkBox1, SIGNAL(stateChanged(int)), this, SLOT(checkBox1(int)));
	connect(ui.checkBox2, SIGNAL(stateChanged(int)), this, SLOT(checkBox2(int)));
	label = 0;//初始化指针，为之后每次清空qlabel对象做准备
}

MainWindow::~MainWindow()
{
	if (label) delete label;
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

/*
* 霍夫圆变换
*/
cv::vector<cv::Vec3f> MainWindow::houghCircles(cv::Mat& image)
{
	cv::Mat imageGray;
	cv::Scalar centerScalar(237, 62, 62), radiusScalar(0, 0, 255);
	int centerRadius = 3;
	cv::cvtColor(image, imageGray, CV_BGR2GRAY);//转换成灰度图
	cv::GaussianBlur(imageGray, imageGray, cv::Size(9, 9), 2, 2);//高斯模糊，降噪处理
	cv::vector<cv::Vec3f> circles;
	cv::HoughCircles(imageGray, circles, CV_HOUGH_GRADIENT, 1, imageGray.rows/20, 100, 60, 0, 0);// 霍夫圆变换
	/* HoughCircles在mian函数中单独跑会出错，原因是:                *\
	\* 下面的框架可能不正确和/或缺失，没有为 ucrtbased.dll 加载符号 */
	/*for (int i = 0; i < circles.size(); i++)
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));//debug模式下这行会出错
		int radius = cvRound(circles[i][2]);//debug模式下这行会出错
		cv::circle(image, center, centerRadius, centerScalar, -1, 8, 0);//圆心
		cv::circle(image, center, radius, radiusScalar, 1, 8, 0);//圆边
	}*/
	return circles;
	/*调试窗口*/
	/*cv::namedWindow("ImageShow", CV_WINDOW_AUTOSIZE);
	cv::imshow("ImageShow", image);*/
}

/*
* 寻找轮廓
*/
void MainWindow::findContours(cv::Mat& image)
{
	cv::Mat image_gray, canny_output;
	cv::cvtColor(image, image_gray, CV_BGR2GRAY);//转换成灰度图 
	cv::blur(image_gray, image_gray, cv::Size(3, 3));//模糊降噪
	cv::vector<cv::vector<cv::Point>> contours;
	cv::vector<cv::Vec4i> hierarchy;
	cv::Canny(image_gray, canny_output, 100, 300);//用canny算子检测边缘
	cv::findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));//寻找轮廓
	cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		cv::drawContours(drawing, contours, i, cv::Scalar(0, 0, 255), 2, 8, hierarchy, 0, cv::Point());
	}
	cv::namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
}

/*
 * 显示图片
 */
void MainWindow::showImage(cv::Mat& image)
{
	cv::cvtColor(image, image, CV_RGB2RGBA);//图像在qt显示，必须先转换
	QImage img = QImage((const unsigned char*)(image.data), image.cols, image.rows, QImage::Format_RGB32);
	if (label)//原指针有对象，删除对象
	{
		delete label;
		label = 0;//空指针，防止访问异常
	}
	label = new QLabel();
	label->setPixmap(QPixmap::fromImage(img));
	ui.scrollArea->setWidget(label);
	/*设置窗口最大高度和宽度为830*480*/
	ui.scrollArea->setMaximumHeight(480);
	ui.scrollArea->setMaximumWidth(830);
	ui.scrollArea->resize(label->pixmap()->size());
}

/*
 * 打开图片
 */
void MainWindow::openImage()
{
	//调用系统资源管理器，打开文件。
	QString filePath = QFileDialog::getOpenFileName(this ,tr("Open Image"),".",tr("Image Files (*.jpg *.png)"));
	if (filePath.length() == 0) return;
	imageFilePath = filePath;//赋值，为保存文件
	std::string imagePath = filePath.toStdString();
	image = cv::imread(imagePath);
	if(!image.data) return;
	lineImage = image.clone();//copy整个对象，而不是信息头
	circleImage = image.clone();
/*	std::clock_t start, end;
	start = std::clock();
	houghCircles(image);
	findContours(image);
	end = std::clock();
	std::string printMessage = "time consuming:" + longToString(end - start) + " ms";
	cv::Scalar scalar(255,122,122);
	cv::putText(image, printMessage, cv::Point(0, image.cols/2), 1, 1.0, scalar, 1);*/
	showImage(image);
}

/*
 * 保存图片
 */
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
 * 显示窗口
 */
void MainWindow::showWindow()
{
	this->show();
}

/*
 * 正面按钮被点击
 */
void MainWindow::positiveButtonClicked()
{
	//do nothing
}

/*
 * 反面按钮被点击
 */
void MainWindow::negativeButtonClicked()
{
	this->hide();
	emit showMainWindow1();
}

/*
 * 检测圆勾选事件
 */
void MainWindow::checkBox1(int state)
{
	/* checkBox1 == 检测圆   *\
	\* checkBox2 == 检测接边 */
	cv::Mat insteadImage;
	if(state == Qt::Checked)//如果checkBox1被勾选
	{
		if (ui.checkBox2->isChecked())//如果checkBox2被勾选,image从lineImage克隆
		{
			insteadImage = lineImage.clone();
		}
		else
		{
			insteadImage = image.clone();
		}
		//调用画圆函数
		//
		//
		//
		cv::vector<cv::Vec3f> circles = houghCircles(insteadImage);
		/* 画圆 */
		cv::Scalar centerScalar(237, 62, 62), radiusScalar(0, 0, 255);
		int centerRadius = 3;
		for (int i = 0; i < circles.size(); i++)
		{
			cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));//debug模式下这行会出错
			int radius = cvRound(circles[i][2]);//debug模式下这行会出错
			cv::circle(insteadImage, center, centerRadius, centerScalar, -1, 8, 0);//圆心
			cv::circle(insteadImage, center, radius, radiusScalar, 1, 8, 0);//圆边
		}
		/* 画圆结束 */
	}else
	{
		if (ui.checkBox2->isChecked())//如果checkBox2被勾选,image从lineImage获取值
		{
			insteadImage = lineImage;
		}
		else
		{
			insteadImage = image;
		}
	}
	if (insteadImage.data) showImage(insteadImage);//当数据不为空，显示图片
}

/*
 * 检测接边勾选事件
 */
void MainWindow::checkBox2(int state)
{
	/* checkBox1 == 检测圆   *\
	\* checkBox2 == 检测接边 */
	cv::Mat insteadImage;
	if(state == Qt::Checked)//如果checkBox2被勾选
	{
		if(ui.checkBox2->isChecked())//如果checkBox1被勾选,image从circleImage克隆，因为mat数据结构的特殊性，clone不会对原数据修改
		{
			insteadImage = circleImage.clone();
		}else
		{
			insteadImage = image.clone();
		}
		//调用画线函数
		//
		//
		//
		//
	}else{
		if (ui.checkBox2->isChecked())//如果checkBox1被勾选,image从circleImage获取值
		{
			insteadImage = circleImage;
		}
		else
		{
			insteadImage = image;
		}
	}
	if (insteadImage.data) showImage(insteadImage);//当数据不为空，显示图片
}