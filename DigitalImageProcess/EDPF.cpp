#include"EDPF.h"

EDPF::EDPF()
{
	GradientThreshold = 8.48;
	AnchorThreshold = 0;
}

EDPF::~EDPF()
{
	
}

//亥姆霍兹验证，用于去除不需要的边缘段
void EDPF::HelmholtzPrinciple()
{
	//数据结构不统一，无法获取完整的边缘段
	//对比度获取失败，无法计算NFA
	
	
}
