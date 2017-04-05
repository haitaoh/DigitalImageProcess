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
 * 初始化
 */
void MainWindow::init()
{
	if(image.data)//如果image有数据，利用它初始化circleImage和lineImage
	{
		circleImage = image.clone();
		lineImage = image.clone();
		/*if(circleImage.data)
			doFindPositiveCircles(circleImage);
		if(lineImage.data)
			doFindPositiveLine(lineImage);*/

		//初始化circles
		circles = Circles(image);
		circles.getCircle();//获取圆
		circles.getEllipse();//获取椭圆
		circles.getSpot();//获取污点
		/*for(int i = 0;i < circles.circ.size();i++)
		{
			std::cout << "(" << circles.circ[i].x << ","
				<< circles.circ[i].y << ") r=" << circles.circ[i].r << std::endl;
		}
		std::cout << "ellipse content" << std::endl;
		std::cout << "ellipse size is: " << circles.elli.size() << std::endl;
		for(int i = 0;i < circles.elli.size();i ++)
		{
			std::cout << "(" << circles.elli[i].x << "," << circles.elli[i].y
				<< ") " << "a=" << circles.elli[i].a << " b=" << circles.elli[i].b
				<< " xtheta=" << circles.elli[i].xtheta << std::endl;
		}
		std::cout << "spot content" << std::endl;
		std::cout << "spot size is: " << circles.spotContour.size() << std::endl;*/
	}
}

/****************************************************************************************\
*                                     Circle Detection                                   *
\****************************************************************************************/

#define hough_cmp_gt(l1,l2) (aux[l1] > aux[l2])

static CV_IMPLEMENT_QSORT_EX(icvHoughSortDescent32s, int, hough_cmp_gt, const int*)

/*
 * 霍夫梯度法
 */
