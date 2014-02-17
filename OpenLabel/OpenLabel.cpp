
// OpenLabel.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "OpenLabel.h"
#include "OpenLabelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenLabelApp

BEGIN_MESSAGE_MAP(COpenLabelApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// COpenLabelApp ����

COpenLabelApp::COpenLabelApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� COpenLabelApp ����

COpenLabelApp theApp;


// COpenLabelApp ��ʼ��

BOOL COpenLabelApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	Gdiplus::GdiplusStartupInput StartupInput;  
    GdiplusStartup(&m_gdiplusToken,&StartupInput,NULL); 

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	COpenLabelDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	m_hwndDlg =NULL; 

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int COpenLabelApp::ExitInstance()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);  
    return __super::ExitInstance(); 
}

BOOL COpenLabelApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_hwndDlg!=NULL)
    {
       //�ж���Ϣ�������Ϣ�ǴӶԻ��򷢳��Ļ������ӿؼ������ģ����Ǿͽ��д���
       if((lpMsg->hwnd==m_hwndDlg) || ::IsChild(m_hwndDlg,lpMsg->hwnd))
       {
          if(lpMsg->message==WM_KEYDOWN) //�����Ϣ��WM_KEYDOWN�����Ǿ͵���һ����Ϣ��
          {
              
          }
       }
    }
	return CWinApp::ProcessMessageFilter(code, lpMsg);
}
