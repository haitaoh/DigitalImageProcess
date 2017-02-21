#include"EdgeDrawing.h"

EdgeDrawing::EdgeDrawing()
{
	Horizon = 0, Vertical = 1, Color = 255;
}

EdgeDrawing::~EdgeDrawing()
{
	
}

void EdgeDrawing::SetGradientThreshold(int gradientThreshold)
{
	GradientThreshold = gradientThreshold;
}

void EdgeDrawing::SetAnchorThreshold(int anchorThreshold)
{
	AnchorThreshold = anchorThreshold;
}

void EdgeDrawing::SetE(Mat image)
{
	cv::cvtColor(image, image, CV_BGR2GRAY);//转换成灰度图
	E = image.zeros;
}

void EdgeDrawing::SetHorizon(int horizon)
{
	Horizon = horizon;
}

void EdgeDrawing::SetVertical(int vertical)
{
	Vertical = vertical;
}

void EdgeDrawing::SetColor(int color)
{
	if(color > 255 || color <= 0)
	{
		Color = 255;
	}else
	{
		Color = color;
	}
}

//计算G和D
void EdgeDrawing::GenerateGradientAndDirection(Mat image)
{
	if(E.empty())
	{
		SetE(image);
	}
	Mat imageGray,Gx,Gy;
	cv::cvtColor(image, imageGray, CV_BGR2GRAY);//转换成灰度图
	cv::GaussianBlur(imageGray, imageGray, cv::Size(5, 5), 1, 1);//高斯模糊，降噪处理
	G = imageGray.zeros,D = imageGray.zeros;

	//计算x,y方向上的导数
	Sobel(imageGray, Gx, CV_16S, 1, 0);
	Sobel(imageGray, Gy, CV_16S, 0, 1);

	int nr = imageGray.rows, nc = imageGray.cols*imageGray.channels();
	for(int i = 0;i < nr;i++)
	{
		uchar* dataGx = Gx.ptr<uchar>(i);
		uchar* dataGy = Gy.ptr<uchar>(i);
		uchar* dataG = G.ptr<uchar>(i);
		uchar* dataD = D.ptr<uchar>(i);
		for(int j = 0;j < nc;j++)
		{
			dataG[j] = sqrt(dataGx[j] * dataGx[j]+dataGy[j] * dataGy[j]);//计算G
			if(dataGx[j] >= dataGy[j])//计算D
			{
				dataD[j] = Vertical;
			}else
			{
				dataD[j] = Horizon;
			}
		}
	}

}

//计算锚点
void EdgeDrawing::GenerateAnchor()
{
	int nr = E.rows, nc = E.cols*E.channels();
	for(int i = 0;i < nr;i ++)
	{
		uchar* dataE = E.ptr<uchar>(i);
		for(int j = 0;j < nc;j++)
		{
			if(IsAnchor(i,j))
			{
				dataE[j] = AnchorColor;
			}
		}
	}
}

//是否锚点
bool EdgeDrawing::IsAnchor(int x, int y)
{
	if(GetPoint(x,y,D) == Horizon)
	{
		if(GetPoint(x,y,G) - GetPoint(x,y-1,G) >= AnchorThreshold
			&& GetPoint(x,y,G) - GetPoint(x,y+1,G) >= AnchorThreshold)
		{
			return true;
		}
	}else
	{
		if(GetPoint(x,y,G) - GetPoint(x-1,y,G) >= AnchorThreshold &&
			GetPoint(x,y,G) - GetPoint(x+1,y,G) >= AnchorThreshold)
		{
			return true;
		}
	}
	return false;
}

//连接锚点
void EdgeDrawing::LinkAnchor()
{
	int nr = E.rows, nc = E.cols*E.channels();
	for(int i = 0;i < nr;i++)
	{
		uchar* dataE = E.ptr<uchar>(i);
		uchar* dataD = D.ptr<uchar>(i);
		for(int j = 0;j < nc;j++)
		{
			if (dataE[j] == AnchorColor) {
				if (dataD[j] == Horizon)
				{
					GoLeft(i, j);
					GoRight(i, j);
				}
				else
				{
					GoUp(i, j);
					GoDown(i, j);
				}
			}
		}
	}
}