static void
icvHoughCirclesGradient(CvMat* img, float dp, float min_dist,
	int min_radius, int max_radius,
	int canny_threshold_low, int canny_threshold_high, int acc_threshold,
	CvSeq* circles, int circles_max)
{
	const int SHIFT = 10, ONE = 1 << SHIFT;
	cv::Ptr<CvMat> dx, dy;
	cv::Ptr<CvMat> edges, accum, dist_buf;
	std::vector<int> sort_buf;
	cv::Ptr<CvMemStorage> storage;

	int x, y, i, j, k, center_count, nz_count;
	float min_radius2 = (float)min_radius*min_radius;
	float max_radius2 = (float)max_radius*max_radius;
	int rows, cols, arows, acols;
	int astep, *adata;
	float* ddata;
	CvSeq *nz, *centers;
	float idp, dr;
	CvSeqReader reader;

	edges = cvCreateMat(img->rows, img->cols, CV_8UC1);
	cvCanny(img, edges, canny_threshold_low, canny_threshold_high, 3);

	dx = cvCreateMat(img->rows, img->cols, CV_16SC1);
	dy = cvCreateMat(img->rows, img->cols, CV_16SC1);
	cvSobel(img, dx, 1, 0, 3);
	cvSobel(img, dy, 0, 1, 3);

	if (dp < 1.f)
		dp = 1.f;
	idp = 1.f / dp;
	accum = cvCreateMat(cvCeil(img->rows*idp) + 2, cvCeil(img->cols*idp) + 2, CV_32SC1);
	cvZero(accum);

	storage = cvCreateMemStorage();
	nz = cvCreateSeq(CV_32SC2, sizeof(CvSeq), sizeof(CvPoint), storage);
	centers = cvCreateSeq(CV_32SC1, sizeof(CvSeq), sizeof(int), storage);

	rows = img->rows;
	cols = img->cols;
	arows = accum->rows - 2;
	acols = accum->cols - 2;
	adata = accum->data.i;
	astep = accum->step / sizeof(adata[0]);
	// Accumulate circle evidence for each edge pixel
	for (y = 0; y < rows; y++)
	{
		const uchar* edges_row = edges->data.ptr + y*edges->step;
		const short* dx_row = (const short*)(dx->data.ptr + y*dx->step);
		const short* dy_row = (const short*)(dy->data.ptr + y*dy->step);

		for (x = 0; x < cols; x++)
		{
			float vx, vy;
			int sx, sy, x0, y0, x1, y1, r;
			CvPoint pt;

			vx = dx_row[x];
			vy = dy_row[x];

			if (!edges_row[x] || (vx == 0 && vy == 0))
				continue;

			float mag = sqrt(vx*vx + vy*vy);
			assert(mag >= 1);
			sx = cvRound((vx*idp)*ONE / mag);
			sy = cvRound((vy*idp)*ONE / mag);

			x0 = cvRound((x*idp)*ONE);
			y0 = cvRound((y*idp)*ONE);
			// Step from min_radius to max_radius in both directions of the gradient
			for (int k1 = 0; k1 < 2; k1++)
			{
				x1 = x0 + min_radius * sx;
				y1 = y0 + min_radius * sy;

				for (r = min_radius; r <= max_radius; x1 += sx, y1 += sy, r++)
				{
					int x2 = x1 >> SHIFT, y2 = y1 >> SHIFT;
					if ((unsigned)x2 >= (unsigned)acols ||
						(unsigned)y2 >= (unsigned)arows)
						break;
					adata[y2*astep + x2]++;
				}

				sx = -sx; sy = -sy;
			}

			pt.x = x; pt.y = y;
			cvSeqPush(nz, &pt);
		}
	}

	nz_count = nz->total;
	if (!nz_count)
		return;
	//Find possible circle centers
	for (y = 1; y < arows - 1; y++)
	{
		for (x = 1; x < acols - 1; x++)
		{
			int base = y*(acols + 2) + x;
			if (adata[base] > acc_threshold &&
				adata[base] > adata[base - 1] && adata[base] > adata[base + 1] &&
				adata[base] > adata[base - acols - 2] && adata[base] > adata[base + acols + 2])
				cvSeqPush(centers, &base);
		}
	}

	center_count = centers->total;
	if (!center_count)
		return;

	sort_buf.resize(MAX(center_count, nz_count));
	cvCvtSeqToArray(centers, &sort_buf[0]);

	icvHoughSortDescent32s(&sort_buf[0], center_count, adata);
	cvClearSeq(centers);
	cvSeqPushMulti(centers, &sort_buf[0], center_count);

	dist_buf = cvCreateMat(1, nz_count, CV_32FC1);
	ddata = dist_buf->data.fl;

	dr = dp;
	min_dist = MAX(min_dist, dp);
	min_dist *= min_dist;
	// For each found possible center
	// Estimate radius and check support
	for (i = 0; i < centers->total; i++)
	{
		int ofs = *(int*)cvGetSeqElem(centers, i);
		y = ofs / (acols + 2);
		x = ofs - (y)*(acols + 2);
		//Calculate circle's center in pixels
		float cx = (float)((x + 0.5f)*dp), cy = (float)((y + 0.5f)*dp);
		float start_dist, dist_sum;
		float r_best = 0;
		int max_count = 0;
		// Check distance with previously detected circles
		for (j = 0; j < circles->total; j++)
		{
			float* c = (float*)cvGetSeqElem(circles, j);
			if ((c[0] - cx)*(c[0] - cx) + (c[1] - cy)*(c[1] - cy) < min_dist)
				break;
		}

		if (j < circles->total)
			continue;
		// Estimate best radius
		cvStartReadSeq(nz, &reader);
		for (j = k = 0; j < nz_count; j++)
		{
			CvPoint pt;
			float _dx, _dy, _r2;
			CV_READ_SEQ_ELEM(pt, reader);
			_dx = cx - pt.x; _dy = cy - pt.y;
			_r2 = _dx*_dx + _dy*_dy;
			if (min_radius2 <= _r2 && _r2 <= max_radius2)
			{
				ddata[k] = _r2;
				sort_buf[k] = k;
				k++;
			}
		}

		int nz_count1 = k, start_idx = nz_count1 - 1;
		if (nz_count1 == 0)
			continue;
		dist_buf->cols = nz_count1;
		cvPow(dist_buf, dist_buf, 0.5);
		icvHoughSortDescent32s(&sort_buf[0], nz_count1, (int*)ddata);

		dist_sum = start_dist = ddata[sort_buf[nz_count1 - 1]];
		for (j = nz_count1 - 2; j >= 0; j--)
		{
			float d = ddata[sort_buf[j]];

			if (d > max_radius)
				break;

			if (d - start_dist > dr)
			{
				float r_cur = ddata[sort_buf[(j + start_idx) / 2]];
				if ((start_idx - j)*r_best >= max_count*r_cur ||
					(r_best < FLT_EPSILON && start_idx - j >= max_count))
				{
					r_best = r_cur;
					max_count = start_idx - j;
				}
				start_dist = d;
				start_idx = j;
				dist_sum = 0;
			}
			dist_sum += d;
		}
		// Check if the circle has enough support
		if (max_count > acc_threshold)
		{
			float c[3];
			c[0] = cx;
			c[1] = cy;
			c[2] = (float)r_best;
			cvSeqPush(circles, c);
			if (circles->total > circles_max)
				return;
		}
	}
}

