#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.openAction, SIGNAL(triggered()), this, SLOT(openImage()));
	connect(ui.saveAction, SIGNAL(triggered()), this, SLOT(saveImage()));
	connect(ui.positiveButton, SIGNAL(clicked()), this, SLOT(positiveButtonClicked()));
	connect(ui.negativeButton, SIGNAL(clicked()), this, SLOT(negativeButtonClicked()));
	connect(ui.checkBox1, SIGNAL(stateChanged(int)), this, SLOT(checkBox1(int)));
	label = 0;//初始化指针，为之后每次清空qlabel对象做准备
}

MainWindow::~MainWindow()
{
	if (label) delete label;
}

/*
 * 初始化
 */
void MainWindow::init()
{
	if (image.data)//如果image有数据，利用它初始化circleImage和lineImage
	{
		circleImage = image.clone();
		lineImage = image.clone();
		/*if(circleImage.data)
			doFindPositiveCircles(circleImage);
		if(lineImage.data)
			doFindPositiveLine(lineImage);*/

			//初始化circles,必须按照此方法的执行顺序。
		circles = Circles(image);
		circles.getCircle();//获取圆
		circles.getEllipse();//获取椭圆
		circles.getCircleFromArc();
		circles.getEllipseFromArc();
		//		circles.getSpot();//获取污点
				//计算circleImage和lineImage
		circles.drawCircle(circleImage);
		circles.drawEllipse(circleImage);
		//		circles.drawSpot(circleImage);

	}
}

/*
 * double 转 std::string
 */
string doubleToString(double i)
{
	std::ostringstream os;
	os << i;
	return os.str();
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
 * 设置图片
 */
void MainWindow::setImage(cv::Mat image)
{
	this->image = image;
	init();
}

/*
 *图片预处理
 *转换灰度图和高斯模糊降噪处理
 */
void MainWindow::pretreatmentImage(cv::Mat &sourceImage, cv::Mat &treatmentImage)
{
	cv::cvtColor(sourceImage, treatmentImage, CV_BGR2GRAY);//转换成灰度图
	cv::GaussianBlur(treatmentImage, treatmentImage, cv::Size(5, 5), 1, 1);//高斯模糊，降噪处理
}

/*
 * 打开图片
 */
void MainWindow::openImage()
{
	//调用系统资源管理器，打开文件。
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.jpg *.png)"));
	if (filePath.length() == 0) return;
	imageFilePath = filePath;//赋值，为保存文件
	std::string imagePath = filePath.toStdString();
	Mat imageRead = cv::imread(imagePath);
	if (imageRead.data) {
		setImage(imageRead);
		showImage(image);
	}
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
	if (selectFilter.compare("*.*") && QFileInfo(saveFileName).suffix().isEmpty()) saveFileName = saveFileName + selectFilter.remove(0, 1);
	if (!file.copy(imageFilePath, saveFileName))//如果copy不成功
	{
		QMessageBox::information(this, "Tips", "save file failed!", QMessageBox::Ok, QMessageBox::Ok);
	}
	else
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

double MainWindow::distance(double x, double y)
{
	return x > y ? (x - y) : (y - x);
}

/*
 * 正面按钮被点击
 */
void MainWindow::positiveButtonClicked()
{
	return;
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
 * 检测圆 勾选事件
 */
void MainWindow::checkBox1(int state)
{
	/* checkBox1 == 检测圆  */
	if (state == Qt::Checked)//如果checkBox1被勾选
	{
		result = circleImage.clone();
		double compatibility = 0;
		for (int i = 0; i < circles.circ.size(); i++)
		{
			double temp = circles.circ[i].compatibility;
			if (temp == 0) continue;
			if (distance(temp, 1) < distance(compatibility, 1))
				compatibility = temp;
		}
		for (int i = 0; i < circles.elli.size(); i++)
		{
			double temp = circles.elli[i].compatibility;
			if (temp == 0) continue;
			if (distance(temp, 1) < distance(compatibility, 1))
				compatibility = temp;
		}
		compatibility *= 100;
		std::string printMessage = "compatibility best: " + doubleToString(compatibility) + "%";
		cv::Scalar scalar(0, 0, 255);
		cv::putText(result, printMessage, cv::Point(0, result.cols - 30), 1, 1.0, scalar, 1);

		/*for (int i = 0; i < circles.circleArcContour.size(); i++) {
			if(circles.circleArcContour[i].size() > 30)
				drawContours(result, circles.circleArcContour, i, Scalar(0, 0, 255));
		}*/

		/*for(int i = 0;i < circles.contour.size();i++)
		{
			drawContours(result, circles.contour, i, Scalar(0, 0, 255));
		}*/
	}
	else
	{
		result = image.clone();
	}
	if (result.data)
		showImage(result);//当数据不为空，显示图片
}