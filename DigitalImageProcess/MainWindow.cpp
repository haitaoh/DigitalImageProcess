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
 * long ����ת std::string ����
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
	//����ϵͳ��Դ�����������ļ���
	QString filePath = QFileDialog::getOpenFileName(this ,tr("Open Image"),".",tr("Image Files (*.jpg *.png)"));
	if (filePath.length() == 0) return;
	imageFilePath = filePath;//��ֵ��Ϊ�����ļ�
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
	cv::cvtColor(image, image, CV_RGB2RGBA);//ͼ����qt��ʾ��������ת��
	QImage img = QImage((const unsigned char*)(image.data), image.cols, image.rows, QImage::Format_RGB32);
	QLabel *label = new QLabel(this);
	label->move(0, 23);
	label->setPixmap(QPixmap::fromImage(img));
	ui.scrollArea->setWidget(label);
	/*���ô������߶ȺͿ��Ϊ840*480*/
	ui.scrollArea->setMaximumHeight(480);
	ui.scrollArea->setMaximumWidth(840);
	ui.scrollArea->resize(label->pixmap()->size());
}

void MainWindow::saveImage()
{
	if (imageFilePath.isEmpty()) return;
	QString suffix = QFileInfo(imageFilePath).suffix();//�õ��ļ���׺
	QString setFilter = "image(*.jpg *.png)";//����ƥ���ʽ
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
	if(!file.copy(imageFilePath,saveFileName))//���copy���ɹ�
	{
		QMessageBox::information(this, "Tips", "save file failed!", QMessageBox::Ok, QMessageBox::Ok);
	}else
	{
		QMessageBox::information(this, "Tips", "save file succeed!", QMessageBox::Ok, QMessageBox::Ok);
	}
}

/*
 * ����Բ�任
 */
void MainWindow::houghCircles(cv::Mat& image)
{
	cv::Mat imageGray;
	cv::cvtColor(image, imageGray, CV_BGR2GRAY);//ת���ɻҶ�ͼ
	cv::GaussianBlur(imageGray,imageGray,cv::Size(9,9),2,2);//��˹ģ�������봦��
	cv::vector<cv::Vec3f> circles;
	cv::vector<cv::Vec3f> circles1;
	cv::HoughCircles(imageGray, circles, CV_HOUGH_GRADIENT, 3, 10, 200, 250, 10, 200);// ����Բ�任
	for(size_t i = 0;i < circles.size();i++)
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		cv::circle(image, center, 3, cv::Scalar(237,62,62), -1, 8, 0);//Բ��
		cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);//Բ��
	}
	/*cv::HoughCircles(imageGray, circles1, CV_HOUGH_GRADIENT, 2.9, 300, 200, 120, 10, 100);// ����Բ�任
	for (size_t i = 0; i < circles1.size(); i++)
	{
		cv::Point center(cvRound(circles1[i][0]), cvRound(circles1[i][1]));
		int radius = cvRound(circles1[i][2]);
		cv::circle(image, center, 3, cv::Scalar(237, 62, 62), -1, 8, 0);//Բ��
		cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);//Բ��
	}*/
}

/*
 * Ѱ������
 */
void MainWindow::findContours(cv::Mat& image)
{
	cv::Mat image_gray,canny_output;
	cv::cvtColor(image, image_gray, CV_BGR2GRAY);//ת���ɻҶ�ͼ 
	cv::blur(image_gray, image_gray, cv::Size(3, 3));//ģ������
	cv::vector<cv::vector<cv::Point>> contours;
	cv::vector<cv::Vec4i> hierarchy;
	cv::Canny(image_gray, canny_output, 100, 300);//��canny���Ӽ���Ե
	cv::findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));//Ѱ������
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