#include"EdgeDrawing.h"

EdgeDrawing::EdgeDrawing()
{
	
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

void EdgeDrawing::GenerateGradientAndDirection(Mat image)//计算G和D
{
	

}

void EdgeDrawing::GenerateAnchor()//计算锚点
{
	
}

bool EdgeDrawing::IsAnchor(int x, int y)//是否锚点
{
	if(GetPoint(x,y,G) == Horizon)
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

void EdgeDrawing::LinkAnchor()//连接锚点
{
	
}

void EdgeDrawing::GoLeft(int x, int y)//向左走
{
	
}

void EdgeDrawing::GoRight(int x, int y)//向右走
{
	
}

void EdgeDrawing::GoUp(int x, int y)//向上走
{
	
}

void EdgeDrawing::GoDown(int x, int y)//向下走
{
	
}

int EdgeDrawing::GetPoint(int x, int y, Mat image)//获取某点像素值
{
	int nr = image.rows;
	int nc = image.cols*image.channels();
	uchar* data = image.ptr<uchar>(x);
	return data[y];
}