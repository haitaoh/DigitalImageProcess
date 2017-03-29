#pragma once

#include<iostream>

#include "opencv\cv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "EDPF.h"
#include "EdgeLines.h"

class EDCircles : EDPF, EdgeLines
{
	EDCircles();
	~EDCircles();
public:

};
