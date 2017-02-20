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

void EdgeDrawing::GenerateGradientAndDirection(Mat image)//����G��D
{
	

}

void EdgeDrawing::GenerateAnchor()//����ê��
{
	
}

bool EdgeDrawing::IsAnchor(int x, int y)//�Ƿ�ê��
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

void EdgeDrawing::LinkAnchor()//����ê��
{
	
}

void EdgeDrawing::GoLeft(int x, int y)//������
{
	
}

void EdgeDrawing::GoRight(int x, int y)//������
{
	
}

void EdgeDrawing::GoUp(int x, int y)//������
{
	
}

void EdgeDrawing::GoDown(int x, int y)//������
{
	
}

int EdgeDrawing::GetPoint(int x, int y, Mat image)//��ȡĳ������ֵ
{
	int nr = image.rows;
	int nc = image.cols*image.channels();
	uchar* data = image.ptr<uchar>(x);
	return data[y];
}