/*
 * 霍夫圆变换 使用霍夫梯度法
 */
CvSeq*
cvHoughCircles(CvArr* src_image, void* circle_storage,
	int method, double dp, double min_dist,
	double canny_threshold_low, double canny_threshold_high, double param2,
	int min_radius, int max_radius)
{
	CvSeq* result = 0;//返回计算结果序列
	CvMat stub, *img = (CvMat*)src_image;
	CvMat* mat = 0;
	CvSeq* circles = 0;
	CvSeq circles_header;
	CvSeqBlock circles_block;
	int circles_max = INT_MAX;//max int值
	int canny_threshold_l = cvRound(canny_threshold_low);//canny边缘算法 下阈值
	int canny_threshold_h = cvRound(canny_threshold_high);//canny边缘算法 上阈值
	int acc_threshold = cvRound(param2);//圆心累加 接受阈值

	img = cvGetMat(img, &stub);

	//条件判断，不合格则抛出异常
	if (!CV_IS_MASK_ARR(img))
		CV_Error(CV_StsBadArg, "The source image must be 8-bit, single-channel");

	if (!circle_storage)
		CV_Error(CV_StsNullPtr, "NULL destination");

	if (dp <= 0 || min_dist <= 0 || canny_threshold_l <= 0 || canny_threshold_h <= 0 || acc_threshold <= 0)
		CV_Error(CV_StsOutOfRange, "dp, min_dist, canny_threshold and acc_threshold must be all positive numbers");
	
	//确保圆半径合格
	min_radius = MAX(min_radius, 0);
	if (max_radius <= 0)
		max_radius = MAX(img->rows, img->cols);
	else if (max_radius <= min_radius)
		max_radius = min_radius + 2;

	//根据类型，初始化数组
	if (CV_IS_STORAGE(circle_storage))
	{
		circles = cvCreateSeq(CV_32FC3, sizeof(CvSeq),
			sizeof(float) * 3, (CvMemStorage*)circle_storage);
	}
	else if (CV_IS_MAT(circle_storage))
	{
		mat = (CvMat*)circle_storage;

		if (!CV_IS_MAT_CONT(mat->type) || (mat->rows != 1 && mat->cols != 1) ||
			CV_MAT_TYPE(mat->type) != CV_32FC3)
			CV_Error(CV_StsBadArg,
				"The destination matrix should be continuous and have a single row or a single column");

		circles = cvMakeSeqHeaderForArray(CV_32FC3, sizeof(CvSeq), sizeof(float) * 3,
			mat->data.ptr, mat->rows + mat->cols - 1, &circles_header, &circles_block);
		circles_max = circles->total;
		cvClearSeq(circles);
	}
	else
		CV_Error(CV_StsBadArg, "Destination is not CvMemStorage* nor CvMat*");

	//选择算法，目前只有一种
	switch (method)
	{
	case CV_HOUGH_GRADIENT:
		icvHoughCirclesGradient(img, (float)dp, (float)min_dist,
			min_radius, max_radius, canny_threshold_low,canny_threshold_high,
			acc_threshold, circles, circles_max);
		break;
	default:
		CV_Error(CV_StsBadArg, "Unrecognized method id");
	}

	if (mat)
	{
		if (mat->cols > mat->rows)
			mat->cols = circles->total;
		else
			mat->rows = circles->total;
	}
	else
		result = circles;

	return result;
}

