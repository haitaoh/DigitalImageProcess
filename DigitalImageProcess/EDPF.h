#pragma once

#include<iostream>

#include "EdgeDrawing.h"

class EDPF : public EdgeDrawing
{
	EDPF();
	~EDPF();
public:
	void static HelmholtzPrinciple();//亥姆霍兹原则校验，去除不要的边缘段
};
