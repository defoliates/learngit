
// ShemiXitong.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CShemiXitongApp:
// �йش����ʵ�֣������ ShemiXitong.cpp
//

class CShemiXitongApp : public CWinApp
{
public:
	CShemiXitongApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CShemiXitongApp theApp;