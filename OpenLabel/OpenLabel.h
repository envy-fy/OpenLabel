
// OpenLabel.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������



// COpenLabelApp:
// �йش����ʵ�֣������ OpenLabel.cpp
//

class COpenLabelApp : public CWinApp
{
public:
	COpenLabelApp();
	HWND m_hwndDlg; 

private:
	ULONG_PTR m_gdiplusToken;
	
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
};

extern COpenLabelApp theApp;