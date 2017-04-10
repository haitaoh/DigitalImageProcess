#include <Circles.h>

Circles::Circles()
{

}

Circles::Circles(Mat img)
{
	this->src = img;
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
	findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
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
	if (sum < 3)
	{
		return circleC;
	}

	double x = 0, y = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0, xy = 0, xy2 = 0, x2y = 0;
	for (int i = 0; i < sum; i++)
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
	float d[65535], sum = 0, aver = 0, var = 0;
	for (int i = 0; i < size; i++)
	{
		float x = points[i].x, y = points[i].y;
		float dx = x - circ.x, dy = y - circ.y;
		d[i] = sqrt(dx*dx + dy*dy);
		sum += d[i];
	}

	aver = sum / size;
	for (int i = 0; i < size; i++)
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

	for (int i = 0; i < n; i++)
	{
		cx += points[i].x*1.0;
		cy += points[i].y*1.0;
	}
	cx /= n;
	cy /= n;

	for (int i = 0; i < n; i++)
	{
		int step = i * 5;
		float px, py;
		px = points[i].x*1.0;
		py = points[i].y*1.0;
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

	for (int i = 0; i < n; i++)
	{
		int step = i * 3;
		float px, py;
		px = points[i].x*1.0;
		py = points[i].y*1.0;
		px -= cx;
		py -= cy;
		B3[i] = 1.0;
		A3[step] = (px - rp[0])*(px - rp[0]);
		A3[step + 1] = (py - rp[1])*(py - rp[1]);
		A3[step + 2] = (px - rp[0])*(py - rp[1]);
	}

	//求出A(x-center.x)^2+B(y-center.y)^2+C(x-center.x)(y-center.y)的最小二乘解
	SVD(A3, n, 3, B3, x1, min_eps);

	rp[4] = -0.5*atan2(x1[2], x1[1] - x1[0]);
	t = sin(-2.0*rp[4]);
	if (fabs(t) > fabs(x1[2])*min_eps)
	{
		t = x1[2] / t;
	}
	else
	{
		t = x1[1] - x1[0];
	}

	rp[2] = fabs(x1[0] + x1[1] - t);
	if (rp[2] > min_eps)
	{
		rp[2] = sqrt(2.0 / rp[2]);
	}

	rp[3] = fabs(x1[0] + x1[1] + t);
	if (rp[3] > min_eps)
	{
		rp[3] = sqrt(2.0 / rp[3]);
	}

	ellipsec.x = (float)rp[0] + cx;
	ellipsec.y = (float)rp[1] + cy;

	ellipsec.a = (float)rp[2];
	ellipsec.b = (float)rp[3];
	if (ellipsec.b > ellipsec.a)
	{
		double temp = ellipsec.a;
		ellipsec.a = ellipsec.b;
		ellipsec.b = temp;
	}

	//与x轴夹角，用圆周角度数表示
	ellipsec.xtheta = (float)(3.1415926 / 2 + rp[4]);
	if (ellipsec.xtheta < -3.1415926)
		ellipsec.xtheta += 3.1415926 * 2;
	if (ellipsec.xtheta > 3.1415926 * 2)
		ellipsec.xtheta -= 3.1415926 * 2;

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
	if (m > n)
	{
		ka = m + 1;
	}
	else
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

	for (i = 0; i < n; i++)
	{
		x[i] = 0.0;
		for (j = 0; j < m; j++)
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
	j--;
	k = 0;

	while ((k <= j) && (a[k*n + k] != 0.0))
		k++;
	k--;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			t = i*m + j;
			aa[t] = 0.0;
			for (l = 0; l <= k; l++)
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

int Circles::muav(float a[], int m, int n, float u[], float v[], float eps, int ka)
{
	int i, j, k, l, it, ll, kk, ix, iy, mm, nn, iz, m1, ks;
	float d, dd, t, sm, sm1, em1, sk, ek, b, c, shh, fg[2], cs[2];
	float *s, *e, *w;

	void ppp(float a[], float e[], float s[], float v[], int m, int n);
	void sss(float fg[], float cs[]);

	s = (float *)malloc(ka * sizeof(float));
	e = (float *)malloc(ka * sizeof(float));
	w = (float *)malloc(ka * sizeof(float));
	it = 60; k = n;
	if (m - 1 < n) k = m - 1;
	l = m;
	if (n - 2 < m) l = n - 2;
	if (l < 0) l = 0;
	ll = k;
	if (l > k) ll = l;
	if (ll >= 1)
	{
		for (kk = 1; kk <= ll; kk++)
		{
			if (kk <= k)
			{
				d = 0.0;
				for (i = kk; i <= m; i++)
				{
					ix = (i - 1)*n + kk - 1; d = d + a[ix] * a[ix];
				}
				s[kk - 1] = (float)sqrt(d);
				if (s[kk - 1] != 0.0)
				{
					ix = (kk - 1)*n + kk - 1;
					if (a[ix] != 0.0)
					{
						s[kk - 1] = (float)fabs(s[kk - 1]);
						if (a[ix] < 0.0) s[kk - 1] = -s[kk - 1];
					}
					for (i = kk; i <= m; i++)
					{
						iy = (i - 1)*n + kk - 1;
						a[iy] = a[iy] / s[kk - 1];
					}
					a[ix] = 1.0f + a[ix];
				}
				s[kk - 1] = -s[kk - 1];
			}
			if (n >= kk + 1)
			{
				for (j = kk + 1; j <= n; j++)
				{
					if ((kk <= k) && (s[kk - 1] != 0.0))
					{
						d = 0.0;
						for (i = kk; i <= m; i++)
						{
							ix = (i - 1)*n + kk - 1;
							iy = (i - 1)*n + j - 1;
							d = d + a[ix] * a[iy];
						}
						d = -d / a[(kk - 1)*n + kk - 1];
						for (i = kk; i <= m; i++)
						{
							ix = (i - 1)*n + j - 1;
							iy = (i - 1)*n + kk - 1;
							a[ix] = a[ix] + d*a[iy];
						}
					}
					e[j - 1] = a[(kk - 1)*n + j - 1];
				}
			}
			if (kk <= k)
			{
				for (i = kk; i <= m; i++)
				{
					ix = (i - 1)*m + kk - 1; iy = (i - 1)*n + kk - 1;
					u[ix] = a[iy];
				}
			}
			if (kk <= l)
			{
				d = 0.0;
				for (i = kk + 1; i <= n; i++)
					d = d + e[i - 1] * e[i - 1];
				e[kk - 1] = (float)sqrt(d);
				if (e[kk - 1] != 0.0)
				{
					if (e[kk] != 0.0)
					{
						e[kk - 1] = (float)fabs(e[kk - 1]);
						if (e[kk] < 0.0) e[kk - 1] = -e[kk - 1];
					}
					for (i = kk + 1; i <= n; i++)
						e[i - 1] = e[i - 1] / e[kk - 1];
					e[kk] = 1.0f + e[kk];
				}
				e[kk - 1] = -e[kk - 1];
				if ((kk + 1 <= m) && (e[kk - 1] != 0.0))
				{
					for (i = kk + 1; i <= m; i++) w[i - 1] = 0.0;
					for (j = kk + 1; j <= n; j++)
						for (i = kk + 1; i <= m; i++)
							w[i - 1] = w[i - 1] + e[j - 1] * a[(i - 1)*n + j - 1];
					for (j = kk + 1; j <= n; j++)
						for (i = kk + 1; i <= m; i++)
						{
							ix = (i - 1)*n + j - 1;
							a[ix] = a[ix] - w[i - 1] * e[j - 1] / e[kk];
						}
				}
				for (i = kk + 1; i <= n; i++)
					v[(i - 1)*n + kk - 1] = e[i - 1];
			}
		}
	}
	mm = n;
	if (m + 1 < n) mm = m + 1;
	if (k < n) s[k] = a[k*n + k];
	if (m < mm) s[mm - 1] = 0.0;
	if (l + 1 < mm) e[l] = a[l*n + mm - 1];
	e[mm - 1] = 0.0;
	nn = m;
	if (m > n) nn = n;
	if (nn >= k + 1)
	{
		for (j = k + 1; j <= nn; j++)
		{
			for (i = 1; i <= m; i++)
				u[(i - 1)*m + j - 1] = 0.0;
			u[(j - 1)*m + j - 1] = 1.0;
		}
	}
	if (k >= 1)
	{
		for (ll = 1; ll <= k; ll++)
		{
			kk = k - ll + 1; iz = (kk - 1)*m + kk - 1;
			if (s[kk - 1] != 0.0)
			{
				if (nn >= kk + 1)
					for (j = kk + 1; j <= nn; j++)
					{
						d = 0.0;
						for (i = kk; i <= m; i++)
						{
							ix = (i - 1)*m + kk - 1;
							iy = (i - 1)*m + j - 1;
							d = d + u[ix] * u[iy] / u[iz];
						}
						d = -d;
						for (i = kk; i <= m; i++)
						{
							ix = (i - 1)*m + j - 1;
							iy = (i - 1)*m + kk - 1;
							u[ix] = u[ix] + d*u[iy];
						}
					}
				for (i = kk; i <= m; i++)
				{
					ix = (i - 1)*m + kk - 1; u[ix] = -u[ix];
				}
				u[iz] = 1.0f + u[iz];
				if (kk - 1 >= 1)
					for (i = 1; i <= kk - 1; i++)
						u[(i - 1)*m + kk - 1] = 0.0;
			}
			else
			{
				for (i = 1; i <= m; i++)
					u[(i - 1)*m + kk - 1] = 0.0;
				u[(kk - 1)*m + kk - 1] = 1.0;
			}
		}
	}
	for (ll = 1; ll <= n; ll++)
	{
		kk = n - ll + 1; iz = kk*n + kk - 1;
		if ((kk <= l) && (e[kk - 1] != 0.0))
		{
			for (j = kk + 1; j <= n; j++)
			{
				d = 0.0;
				for (i = kk + 1; i <= n; i++)
				{
					ix = (i - 1)*n + kk - 1; iy = (i - 1)*n + j - 1;
					d = d + v[ix] * v[iy] / v[iz];
				}
				d = -d;
				for (i = kk + 1; i <= n; i++)
				{
					ix = (i - 1)*n + j - 1; iy = (i - 1)*n + kk - 1;
					v[ix] = v[ix] + d*v[iy];
				}
			}
		}
		for (i = 1; i <= n; i++)
			v[(i - 1)*n + kk - 1] = 0.0;
		v[iz - n] = 1.0;
	}
	for (i = 1; i <= m; i++)
		for (j = 1; j <= n; j++)
			a[(i - 1)*n + j - 1] = 0.0;
	m1 = mm; it = 60;
	while (true)
	{
		if (mm == 0)
		{
			ppp(a, e, s, v, m, n);
			free(s); free(e); free(w); return(1);
		}
		if (it == 0)
		{
			ppp(a, e, s, v, m, n);
			free(s); free(e); free(w); return(-1);
		}
		kk = mm - 1;
		while ((kk != 0) && (fabs(e[kk - 1]) != 0.0))
		{
			d = (float)(fabs(s[kk - 1]) + fabs(s[kk]));
			dd = (float)fabs(e[kk - 1]);
			if (dd > eps*d) kk = kk - 1;
			else e[kk - 1] = 0.0;
		}
		if (kk == mm - 1)
		{
			kk = kk + 1;
			if (s[kk - 1] < 0.0)
			{
				s[kk - 1] = -s[kk - 1];
				for (i = 1; i <= n; i++)
				{
					ix = (i - 1)*n + kk - 1; v[ix] = -v[ix];
				}
			}
			while ((kk != m1) && (s[kk - 1] < s[kk]))
			{
				d = s[kk - 1]; s[kk - 1] = s[kk]; s[kk] = d;
				if (kk < n)
					for (i = 1; i <= n; i++)
					{
						ix = (i - 1)*n + kk - 1; iy = (i - 1)*n + kk;
						d = v[ix]; v[ix] = v[iy]; v[iy] = d;
					}
				if (kk < m)
					for (i = 1; i <= m; i++)
					{
						ix = (i - 1)*m + kk - 1; iy = (i - 1)*m + kk;
						d = u[ix]; u[ix] = u[iy]; u[iy] = d;
					}
				kk = kk + 1;
			}
			it = 60;
			mm = mm - 1;
		}
		else
		{
			ks = mm;
			while ((ks > kk) && (fabs(s[ks - 1]) != 0.0))
			{
				d = 0.0;
				if (ks != mm) d = d + (float)fabs(e[ks - 1]);
				if (ks != kk + 1) d = d + (float)fabs(e[ks - 2]);
				dd = (float)fabs(s[ks - 1]);
				if (dd > eps*d) ks = ks - 1;
				else s[ks - 1] = 0.0;
			}
			if (ks == kk)
			{
				kk = kk + 1;
				d = (float)fabs(s[mm - 1]);
				t = (float)fabs(s[mm - 2]);
				if (t > d) d = t;
				t = (float)fabs(e[mm - 2]);
				if (t > d) d = t;
				t = (float)fabs(s[kk - 1]);
				if (t > d) d = t;
				t = (float)fabs(e[kk - 1]);
				if (t > d) d = t;
				sm = s[mm - 1] / d; sm1 = s[mm - 2] / d;
				em1 = e[mm - 2] / d;
				sk = s[kk - 1] / d; ek = e[kk - 1] / d;
				b = ((sm1 + sm)*(sm1 - sm) + em1*em1) / 2.0f;
				c = sm*em1; c = c*c; shh = 0.0;
				if ((b != 0.0) || (c != 0.0))
				{
					shh = (float)sqrt(b*b + c);
					if (b < 0.0) shh = -shh;
					shh = c / (b + shh);
				}
				fg[0] = (sk + sm)*(sk - sm) - shh;
				fg[1] = sk*ek;
				for (i = kk; i <= mm - 1; i++)
				{
					sss(fg, cs);
					if (i != kk) e[i - 2] = fg[0];
					fg[0] = cs[0] * s[i - 1] + cs[1] * e[i - 1];
					e[i - 1] = cs[0] * e[i - 1] - cs[1] * s[i - 1];
					fg[1] = cs[1] * s[i];
					s[i] = cs[0] * s[i];
					if ((cs[0] != 1.0) || (cs[1] != 0.0))
						for (j = 1; j <= n; j++)
						{
							ix = (j - 1)*n + i - 1;
							iy = (j - 1)*n + i;
							d = cs[0] * v[ix] + cs[1] * v[iy];
							v[iy] = -cs[1] * v[ix] + cs[0] * v[iy];
							v[ix] = d;
						}
					sss(fg, cs);
					s[i - 1] = fg[0];
					fg[0] = cs[0] * e[i - 1] + cs[1] * s[i];
					s[i] = -cs[1] * e[i - 1] + cs[0] * s[i];
					fg[1] = cs[1] * e[i];
					e[i] = cs[0] * e[i];
					if (i < m)
						if ((cs[0] != 1.0) || (cs[1] != 0.0))
							for (j = 1; j <= m; j++)
							{
								ix = (j - 1)*m + i - 1;
								iy = (j - 1)*m + i;
								d = cs[0] * u[ix] + cs[1] * u[iy];
								u[iy] = -cs[1] * u[ix] + cs[0] * u[iy];
								u[ix] = d;
							}
				}
				e[mm - 2] = fg[0];
				it = it - 1;
			}
			else
			{
				if (ks == mm)
				{
					kk = kk + 1;
					fg[1] = e[mm - 2]; e[mm - 2] = 0.0;
					for (ll = kk; ll <= mm - 1; ll++)
					{
						i = mm + kk - ll - 1;
						fg[0] = s[i - 1];
						sss(fg, cs);
						s[i - 1] = fg[0];
						if (i != kk)
						{
							fg[1] = -cs[1] * e[i - 2];
							e[i - 2] = cs[0] * e[i - 2];
						}
						if ((cs[0] != 1.0) || (cs[1] != 0.0))
							for (j = 1; j <= n; j++)
							{
								ix = (j - 1)*n + i - 1;
								iy = (j - 1)*n + mm - 1;
								d = cs[0] * v[ix] + cs[1] * v[iy];
								v[iy] = -cs[1] * v[ix] + cs[0] * v[iy];
								v[ix] = d;
							}
					}
				}
				else
				{
					kk = ks + 1;
					fg[1] = e[kk - 2];
					e[kk - 2] = 0.0;
					for (i = kk; i <= mm; i++)
					{
						fg[0] = s[i - 1];
						sss(fg, cs);
						s[i - 1] = fg[0];
						fg[1] = -cs[1] * e[i - 1];
						e[i - 1] = cs[0] * e[i - 1];
						if ((cs[0] != 1.0) || (cs[1] != 0.0))
							for (j = 1; j <= m; j++)
							{
								ix = (j - 1)*m + i - 1;
								iy = (j - 1)*m + kk - 2;
								d = cs[0] * u[ix] + cs[1] * u[iy];
								u[iy] = -cs[1] * u[ix] + cs[0] * u[iy];
								u[ix] = d;
							}
					}
				}
			}
		}
	}

	free(s); free(e); free(w);
	return(1);

}

void ppp(float a[], float e[], float s[], float v[], int m, int n)
{
	int i, j, p, q;
	float d;
	if (m >= n)
		i = n;
	else
		i = m;
	for (j = 1; j <= i - 1; j++)
	{
		int temp = (j - 1)*n + j;
		a[temp - 1] = s[j - 1];
		a[temp] = e[j - 1];
	}
	a[(i - 1)*n + i - 1] = s[i - 1];
	if (m < n)
		a[(i - 1)*n + i] = e[i - 1];
	for (i = 1; i <= n - 1; i++)
	{
		for (j = i + 1; j <= n; j++)
		{
			p = (i - 1)*n + j - 1;
			q = (j - 1)*n + i - 1;
			d = v[p];
			v[p] = v[q];
			v[q] = d;
		}
	}
	return;
}

void sss(float fg[], float cs[])
{
	float r, d;
	if ((fabs(fg[0]) + fabs(fg[1])) == 0.0)
	{
		cs[0] = 1.0;
		cs[1] = 0.0;
		d = 0.0;
	}
	else
	{
		d = sqrt(fg[0] * fg[0] + fg[1] * fg[1]);
		if (fabs(fg[0]) > fabs(fg[1]))
		{
			d = fabs(d);
			if (fg[0] < 0.0)
				d = -d;
		}
		if (fabs(fg[1]) >= fabs(fg[0]))
		{
			d = fabs(d);
			if (fg[1] < 0.0)
				d = -d;
		}
		cs[0] = fg[0] / d;
		cs[1] = fg[1] / d;
	}
	r = 1.0;
	if (fabs(fg[0]) > fabs(fg[1]))
		r = cs[1];
	else if (cs[0] != 0.0)
		r = 1.0f / cs[0];
	fg[0] = d;
	fg[1] = r;
	return;
}

/**
 * 计算椭圆均差
 */
float Circles::computeEllipseVariance(const vector<Point> points, const ellipseContent elli)
{
	/*计算方法：根据公式：x=acos@ y=bsin@ @等价于xtheta(点与圆心的连线相对于a,b坐标轴的夹角)
	 *
	 *
	 */
	int size = points.size();
	float d[65535],//拟合点到圆心距离
		diff[65535],//拟合点到圆心距离 - 对应椭圆上点到圆心距离
		ctheta[65535];//拟合点相对于a,b坐标轴的角度
	float sum = 0;

	for (int i = 0; i < size; i++)
	{
		float x = points[i].x, y = points[i].y;
		float dx = x - elli.x, dy = y - elli.y;
		d[i] = sqrt(dx*dx + dy*dy);
		ctheta[i] = cvFastArctan(dy, dx);
		ctheta[i] = ctheta[i] - elli.xtheta;
		float cx = elli.a*cos(ctheta[i]),
			cy = elli.b*sin(ctheta[i]);
		diff[i] = d[i] - sqrt(cx*cx + cy*cy);
		sum += diff[i] * diff[i];
	}

	sum /= size - 1;
	return sqrt(sum);
}

/**
 *计算圆
 */
void Circles::getCircle()
{
	if (contour.size() <= 0)//获取contours
	{
		getContours();
	}
	int n = contour.size();
	for (int i = 0; i < n; i++)
	{
		vector<Point> points = contour.at(i);
		if (hasRepeat(points, ellipseContour)) continue;
		if (points.size() >= minThreshold)
		{
			circleContent circon = circleLeastFit(points);
			if (computeVariance(points, circon) <= 1.5f)
			{
				if (points.size() >= circleNumberThreshold) {
					circ.push_back(circon);//将圆信息保存在circ中
					//将圆的轮廓信息保存在circleContour中
					circleContour.push_back(points);
				}
				else
				{//加入圆弧候选
					circleArcContent cirAc;
					cirAc.circ = circon;
					cirAc.points = points;
					cirAc.flag = false;
					circleArcs.push_back(cirAc);
				}
			}
		}
	}
}

/**
 *计算椭圆
 */
void Circles::getEllipse()
{
	if (contour.size() <= 0)//获取contours
	{
		getContours();
	}
	int n = contour.size();
	for (int i = 0; i < n; i++)
	{
		vector<Point> points = contour.at(i);
		if (hasRepeat(points, circleContour)) continue;
		if (points.size() >= minThreshold)
		{
			ellipseContent ellicon = ellipseLeastFit(points);
			float varianceValue = computeEllipseVariance(points, ellicon);
			if (varianceValue <= 1.5f)
			{
				if (points.size() >= ellipseNumberThreshold) {
					elli.push_back(ellicon);//将圆信息保存在circ中
					//将圆的轮廓信息保存在ellipseContour中
					ellipseContour.push_back(points);
				}
				else
				{//加入椭圆弧候选
					ellipseArcContent elliAc;
					elliAc.elli = ellicon;
					elliAc.points = points;
					elliAc.flag = false;
					ellipseArcs.push_back(elliAc);
				}
			}
		}
	}
}

/**
 * 轮廓已经被排除
 */
bool Circles::hasRepeat(const vector<Point> points, const vector<vector<Point>> contour)
{
	if (contour.size() <= 0) return false;
	int n = contour.size();
	for (int i = 0; i < n; i++)
	{
		if (points == contour.at(i))
			return true;
	}
	return false;
}

/**
 * 获取污点
 */
void Circles::getSpot()
{
	//判断轮廓是否已经获取
	if (contour.size() <= 0 || (circleContour.size() <= 0 && ellipseContour.size() <= 0))
	{
		return;
	}
	int size = contour.size();

	for (int i = 0; i < size; i++)
	{
		vector<Point> points = contour.at(i);
		if (points.size() > spotNumberThreshold) {
			//排除圆和椭圆轮廓
			if (hasRepeat(points, circleContour))
				continue;
			if (hasRepeat(points, ellipseContour))
				continue;
			if (hasRepeat(points, circleArcContour))
				continue;
			if (hasRepeat(points, ellipseArcContour))
				continue;
			//将其它轮廓添加到污点轮廓
			spotContour.push_back(points);
		}
	}
}

/**
 * 绘制圆
 */
void Circles::drawCircle(Mat &img)
{
	if (circ.size() <= 0)
	{
		return;
	}
	for (int i = 0; i < circ.size(); i++)
	{
		Point center(cvRound(circ[i].x), cvRound(circ[i].y));
		int radius = cvRound(circ[i].r);
		circle(img, center, 3, Scalar(237, 62, 62), -1, 8, 0);//圆心
		circle(img, center, radius, Scalar(255, 0, 0), 1);//圆边
	}
}

/**
 * 绘制椭圆
 */
void Circles::drawEllipse(Mat &img)
{
	if (elli.size() <= 0)
	{
		return;
	}
	for (int i = 0; i < elli.size(); i++)
	{
		Point center(cvRound(elli[i].x), cvRound(elli[i].y));
		Size size(cvRound(elli[i].a), cvRound(elli[i].b));
		ellipse(img, center, size, elli[i].xtheta, 0, 360, Scalar(0, 0, 255), 1);
	}
}

/**
 * 绘制污点，用矩形将其框起来
 */
void Circles::drawSpot(Mat &img)
{
	if (spotContour.size() <= 0)
	{
		return;
	}
	for (int i = 0; i < spotContour.size(); i++)
	{
		vector<Point> contours_ploly;
		Rect boundRect;
		approxPolyDP(Mat(spotContour[i]), contours_ploly, 3, true);
		boundRect = boundingRect(Mat(contours_ploly));
		rectangle(img, boundRect.tl(), boundRect.br(), Scalar(255, 255, 0), 2);
	}
}

/**
 * 通过圆弧获取圆
 */
void Circles::getCircleFromArc()
{
	if (circleArcs.size() <= 0)
		return;

	int size = circleArcs.size();
	for (int i = 0; i < size; i++)
	{
		if (!circleArcs[i].flag)
		{
			vector<Point> points;
			circleContent cir;
			for (int j = i; j < size; j++)
			{
				if (!circleArcs[j].flag && !hasRepeat(circleArcs[j].points, ellipseContour))//如果没有被使用过
				{
					if (points.size() == 0) {
						points = circleArcs[j].points;
						cir = circleArcs[j].circ;
						circleArcs[j].flag = true;
						continue;
					}
					//比较后来的圆弧和第一个圆弧是否在一个误差区间内
					int rs = cir.r;//标准弧的半径
					int rt = rs* 0.25f,//误差圆心范围
						rArcMin = rs* 0.75f,//误差半径范围
						rArcMax = rs*1.25f;
					circleContent cc = circleArcs[j].circ;
					float ds = computeDistance(Point(cc.x, cc.y), cir);
					if (ds <= rt)
					{
						float dt = fabs(cc.r - ds);
						if (dt >= rArcMin && dt <= rArcMax)
						{
							vector<Point> pp = circleArcs[j].points;
							if (hasRepeat(points, circleArcContour))//如果没有加入第一个弧，则加入
								circleArcContour.push_back(points);
							circleArcContour.push_back(pp);
							for (auto k = 0; k < pp.size(); k++)
							{
								points.push_back(pp[k]);
							}
							circleArcs[j].flag = true;
						}
					}
				}
			}
			if (points.size() >= circleNumberThreshold) {
				circleContent circon = circleLeastFit(points);
				if (computeVariance(points, circon) <= 1.5f)
				{
					circ.push_back(circon);//将圆信息保存在circ中
				}
			}
		}
	}
}

/**
 * 通过椭圆弧获取圆
 */
void Circles::getEllipseFromArc()
{
	if (ellipseArcs.size() <= 0)
		return;

	int size = ellipseArcs.size();
	for (int i = 0; i < size; i++)
	{
		if (!ellipseArcs[i].flag)
		{
			vector<Point> points;
			ellipseContent ellipseCon;
			for (int j = i; j < size; j++)
			{
				if (!ellipseArcs[j].flag && !hasRepeat(ellipseArcs[j].points, circleContour)
					&& !hasRepeat(ellipseArcs[j].points, circleArcContour))//如果没有被使用过
				{
					if (points.size() == 0) {
						points = ellipseArcs[j].points;
						ellipseCon = ellipseArcs[j].elli;
						ellipseArcs[j].flag = true;
						continue;
					}
					//比较后来的椭圆弧和第一个椭圆弧是否在一个误差区间内
					int ra = ellipseCon.a;//标准弧的半径
					int rt = ra* 0.5f,//误差圆心范围
						rArcMin = rt,//误差半径范围
						rArcMax = ra*1.5f;
					ellipseContent ec = ellipseArcs[j].elli;
					float dx = ec.x - ellipseCon.x, dy = ec.y - ellipseCon.y;
					float ds = sqrt(dx*dx + dy*dy);
					if (ds <= rt)
					{
						float dt = fabs(ellipseCon.a - ds);
						if (dt >= rArcMin && dt <= rArcMax)
						{
							vector<Point> pp = ellipseArcs[j].points;
							if (hasRepeat(points, ellipseArcContour))//如果没有加入第一个弧，则加入
								ellipseArcContour.push_back(points);
							ellipseArcContour.push_back(pp);
							for (auto k = 0; k < pp.size(); k++)
							{
								points.push_back(pp[k]);
							}
							ellipseArcs[j].flag = true;
						}
					}
				}
			}
			if (points.size() >= ellipseNumberThreshold) {
				ellipseContent ellicon = ellipseLeastFit(points);
				if (computeEllipseVariance(points, ellicon) <= 1.5f)
				{
					elli.push_back(ellicon);//将圆信息保存在circ中
				}
			}
		}
	}
}