const int STORAGE_SIZE = 1 << 12;

/*
 * 序列转数组（vector）
 */
static void seqToMat(const CvSeq* seq, cv::OutputArray _arr)
{
	if (seq && seq->total > 0)
	{
		_arr.create(1, seq->total, seq->flags, -1, true);
		cv::Mat arr = _arr.getMat();
		cvCvtSeqToArray(seq, arr.data);
	}
	else
		_arr.release();
}

/*
 * c++ 使用cvHoughCircles来获取圆
 */
void myHough::HoughCircles(cv::InputArray _image, cv::OutputArray _circles,
	int method, double dp, double min_dist,
	double canny_threshold_low, double canny_threshold_high, double acc_threshold,
	int minRadius, int maxRadius)
{
	cv::Ptr<CvMemStorage> storage = cvCreateMemStorage(STORAGE_SIZE);
	cv::Mat image = _image.getMat();
	CvMat c_image = image;
	CvSeq* seq = cvHoughCircles(&c_image, storage, method,
		dp, min_dist, canny_threshold_low, canny_threshold_high, acc_threshold, minRadius, maxRadius);
	seqToMat(seq, _circles);
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
 *霍夫线变换 找接边
 */
void MainWindow::houghLines(cv::Mat &image,cv::vector<cv::Vec4i> &lines)
{
	cv::Mat dst;
	pretreatmentImage(image, dst);
	cv::Canny(dst, dst, 10, 12);
	cv::HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 50, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		cv::Vec4i l = lines[i];
		cv::line(image, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 1, CV_AA);
	}
}

/*
* 霍夫圆变换 找圆 绘制圆
*/
void MainWindow::houghCircles(cv::Mat& image,cv::Mat &img,cv::vector<cv::Vec3f> &circles,
	int dp,int min_dist,int canny_threshold_low,int canny_threshold_high,
	int acc_threshold,int min_radius,int max_radius)
{
	cv::Scalar centerScalar(237, 62, 62), radiusScalar(0, 0, 255);
	int centerRadius = 3;
	myHough::HoughCircles(image, circles, CV_HOUGH_GRADIENT, 1, min_dist,
		canny_threshold_low, canny_threshold_high, acc_threshold, min_radius, max_radius);
	for (int i = 0; i < circles.size(); i++)
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		cv::circle(img, center, centerRadius, centerScalar, -1, 8, 0);//圆心
		cv::circle(img, center, radius, radiusScalar, 1, 8, 0);//圆边
	}
}

