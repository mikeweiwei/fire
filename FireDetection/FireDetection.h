
// FireDetection.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFireDetectionApp:
// �йش����ʵ�֣������ FireDetection.cpp
//

class CFireDetectionApp : public CWinApp
{
public:
	CFireDetectionApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFireDetectionApp theApp;