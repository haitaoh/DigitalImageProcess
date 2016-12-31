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
	label = 0;//��ʼ��ָ�룬Ϊ֮��ÿ�����qlabel������׼��
}

MainWindow::~MainWindow()
{
	if (label) delete label;
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

/*
* ����Բ�任
*/
cv::vector<cv::Vec3f> MainWindow::houghCircles(cv::Mat& image)
{
	cv::Mat imageGray;
	cv::Scalar centerScalar(237, 62, 62), radiusScalar(0, 0, 255);
	int centerRadius = 3;
	cv::cvtColor(image, imageGray, CV_BGR2GRAY);//ת���ɻҶ�ͼ
	cv::GaussianBlur(imageGray, imageGray, cv::Size(9, 9), 2, 2);//��˹ģ�������봦��
	cv::vector<cv::Vec3f> circles;
	cv::HoughCircles(imageGray, circles, CV_HOUGH_GRADIENT, 1, imageGray.rows/20, 100, 60, 0, 0);// ����Բ�任
	/* HoughCircles��mian�����е����ܻ����ԭ����:                *\
	\* ����Ŀ�ܿ��ܲ���ȷ��/��ȱʧ��û��Ϊ ucrtbased.dll ���ط��� */
	/*for (int i = 0; i < circles.size(); i++)
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));//debugģʽ�����л����
		int radius = cvRound(circles[i][2]);//debugģʽ�����л����
		cv::circle(image, center, centerRadius, centerScalar, -1, 8, 0);//Բ��
		cv::circle(image, center, radius, radiusScalar, 1, 8, 0);//Բ��
	}*/
	return circles;
	/*���Դ���*/
	/*cv::namedWindow("ImageShow", CV_WINDOW_AUTOSIZE);
	cv::imshow("ImageShow", image);*/
}

/*
* Ѱ������
*/
void MainWindow::findContours(cv::Mat& image)
{
	cv::Mat image_gray, canny_output;
	cv::cvtColor(image, image_gray, CV_BGR2GRAY);//ת���ɻҶ�ͼ 
	cv::blur(image_gray, image_gray, cv::Size(3, 3));//ģ������
	cv::vector<cv::vector<cv::Point>> contours;
	cv::vector<cv::Vec4i> hierarchy;
	cv::Canny(image_gray, canny_output, 100, 300);//��canny���Ӽ���Ե
	cv::findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));//Ѱ������
	cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		cv::drawContours(drawing, contours, i, cv::Scalar(0, 0, 255), 2, 8, hierarchy, 0, cv::Point());
	}
	cv::namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
}

/*
 * ��ʾͼƬ
 */
void MainWindow::showImage(cv::Mat& image)
{
	cv::cvtColor(image, image, CV_RGB2RGBA);//ͼ����qt��ʾ��������ת��
	QImage img = QImage((const unsigned char*)(image.data), image.cols, image.rows, QImage::Format_RGB32);
	if (label)//ԭָ���ж���ɾ������
	{
		delete label;
		label = 0;//��ָ�룬��ֹ�����쳣
	}
	label = new QLabel();
	label->setPixmap(QPixmap::fromImage(img));
	ui.scrollArea->setWidget(label);
	/*���ô������߶ȺͿ��Ϊ830*480*/
	ui.scrollArea->setMaximumHeight(480);
	ui.scrollArea->setMaximumWidth(830);
	ui.scrollArea->resize(label->pixmap()->size());
}

/*
 * ��ͼƬ
 */
void MainWindow::openImage()
{
	//����ϵͳ��Դ�����������ļ���
	QString filePath = QFileDialog::getOpenFileName(this ,tr("Open Image"),".",tr("Image Files (*.jpg *.png)"));
	if (filePath.length() == 0) return;
	imageFilePath = filePath;//��ֵ��Ϊ�����ļ�
	std::string imagePath = filePath.toStdString();
	image = cv::imread(imagePath);
	if(!image.data) return;
	lineImage = image.clone();//copy�������󣬶�������Ϣͷ
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
 * ����ͼƬ
 */
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
 * ��ʾ����
 */
void MainWindow::showWindow()
{
	this->show();
}

/*
 * ���水ť�����
 */
void MainWindow::positiveButtonClicked()
{
	//do nothing
}

/*
 * ���水ť�����
 */
void MainWindow::negativeButtonClicked()
{
	this->hide();
	emit showMainWindow1();
}

/*
 * ���Բ��ѡ�¼�
 */
void MainWindow::checkBox1(int state)
{
	/* checkBox1 == ���Բ   *\
	\* checkBox2 == ���ӱ� */
	cv::Mat insteadImage;
	if(state == Qt::Checked)//���checkBox1����ѡ
	{
		if (ui.checkBox2->isChecked())//���checkBox2����ѡ,image��lineImage��¡
		{
			insteadImage = lineImage.clone();
		}
		else
		{
			insteadImage = image.clone();
		}
		//���û�Բ����
		//
		//
		//
		cv::vector<cv::Vec3f> circles = houghCircles(insteadImage);
		/* ��Բ */
		cv::Scalar centerScalar(237, 62, 62), radiusScalar(0, 0, 255);
		int centerRadius = 3;
		for (int i = 0; i < circles.size(); i++)
		{
			cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));//debugģʽ�����л����
			int radius = cvRound(circles[i][2]);//debugģʽ�����л����
			cv::circle(insteadImage, center, centerRadius, centerScalar, -1, 8, 0);//Բ��
			cv::circle(insteadImage, center, radius, radiusScalar, 1, 8, 0);//Բ��
		}
		/* ��Բ���� */
	}else
	{
		if (ui.checkBox2->isChecked())//���checkBox2����ѡ,image��lineImage��ȡֵ
		{
			insteadImage = lineImage;
		}
		else
		{
			insteadImage = image;
		}
	}
	if (insteadImage.data) showImage(insteadImage);//�����ݲ�Ϊ�գ���ʾͼƬ
}

/*
 * ���ӱ߹�ѡ�¼�
 */
void MainWindow::checkBox2(int state)
{
	/* checkBox1 == ���Բ   *\
	\* checkBox2 == ���ӱ� */
	cv::Mat insteadImage;
	if(state == Qt::Checked)//���checkBox2����ѡ
	{
		if(ui.checkBox2->isChecked())//���checkBox1����ѡ,image��circleImage��¡����Ϊmat���ݽṹ�������ԣ�clone�����ԭ�����޸�
		{
			insteadImage = circleImage.clone();
		}else
		{
			insteadImage = image.clone();
		}
		//���û��ߺ���
		//
		//
		//
		//
	}else{
		if (ui.checkBox2->isChecked())//���checkBox1����ѡ,image��circleImage��ȡֵ
		{
			insteadImage = circleImage;
		}
		else
		{
			insteadImage = image;
		}
	}
	if (insteadImage.data) showImage(insteadImage);//�����ݲ�Ϊ�գ���ʾͼƬ
}