/*
* 寻找轮廓,利用canny算法
*/
void MainWindow::findContours(cv::Mat& image, cv::vector<cv::vector<cv::Point>> &contours)
{
	cv::Mat imageGray;
	cv::vector<cv::Vec4i> hierarchy;
	cv::cvtColor(image, imageGray, CV_BGR2GRAY);//转换成灰度图
	cv::GaussianBlur(imageGray, imageGray, cv::Size(5,5),1,1);//模糊降噪
	cv::Canny(imageGray, imageGray, 10, 40);
	cv::threshold(imageGray, imageGray, 128, 255, cv::THRESH_BINARY);//阈值化检测，转换为二值对象
	cv::findContours(imageGray,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));
	for(int i = 0;i < contours.size();i++)
	{
		drawContours(image, contours, i, cv::Scalar(255,255,255), 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
		if(contours[i].size() > 5)//必须大于等于6
		{
			cv::ellipse(image, fitEllipse(contours[i]), cv::Scalar(255, 255, 255), 2, 8);
		}
	}
	cv::namedWindow("show", CV_WINDOW_AUTOSIZE);
	cv::imshow("show", image);
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
 * 找圆
 */
void MainWindow::doFindPositiveCircles(cv::Mat &image)
{
	cv::Mat imageGray;
	pretreatmentImage(image, imageGray);
	cv::vector<cv::Vec3f> circles1;
	houghCircles(imageGray, image, circles1, 1, 1, 15, 44, 115, 74, 189);//外圈
	cv::vector<cv::Vec3f> circles2;
	houghCircles(imageGray, image, circles2, 1, 1, 15, 44, 122, 177, 189);//内圈
	cv::vector<cv::Vec3f> circles3;
	houghCircles(imageGray, image, circles3, 1, 1, 15, 44, 74, 74, 177);//底面
}

/*
 * 找接边
 */
void MainWindow::doFindPositiveLine(cv::Mat &image)
{
	cv::vector<cv::Vec4i> lines;
	houghLines(image, lines);
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
 * 检测圆 勾选事件
 */
void MainWindow::checkBox1(int state)
{
	/* checkBox1 == 检测圆   *\
	\* checkBox2 == 检测接边 */
	if(state == Qt::Checked)//如果checkBox1被勾选
	{
		if (ui.checkBox2->isChecked())//如果checkBox2被勾选,image从lineImage克隆
		{
			result = lineImage.clone();
			//doFindPositiveCircles(result)
			std::clock_t start, end;
			start = std::clock();
			doFindPositiveCircles(result);
			end = std::clock();
			std::string printMessage = "time consuming:" + longToString(end - start) + " ms";
			cv::Scalar scalar(255, 122, 122);
			cv::putText(result, printMessage, cv::Point(0, result.cols / 2), 1, 1.0, scalar, 1);
		}
		else
		{
//			result = circleImage.clone();
			/*result = image.clone();
			for (int i = 0; i < circles.contour.size(); i++) {
				drawContours(result, circles.contour, i, Scalar(0, 0, 255));
			}*/
			circles.drawCircle(result);
			circles.drawEllipse(result);
			circles.drawSpot(result);
		}
	}else
	{
		if (ui.checkBox2->isChecked())//如果checkBox2被勾选,image从lineImage获取值
		{
			result = lineImage.clone();
		}
		else
		{
			result = image.clone();
		}
	}
	if (result.data)
		showImage(result);//当数据不为空，显示图片
}

/*
 * 检测接边勾选事件
 */
void MainWindow::checkBox2(int state)
{
	/* checkBox1 == 检测圆   *\
	\* checkBox2 == 检测接边 */
	if(state == Qt::Checked)//如果checkBox2被勾选
	{
		if(ui.checkBox1->isChecked())//如果checkBox1被勾选,image从circleImage克隆，因为mat数据结构的特殊性，clone不会对原数据修改
		{
			result = circleImage.clone();
			//doFindPositiveLine(result)
			doFindPositiveLine(result);
		}else
		{
			result = lineImage.clone();
		}
	}else{
		if (ui.checkBox1->isChecked())//如果checkBox1被勾选,image从circleImage获取值
		{
			result = circleImage;
		}
		else
		{
			result = image;
		}
	}
	if (result.data) showImage(result);//当数据不为空，显示图片
}