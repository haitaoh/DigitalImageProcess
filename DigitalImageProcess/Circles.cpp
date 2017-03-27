#include <Circles.h>

Circles::Circles()
{
	
}

Circles::Circles(Mat img)
{
	init(img);
}

Circles::~Circles()
{
	
}

/**
 *初始化图像，做一些操作
 */
void Circles::init(Mat img)
{
	Mat image;
	cvtColor(img, image, CV_BGR2GRAY);//转成灰度图
	GaussianBlur(image, image, Size(5, 5), 1, 1);//高斯模糊降噪
	cv::Canny(image, image, 10, 40);
	this->img = image;
}

/**
 * 获取轮廓
 */
void Circles::getContours()
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//
	findContours(img, contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
	//
	this->contour = contours;
}

/**
 * 拟合圆
 */
circleContent Circles::circleLeastFit(const vector<Point> points)
{
	circleContent circleC;
	circleC.x = 0;
	circleC.y = 0;
	circleC.r = 0;
	
	int sum = points.size();
	if(sum < 3)
	{
		return circleC;
	}

	double x = 0, y = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0, xy = 0, xy2 = 0, x2y = 0;
	for(int i = 0;i < sum;i ++)
	{
		double x1 = points[i].x, y1 = points[i].y;
		double x12 = x1 * x1, y12 = y1 * y1;
		x += x1;
		y += y1;
		x2 += x12;
		y2 += y12;
		x3 += x12 * x1;
		y3 += y12 * y1;
		xy += x1 * y1;
		xy2 += x1 * y12;
		x2y += x12 * y1;
	}

	double C, D, E, G, H;
	double a, b, c;
	C = sum*x2 - x*x;
	D = sum*xy - x*y;
	E = sum*x3 + sum*xy2 - (x2 + y2)*x;
	G = sum*y2 - y*y;
	H = sum*x2y + sum*y3 - (x2 + y2)*y;
	a = (H*D - E*G) / (C*G - D*D);
	b = (H*C - E*D) / (D*D - G*C);
	c = -(a*x + b*y + x2 + y2) / sum;

	circleC.x = -a / 2;
	circleC.y = -b / 2;
	circleC.r = sqrt(a*a + b*b - 4 * c) / 2;

	return circleC;
}

/**
 * 计算均方差
 */
float Circles::computeVariance(const vector<Point> points, const circleContent circ)
{
	int size = points.size();
	float d[65535],sum = 0,aver = 0,var = 0;
	for(int i = 0;i < size;i++)
	{
		float x = points[i].x, y = points[i].y;
		float dx = x - circ.x, dy = y - circ.y;
		d[i] = sqrt(dx*dx + dy*dy);
		sum += d[i];
	}

	aver = sum / size;
	for(int i = 0;i < size;i++)
	{
		var += (d[i] - aver)*(d[i] - aver);
	}
	var /= size - 1;
	return sqrt(var);
}

/**
 * 计算圆心到点的距离
 */
float Circles::computeDistance(const Point point, const circleContent circ)
{
	float x = point.x, y = point.y;
	float dx = x - circ.x, dy = y - circ.y;
	return sqrt(dx*dx + dy*dy);
}

/**
 * 拟合椭圆
 */
