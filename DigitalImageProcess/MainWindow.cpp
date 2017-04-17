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
	label = 0;//��ʼ��ָ�룬Ϊ֮��ÿ�����qlabel������׼��
}

MainWindow::~MainWindow()
{
	if (label) delete label;
}

/*
 * ��ʼ��
 */
void MainWindow::init()
{
	if (image.data)//���image�����ݣ���������ʼ��circleImage��lineImage
	{
		circleImage = image.clone();
		lineImage = image.clone();
		/*if(circleImage.data)
			doFindPositiveCircles(circleImage);
		if(lineImage.data)
			doFindPositiveLine(lineImage);*/

			//��ʼ��circles,���밴�մ˷�����ִ��˳��
		circles = Circles(image);
		circles.getCircle();//��ȡԲ
		circles.getEllipse();//��ȡ��Բ
		circles.getCircleFromArc();
		circles.getEllipseFromArc();
		//		circles.getSpot();//��ȡ�۵�
				//����circleImage��lineImage
		circles.drawCircle(circleImage);
		circles.drawEllipse(circleImage);
		//		circles.drawSpot(circleImage);

	}
}

/*
 * double ת std::string
 */
string doubleToString(double i)
{
	std::ostringstream os;
	os << i;
	return os.str();
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
 * ����ͼƬ
 */
void MainWindow::setImage(cv::Mat image)
{
	this->image = image;
	init();
}

/*
 *ͼƬԤ����
 *ת���Ҷ�ͼ�͸�˹ģ�����봦��
 */
void MainWindow::pretreatmentImage(cv::Mat &sourceImage, cv::Mat &treatmentImage)
{
	cv::cvtColor(sourceImage, treatmentImage, CV_BGR2GRAY);//ת���ɻҶ�ͼ
	cv::GaussianBlur(treatmentImage, treatmentImage, cv::Size(5, 5), 1, 1);//��˹ģ�������봦��
}

/*
 * ��ͼƬ
 */
void MainWindow::openImage()
{
	//����ϵͳ��Դ�����������ļ���
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.jpg *.png)"));
	if (filePath.length() == 0) return;
	imageFilePath = filePath;//��ֵ��Ϊ�����ļ�
	std::string imagePath = filePath.toStdString();
	Mat imageRead = cv::imread(imagePath);
	if (imageRead.data) {
		setImage(imageRead);
		showImage(image);
	}
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
	if (selectFilter.compare("*.*") && QFileInfo(saveFileName).suffix().isEmpty()) saveFileName = saveFileName + selectFilter.remove(0, 1);
	if (!file.copy(imageFilePath, saveFileName))//���copy���ɹ�
	{
		QMessageBox::information(this, "Tips", "save file failed!", QMessageBox::Ok, QMessageBox::Ok);
	}
	else
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

double MainWindow::distance(double x, double y)
{
	return x > y ? (x - y) : (y - x);
}

/*
 * ���水ť�����
 */
void MainWindow::positiveButtonClicked()
{
	return;
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
 * ���Բ ��ѡ�¼�
 */
void MainWindow::checkBox1(int state)
{
	/* checkBox1 == ���Բ  */
	if (state == Qt::Checked)//���checkBox1����ѡ
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
		showImage(result);//�����ݲ�Ϊ�գ���ʾͼƬ
}