//向左走
void EdgeDrawing::GoLeft(int x, int y)
{
	while(GetPoint(x,y,G) > 0 && GetPoint(x,y,D) == Horizon
		&& GetPoint(x,y,E) != Color)
	{
		SetPoint(x, y, E, Color);
		if(GetPoint(x-1,y-1,G) > GetPoint(x-1,y,G) &&
			GetPoint(x-1,y-1,G) > GetPoint(x-1,y+1,G))
		{
			x = x - 1, y = y - 1;
		}else if(GetPoint(x-1,y+1,G) > GetPoint(x-1,y-1,G) &&
			GetPoint(x-1,y+1,G) > GetPoint(x-1,y,G))
		{
			x = x - 1, y = y + 1;
		}else
		{
			x = x - 1;
		}
	}
}

//向右走
void EdgeDrawing::GoRight(int x, int y)
{
	while (GetPoint(x, y, G) > 0 && GetPoint(x, y, D) == Horizon
		&& GetPoint(x, y, E) != Color)
	{
		SetPoint(x, y, E, Color);
		if(GetPoint(x+1,y-1,G) > GetPoint(x+1,y,G) &&
			GetPoint(x+1,y-1,G) > GetPoint(x+1,y+1,G))
		{
			x = x + 1, y = y - 1;
		}else if(GetPoint(x+1,y+1,G) > GetPoint(x+1,y,G) &&
			GetPoint(x+1,y+1,G) > GetPoint(x+1,y-1,G))
		{
			x = x + 1, y = y + 1;
		}else
		{
			x = x + 1;
		}
	}
}

//向上走
void EdgeDrawing::GoUp(int x, int y)
{
	while (GetPoint(x, y, G) > 0 && GetPoint(x, y, D) == Vertical
		&& GetPoint(x, y, E) != Color)
	{
		SetPoint(x, y, E, Color);
		if (GetPoint(x - 1, y - 1, G) > GetPoint(x, y - 1, G) &&
			GetPoint(x - 1, y - 1, G) > GetPoint(x + 1, y - 1,G))
		{
			x = x - 1, y = y - 1;
		}else if(GetPoint(x+1,y-1,G) > GetPoint(x,y-1,G) &&
			GetPoint(x+1,y-1,G) > GetPoint(x-1,y-1,G))
		{
			x = x + 1, y = y - 1;
		}else
		{
			y = y - 1;
		}
	}
}

//向下走
void EdgeDrawing::GoDown(int x, int y)
{
	while (GetPoint(x, y, G) > 0 && GetPoint(x, y, D) == Vertical
		&& GetPoint(x, y, E) != Color)
	{
		SetPoint(x, y, E, Color);
		if(GetPoint(x-1,y+1,G) > GetPoint(x,y+1,G) &&
			GetPoint(x-1,y+1,G) > GetPoint(x+1,y+1,G))
		{
			x = x - 1, y = y + 1;
		}else if(GetPoint(x+1,y+1,G) > GetPoint(x,y+1,G) &&
			GetPoint(x+1,y+1,G) > GetPoint(x-1,y+1,G))
		{
			x = x + 1, y = y + 1;
		}else
		{
			y = y + 1;
		}
	}
}

/*
 *获取某点像素值*/
int EdgeDrawing::GetPoint(int x, int y, Mat image)
{
	int nr = image.rows;
	int nc = image.cols*image.channels();
	if (x < nr && x > -1) {
		uchar* data = image.ptr<uchar>(x);
		if (y < nc) return data[y];
		else return data[0];
	}
	return -1;
}

/*
 *设置某点像素值*/
void EdgeDrawing::SetPoint(int x,int y,Mat image,int value)
{
	int nr = image.rows;
	int nc = image.cols*image.channels();
	if (x < nr && x > -1) {
		uchar* data = image.ptr<uchar>(x);
		if (y < nc && y > -1)
			data[y] = value;
	}
}