ellipseContent Circles::ellipseLeastFit(const vector<Point> points)
{
	ellipseContent ellipsec;

	float cx = 0, cy = 0;
	double rp[5], t;
	int n = points.size();

	float *A1 = new float[n * 5];
	float *A2 = new float[2 * 2];
	float *A3 = new float[n * 3];
	float *B1 = new float[n];
	float *B2 = new float[2];
	float *B3 = new float[n];

	const double min_eps = 1e-6;
	
	for(int i = 0;i < n;i++)
	{
		cx += points[i].x;
		cy += points[i].y;
	}
	cx /= n;
	cy /= n;

	for(int i = 0;i < n;i++)
	{
		int step = i * 5;
		float px, py;
		px = points[i].x;
		py = points[i].y;
		px -= cx;
		py -= cy;
		B1[i] = 10000.0;
		A1[step] = -px * px;
		A1[step + 1] = -py * py;
		A1[step + 2] = -px * py;
		A1[step + 3] = px;
		A1[step + 4] = py;
	}

	float *x1 = new float[5];

	//解出Ax^2+By^2+Cxy+Dx+Ey=10000的最小二乘解！
	SVD(A1, n, 5, B1, x1, min_eps);

	A2[0] = 2 * x1[0];
	A2[1] = A2[2] = x1[2];
	A2[3] = 2 * x1[1];

	B2[0] = x1[3];
	B2[1] = x1[4];

	float *x2 = new float[2];

	//标准化，将一次项消掉，求出center.x和center.y;
	SVD(A2, 2, 2, B2, x2, min_eps);
	rp[0] = x2[0];
	rp[1] = x2[1];

	for(int i = 0;i < n;i++)
	{
		int step = i * 3;
		float px, py;
		px = points[i].x;
		py = points[i].y;
		px -= cx;
		py -= cy;
		B3[i] = 1;
		A3[step] = (px - rp[0])*(px - rp[0]);
		A3[step + 1] = (py - rp[1])*(py - rp[1]);
		A3[step + 2] = (px - rp[0])*(py - rp[1]);
	}

	//求出A(x-center.x)^2+B(y-center.y)^2+C(x-center.x)(y-center.y)的最小二乘解
	SVD(A3, n, 3, B3, x1, min_eps);

	rp[4] = -0.5*atan2(x1[2], x1[1] - x1[0]);
	t = sin(-2.0*rp[4]);
	if(fabs(t) > fabs(x1[2])*min_eps)
	{
		t = x1[2] / t;
	}else
	{
		t = x1[1] - x1[0];
	}

	rp[2] = fabs(x1[0] + x1[1] - t);
	if(rp[2] > min_eps)
	{
		rp[2] = sqrt(2.0 / rp[2]);
	}

	rp[3] = fabs(x1[0] + x1[1] + t);
	if(rp[3] > min_eps)
	{
		rp[3] = sqrt(2.0 / rp[3]);
	}

	ellipsec.x = rp[0] + cx;
	ellipsec.y = rp[1] + cy;
	ellipsec.a = rp[2];
	ellipsec.b = rp[3];
	if(ellipsec.b > ellipsec.a)
	{
		double temp = ellipsec.a;
		ellipsec.a = ellipsec.b;
		ellipsec.b = temp;
	}

	ellipsec.xtheta = (90 + rp[4] * 180 / 3.1415926);
	if (ellipsec.xtheta < -180)
		ellipsec.xtheta += 360;
	if (ellipsec.xtheta > 360)
		ellipsec.xtheta -= 360;

	//释放内存
	delete[]A1;
	delete[]A2;
	delete[]A3;
	delete[]B1;
	delete[]B2;
	delete[]B3;
	delete[]x1;
	delete[]x2;

	return ellipsec;
}

int Circles::SVD(float *a, int m, int n, float b[], float x[], float esp)
{
	float *aa = new float[n*m];
	float *u = new float[m*m];
	float *v = new float[n*n];

	int ka, flag;
	if(m > n)
	{
		ka = m + 1;
	}else
	{
		ka = n + 1;
	}

	flag = gmiv(a, m, n, b, x, aa, esp, u, v, ka);

	delete[]aa;
	delete[]u;
	delete[]v;

	return flag;
}

int Circles::gmiv(float a[], int m, int n, float b[], float x[], float aa[], float eps, float u[], float v[], int ka)
{
	int i, j;
	i = gniv(a, m, n, aa, eps, u, v, ka);
	if (i < 0) return -1;
	
	for(i = 0;i < n;i++)
	{
		x[i] = 0.0;
		for(j = 0;j < m;j++)
		{
			x[i] += aa[i*m + j] * b[j];
		}
	}
	return 1;
}

int Circles::gniv(float a[], int m, int n, float aa[], float eps, float u[], float v[], int ka)
{
	int i, j, k, l, t, p, q, f;
	i = muav(a, m, n, u, v, eps, ka);
	if (i < 0) return -1;
	j = n;
	if (m < n) j = m;
	j -= 1;
	k = 0;

	while ((k <= j) && (a[k*n + k] != 0.0))
		k++;
	k--;
	for(i = 0;i < n;i++)
	{
		for(j = 0;j < m;j++)
		{
			t = i*m + j;
			aa[t] = 0.0;
			for(l = 0;l <= k;l ++)
			{
				f = l*n + i;
				p = j*m + l;
				q = l*n + l;
				aa[t] += v[f] * u[p] / a[q];
			}
		}
	}
	return 1;
}





