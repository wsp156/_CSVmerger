
// CSVmerger.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.



// CCSVmergerApp:
// �� Ŭ������ ������ ���ؼ��� CSVmerger.cpp�� �����Ͻʽÿ�.
//

class CCSVmergerApp : public CWinApp
{
public:
	CCSVmergerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};


extern CCSVmergerApp theApp;