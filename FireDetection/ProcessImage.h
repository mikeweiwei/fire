#include "stdafx.h"
#pragma once
class ProcessImage
{
public:
	ProcessImage(void);
	~ProcessImage(void);
	static IplImage* Process(IplImage* img);  //ͼ����
	static IplImage* ProcessVideo(IplImage* img);
};

