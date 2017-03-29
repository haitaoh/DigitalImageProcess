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

int Circles::muav(float a[], int m, int n, float u[], float v[], float eps, int ka)
{
	int i, j, k, l, it, ll, kk, ix, iy, mm, nn, iz, ml, ks;
	float d, dd, t, sm, sml, eml, sk, ek, b, c, shh, fg[2], cs[2];
	float *s = new float[ka],
		*e = new float[ka], 
		*w = new float[ka];

	void ppp(float a[], float e[], float s[], float v[], int m, int n);
	void sss(float fg[], float cs[]);


	it = 60;
	k = n;

	if(m-1 < n)
	{
		k = m - 1;
	}
	l = m;
	if(n-2 <m)
	{
		l = n - 2;
	}
	if(l < 0)
	{
		l = 0;
	}
	ll = k;
	if(l > k)
	{
		ll = 1;
	}
	if(ll >= 1)
	{
		for(kk = 1;kk <=ll;kk++)
		{
			if(kk <= k)
			{
				d = 0.0;
				for(i = kk;i <=m;i++)
				{
					ix = (i - 1)*n + kk - 1;
					d += a[ix] * a[ix];
				}
				s[kk - 1] = sqrt(d);
				if(s[kk-1] != 0)
				{
					ix = (kk - 1)*n + kk - 1;
					if(a[ix] != 0.0)
					{
						s[kk - 1] = fabs(s[kk - 1]);
						if(a[ix] <0.0)
						{
							s[kk - 1] = -s[kk - 1];
						}
					}
					for(i = kk;i <= m;i++)
					{
						iy = (i - 1)*n + kk - 1;
						a[iy] /= s[kk - 1];
					}
					a[ix] = 1.0f + a[ix];
				}
				s[kk - 1] = -s[kk - 1];
			}
			if(n >= kk+1)
			{
				for(j = kk+1;j <= n;j++)
				{
					if((kk<=k)&&(s[kk-1]!=0.0))
					{
						d = 0.0;
						for(i = kk;i <= m;i ++)
						{
							ix = (i - 1)*n + kk - 1;
							iy = (i - 1)*n + j - 1;
							d += a[ix] * a[iy];
						}
						d /= -a[(kk - 1)*n + kk - 1];
						for(i = kk;i <= m;i++)
						{
							ix = (i - 1)*n + j - 1;
							iy = (i - 1)*n + kk - 1;
							a[ix] += d*a[iy];
						}
					}
					e[j - 1] = a[(kk - 1)*n + j - 1];
				}
			}
			if(kk <= k)
			{
				for(i = kk;i <= m;i++)
				{
					ix = (i - 1)*m + kk - 1;
					iy = (i - 1)*n + kk - 1;
					u[ix] = a[iy];
				}
			}
			if(kk <= 1)
			{
				d = 0.0;
				for (i = kk + 1; i <= n; i++)
				{
					d += e[i - 1] * e[i - 1];
				}
				e[kk - 1] = sqrt(d);
				if(e[kk - 1] != 0.0)
				{
					if(e[kk] != 0)
					{
						e[kk - 1] = fabs(e[kk - 1]);
						if(e[kk] < 0.0)
						{
							e[kk - 1] = -e[kk - 1];
						}
					}
					for(i = kk+1;i <= n;i++)
					{
						e[i - 1] /= e[kk - 1];
					}
					e[kk] = 1.0f + e[kk];
				}
				e[kk - 1] = -e[kk - 1];
				if((kk+1<m) && (e[kk-1]!= 0.0))
				{
					for(i = kk+1;i <= m;i++)
					{
						w[i - 1] = 0.0;
					}
					for(j = kk+1; j <= n;j++)
					{
						for(i = kk+1;i <= m;i ++)
						{
							w[i - 1] += e[j - 1] * a[(i - 1)*n + j - 1];
						}
					}
					for(j = kk+1;j <= n;j++)
					{
						for(i = kk+1; i <= m;i ++)
						{
							ix = (i - 1)*n + j - 1;
							a[ix] -= w[i - 1] * e[j - 1] / e[kk];
						}
					}
				}
				for(i = kk+1;i <= n;i ++)
				{
					v[(i - 1)*n + kk - 1] = e[i - 1];
				}
			}
		}
	}

	mm = n;
	if (m + 1 < n)
		mm = m + 1;
	if (k < n)
		s[k] = a[k*n + k];
	if (m < mm)
		s[mm - 1] = 0.0;
	if (l + 1 < mm)
		e[l] = a[l*n + mm - 1];
	e[mm - 1] = 0.0;
	nn = m;
	if (m > n)
		nn = n;
	if(nn >= kk+1)
	{
		for(j = k+1;j <= nn;j++)
		{
			for (i = 1; i <= m; i++)
				u[(i - 1)*m + j - 1] = 0.0;
			u[(j - 1)*m + j - 1] = 1.0;
		}
	}
	if(k >= 1)
	{
		for(ll = 1;ll <= k;ll ++)
		{
			kk = k - ll + 1;
			iz = (kk - 1)*m + kk - 1;
			if(s[kk-1]!=0.0)
			{
				if(nn >= kk+1)
				{
					for(j = kk+1;j <= nn;j ++)
					{
						d = 0.0;
						for(i = kk;i <= m;i++)
						{
							ix = (i - 1)*m + kk - 1;
							iy = (i - 1)*m + j - 1;
							d += u[ix] * u[iy] / u[iz];
						}
						d = -d;
						for(i = kk;i <= m;i++)
						{
							ix = (i - 1)*m + j - 1;
							iy = (i - 1)*m + kk - 1;
							u[ix] += d*u[iy];
						}

					}
				}
				for(i = kk;i <= m;i++)
				{
					ix = (i - 1)*m + kk - 1;
					u[ix] = -u[ix];
				}
				u[iz] = 1.0f + u[iz];
				if(kk-1 >= 1)
				{
					for (i = 1; i <= kk - 1; i++)
						u[(i - 1)*m + kk - 1] = 0.0;
				}
			}
			else
			{
				for (i = 1; i <= m; i++)
					u[(i - 1)*m + kk - 1] = 0.0;
				u[(kk - 1)*m + kk - 1] = 1.0;
			}
		}
	}

	for(ll = 1;ll <= n;ll++)
	{
		kk = n - ll + 1;
		iz = kk*n + kk - 1;
		if((kk<=1)&&(e[kk-1]!=0.0))
		{
			for(j = kk+1;j <= n;j++)
			{
				d = 0.0;
				for(i = kk+1;i <= n;i++)
				{
					ix = (i - 1)*n + kk - 1;
					iy = (i - 1)*n + j - 1;
					d += v[ix] * v[iy] / v[iz];
				}
				d = -d;
				for(i = kk+1;i <= n;i++)
				{
					ix = (i - 1)*n + j - 1;
					iy = (i - 1)*n + kk - 1;
					v[ix] += d*v[iy];
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

	ml = mm;
	it = 60;

	while(true)
	{
		if(mm == 0)
		{
			ppp(a, e, s, v, m, n);
			free(s);
			free(e);
			free(w);
			return 1;
		}
		if(it == 0)
		{
			ppp(a, e, s, v, m, n);
			free(s);
			free(e);
			free(w);
			return -1;
		}
		kk = mm - 1;
		while((kk!=0) && (fabs(e[kk-1])!=0.0))
		{
			d = fabs(s[kk - 1]) + fabs(s[kk]);
			dd = fabs(e[kk - 1]);
			if (dd > eps*d)
				kk --;
			else
				e[kk - 1] = 0.0;
		}
		if(kk == mm-1)
		{
			kk++;
			if(s[kk-1]<0.0)
			{
				s[kk - 1] = -s[kk - 1];
				for(i = 1;i <= n;i++)
				{
					ix = (i - 1)*n + kk - 1;
					v[ix] = -v[ix];
				}
			}
			while((kk!=ml) && (s[kk-1]<s[kk]))
			{
				d = s[kk - 1];
				s[kk - 1] = s[kk];
				s[kk] = d;
				if(kk < n)
				{
					for(i = 1;i <= n;i++)
					{
						ix = (i - 1)*n + kk - 1;
						iy = ix + 1;
						d = v[ix];
						v[ix] = v[iy];
						v[iy] = d;
					}
				}
				if(kk < m)
				{
					for(i = 1;i <= m;i++)
					{
						ix = (i - 1)*m + kk - 1;
						iy = ix + 1;
						d = u[ix];
						u[ix] = u[iy];
						u[iy] = d;
					}
				}
				kk++;
			}
			it = 60;
			mm--;
		}else
		{
			ks = mm;
			while((ks >kk) && (fabs(s[ks-1])!=0.0))
			{
				d = 0.0;
				if (ks != mm)
					d += fabs(e[ks - 1]);
				if (ks != kk + 1)
					d += fabs(e[ks - 2]);
				dd = fabs(s[ks - 1]);
				if (dd > eps*d)
					ks--;
				else
					s[ks - 1] = 0.0;
			}
			if(ks == kk)
			{
				kk++;
				d = fabs(s[mm - 1]);
				t = fabs(s[mm - 2]);
				if (t > d)
					d = t;
				t = fabs(e[mm - 2]);
				if (t > d)
					d = t;
				t = fabs(s[kk - 1]);
				if (t > d)
					d = t;
				t = fabs(e[kk - 1]);
				if (t > d)
					d = t;
				sm = s[mm - 1] / d;
				sml = s[mm - 2] / d;
				eml = e[mm - 2] / d;
				sk = s[kk - 1] / d;
				ek = e[kk - 1] / d;
				b = ((sml + sm)*(sml - sm) + eml*eml) / 2.0f;
				c = sm*eml;
				c *= c;
				shh = 0.0;
				if((b !=0.0) ||(c!=0.0))
				{
					shh = sqrt(b*b + c);
					if (b < 0.0)
						shh = -shh;
					shh = c / (b + shh);
				}
				fg[0] = (sk + sm)*(sk - sm) - shh;
				fg[1] = sk*ek;
				for(i = kk;i <= mm-1;i++)
				{
					sss(fg, cs);
					if (i != kk)
						e[i - 2] = fg[0];
					fg[0] = cs[0] * s[i - 1] + cs[1] * e[i - 1];
					e[i - 1] = cs[0] * e[i - 1] - cs[1] * s[i - 1];
					fg[1] = cs[1] * s[i];
					s[i] = cs[0] * s[i];
					if((cs[0]!=1.0) || (cs[1]!=0.0))
					{
						for(j = 1;j <= n;j++)
						{
							ix = (j - 1)*n + i - 1;
							iy = ix + 1;
							d = cs[0] * v[ix] + cs[1] * v[iy];
							v[iy] = -cs[1] * v[ix] + cs[0] * v[iy];
							v[ix] = d;
						}
					}
					sss(fg, cs);
					s[i - 1] = fg[0];
					fg[0] = cs[0] * e[i - 1] + cs[1] * s[i];
					s[i] = -cs[1] * e[i - 1] + cs[0] * s[i];
					fg[1] = cs[1] * e[i];
					e[i] = cs[0] * e[i];
					if(i < m)
					{
						if((cs[0]!=1.0) ||(cs[1]!=0.0))
						{
							for(j = 1;j <= m;j++)
							{
								ix = (j - 1)*m + i - 1;
								iy = ix + 1;
								d = cs[0] * u[ix] + cs[1] * u[iy];
								u[iy] = -cs[1] * u[ix] + cs[0] * u[iy];
								u[ix] = d;
							}
						}
					}
				}
				e[mm - 2] = fg[0];
				it = it - 1;
			}else
			{
				if(ks == mm)
				{
					kk++;
					fg[1] = e[mm - 2];
					e[mm - 2] = 0.0;
					for(ll = kk;ll < mm;ll++)
					{
						i = mm + kk - ll - 1;
						fg[0] = s[i - 1];
						sss(fg, cs);
						s[i - 1] = fg[0];
						if(i != kk)
						{
							fg[1] = -cs[1] * e[i - 2];
							e[i - 2] = cs[0] * e[i - 2];
						}
						if((cs[0] !=1.0) ||(cs[1]!=0.0))
						{
							for(j = 1;j <= n;j++)
							{
								ix = (j - 1)*n + i - 1;
								iy = (j - 1)*n + mm - 1;
								d = cs[0] * v[ix] + cs[1] * v[iy];
								v[iy] = -cs[1] * v[ix] + cs[0] * v[iy];
								v[ix] = d;
							}
						}
					}
				}else
				{
					kk = ks + 1;
					fg[1] = e[kk - 2];
					e[kk - 2] = 0.0;
					for(i = kk;i <= mm;i ++)
					{
						fg[0] = s[i - 1];
						sss(fg, cs);
						s[i - 1] = fg[0];
						fg[1] = -cs[1] * e[i - 1];
						e[i - 1] = cs[0] * e[i - 1];
						if((cs[0]!=1.0)||(cs[1]!=0.0))
						{
							for(j = 1;j <= m;j++)
							{
								ix = (j - 1)*m + i - 1;
								iy = (j - 1)*mm + kk - 2;
								d = cs[0] * u[ix] + cs[1] * u[iy];
								u[iy] = -cs[1] * u[ix] + cs[0] * u[iy];
								u[ix] = d;
							}
						}
					}
				}
			}
		}
	}
	
	delete []s;
	delete []e;
	delete []w;

	return 1;

}

void ppp(float a[], float e[], float s[], float v[], int m, int n)
{
	int i, j, p, q;
	float d;
	if (m >= n)
		i = n;
	else
		i = m;
	for(j = 1;j <= i-1;j++)
	{
		int temp = (j - 1)*n + j;
		a[temp - 1] = s[j - 1];
		a[temp] = e[j - 1];
	}
	a[(i - 1)*n + i - 1] = s[i - 1];
	if (m < n)
		a[(i - 1)*n + i] = e[i - 1];
	for(i = 1;i <= n-1;i++)
	{
		for(j = i+1;j <= n;j++)
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

void sss(float fg[],float cs[])
{
	float r, d;
	if((fabs(fg[0])+fabs(fg[1])) == 0.0)
	{
		cs[0] = 1.0;
		cs[1] = 0.0;
		d = 0.0;
	}else
	{
		d = sqrt(fg[0] * fg[0] + fg[1] * fg[1]);
		if(fabs(fg[0])>fabs(fg[1]))
		{
			d = fabs(d);
			if (fg[0] < 0.0)
				d = -d;
		}
		if(fabs(fg[1])>=fabs(fg[0]))
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
	
	return 1.0f;
}

/**
 *计算圆
 */
void Circles::getCircle()
{
	if(contour.size() <= 0)//获取contours
	{
		getContours();
	}
	int n = contour.size();
	for(int i = 0;i < n;i++)
	{
		vector<Point> points = contour.at(i);
		if (hasRepeat(points,ellipseContour)) continue;
		if(points.size() >= circleNumberThreshold)
		{
			circleContent circon = circleLeastFit(points);
			if(computeVariance(points,circon) <= 1.5)
			{
				circ.push_back(circon);//将圆信息保存在circ中
				//将圆的轮廓信息保存在circleContour中
				circleContour.push_back(points);
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
		if (hasRepeat(points,circleContour)) continue;
		if (points.size() >= circleNumberThreshold)
		{
			ellipseContent ellicon = ellipseLeastFit(points);
			if (computeEllipseVariance(points, ellicon) <= 1.5)
			{
				elli.push_back(ellicon);//将圆信息保存在circ中
				//将圆的轮廓信息保存在ellipseContour中
				ellipseContour.push_back(points);
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
	for(int i = 0;i < n;i++)
	{
		if (points == contour.at(i))
			return true;
	}
	return false;
}