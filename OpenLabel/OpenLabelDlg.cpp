
// OpenLabelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpenLabel.h"
#include "OpenLabelDlg.h"
#include "afxdialogex.h"
#include "GetLabelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_LABEL_NUM           15


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COpenLabelDlg �Ի���




COpenLabelDlg::COpenLabelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpenLabelDlg::IDD, pParent)
	, m_pCapture(NULL)
	, m_iCurrentNum(0)
	, m_pLabelFile(NULL)
	, m_fWRatio(0)
	, m_fHRatio(0)
	, m_bDrawFlag(false)
	, m_pCurrentFrame(NULL)
	, m_bStarted(false)
	, m_iOffsetX(0)
	, m_iOffsetY(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenLabelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABLEFILEPATH, m_ctrlLabelFilePath);
	DDX_Control(pDX, IDC_START, m_ctrlStartLabel);
	DDX_Control(pDX, IDC_NEXT, m_ctrlNextFrame);
	DDX_Control(pDX, IDC_UNDO, m_ctrlUndo);
	DDX_Control(pDX, IDC_CLEARCURRENT, m_ctrlClear);
	DDX_Control(pDX, IDC_RESTART, m_ctrlRestart);
	DDX_Control(pDX, IDC_DONE, m_ctrlDone);
	DDX_Control(pDX, IDC_FRAMENUM, m_ctrlTextFrameNum);
}

BEGIN_MESSAGE_MAP(COpenLabelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EXPLORE, &COpenLabelDlg::OnBnClickedExplore)
	ON_BN_CLICKED(IDC_OPENFILE, &COpenLabelDlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(IDC_START, &COpenLabelDlg::OnBnClickedStart)
	ON_WM_CLOSE()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_NEXT, &COpenLabelDlg::OnBnClickedNext)
	ON_BN_CLICKED(IDC_UNDO, &COpenLabelDlg::OnBnClickedUndo)
	ON_BN_CLICKED(IDC_CLEARCURRENT, &COpenLabelDlg::OnBnClickedClearcurrent)
	ON_BN_CLICKED(IDC_RESTART, &COpenLabelDlg::OnBnClickedRestart)
	ON_BN_CLICKED(IDC_DONE, &COpenLabelDlg::OnBnClickedDone)
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// COpenLabelDlg ��Ϣ�������

BOOL COpenLabelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	((COpenLabelApp*)AfxGetApp())->m_hwndDlg =this->m_hWnd; 
	InitResource();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void COpenLabelDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COpenLabelDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR COpenLabelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int COpenLabelDlg::InitResource(void)
{
	m_pCapture = NULL;
	m_iCurrentNum = 0;
	m_pLabelFile = NULL;

	m_pCurrentFrame = NULL;

	m_bDrawFlag = false;

	return 0;
}


void COpenLabelDlg::OnBnClickedExplore()
{
	// TODO: Add your control notification handler code here
	CString strFilter = "Moive File (*.avi)|*.avi";
	CFileDialog dlgOpen(TRUE,NULL,NULL,OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, 
		strFilter, this);
	if (IDOK == dlgOpen.DoModal()) 
	{
		CString sourceFile;
		sourceFile = dlgOpen.GetPathName();
		HWND hWnd = NULL;
		GetDlgItem(IDC_FILEPATH)->SetWindowTextA(sourceFile);

		OnBnClickedOpenfile();

	}
}


void COpenLabelDlg::OnBnClickedOpenfile()
{
	// TODO: Add your control notification handler code here
	CString fileName;
	GetDlgItem(IDC_FILEPATH)->GetWindowText(fileName);

	if(fileName.GetLength() == 0)
	{
		MessageBox("Please select a file !");
		return ;
	}

	if(m_pCapture != NULL)
	{
		cvReleaseCapture(&m_pCapture);
	}
	//m_pCapture=cvCaptureFromFile(fileName.GetBuffer());
	if(m_pCapture = cvCaptureFromFile(fileName.GetBuffer()))
	{
		m_pCurrentFrame = cvQueryFrame(m_pCapture);

		CString strText;
		strText.Format("Frame No: %d",m_iCurrentNum);
		this->m_ctrlTextFrameNum.SetWindowTextA(strText);

		this->m_iCurrentNum = 0;
		this->m_iCurrentNum ++;


		HWND hWnd = NULL;
		GetDlgItem(IDC_FRAME,&hWnd);
		CString str;
		DisplayImage(hWnd,m_pCurrentFrame);

		this->m_ctrlStartLabel.EnableWindow();

		int pos = fileName.Find(".");
		fileName.Delete(pos+1,fileName.GetLength()-pos-1);
		fileName.Append("txt");
		this->m_ctrlLabelFilePath.SetWindowTextA(fileName);

		CRect rc;
		GetDlgItem(IDC_FRAME)->GetWindowRect(&rc);
		ScreenToClient(rc);

		long lWidth = rc.right - rc.left;
		long lHeight = rc.bottom - rc.top;

		m_iOffsetX = rc.left;
		m_iOffsetY = rc.top;

		m_fWRatio = (float)lWidth/(float)m_pCurrentFrame->width;
		m_fHRatio = (float)lHeight/(float)m_pCurrentFrame->height;
	}
	
}


void COpenLabelDlg::DisplayImage(HWND hWnd,IplImage* pImage)
{
	RECT rc;
	::GetWindowRect(hWnd, &rc);
	long lWidth = rc.right - rc.left;
	long lHeight = rc.bottom - rc.top;
    
	BITMAPINFOHEADER bih;

	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biHeight = -pImage->height;
	bih.biWidth = pImage->width;
	bih.biPlanes = 1;
    bih.biBitCount = 24;
	bih.biCompression = BI_RGB;

	HDC hdc = ::GetDC(hWnd);
	Gdiplus::Rect rect(0, 0, lWidth, lHeight);

	//�����ڴ��л�ͼ
	Bitmap memBit(lWidth, lHeight);
	Graphics memGraphics(&memBit);

	Bitmap image((BITMAPINFO*)&(bih), pImage->imageData);
	memGraphics.DrawImage(&image, rect);

	//����Ļ�ϻ�ͼ
	Graphics graphics(hdc);
	graphics.DrawImage(&memBit, rect);

	::ReleaseDC(hWnd, hdc);
}


void COpenLabelDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	CString strName;
	this->m_ctrlLabelFilePath.GetWindowText(strName);
	this->m_pLabelFile = fopen(strName,"wt+");

	this->m_ctrlNextFrame.EnableWindow();
	this->m_ctrlDone.EnableWindow();

	this->m_Boxes.clear();
	
	this->m_bStarted = true;

}

void COpenLabelDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(m_pCapture!=NULL)
		cvReleaseCapture(&m_pCapture);
	if(m_pLabelFile != NULL)
		fclose(m_pLabelFile);

	CDialogEx::OnClose();
}

void COpenLabelDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if( m_bStarted){

		CRect rc;
		GetDlgItem(IDC_FRAME)->GetWindowRect(&rc);
		ScreenToClient(rc);

		CPoint temp;

		temp.x = point.x -rc.right;
		temp.y = point.y -rc.bottom;

		if( temp.x > 0 || temp.y > 0)
			return;

		temp.x = point.x-rc.left;
		temp.y = point.y-rc.top;

		if(temp.x <0 || temp.y < 0)
		{
			return;
		}

		m_curBox.x = temp.x/m_fWRatio;
		m_curBox.y = temp.y/m_fHRatio;

		m_ptOrigin = m_ptOld = point;
		m_bDrawFlag = true;

	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void COpenLabelDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bStarted){

		m_bDrawFlag = false;

		CRect rc;
		GetDlgItem(IDC_FRAME)->GetWindowRect(&rc);
		ScreenToClient(rc);


		CPoint temp;
		CPoint out;

		temp.x = point.x - m_ptOrigin.x;
		temp.y = point.y - m_ptOrigin.y;
		if(temp.x < 0 || temp.y < 0)
			return;

		out.x = point.x - rc.right;
		out.y = point.y - rc.bottom;


		temp.x = point.x-rc.left;
		temp.y = point.y-rc.top;

		if(temp.x <0 || temp.y < 0)
		{
			return;
		}

		CClientDC dc(this);
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		CBrush *pOldBrush=dc.SelectObject(pBrush);//ѡ���豸������
		dc.Rectangle(CRect(this->m_ptOrigin, m_ptOld));
		dc.SelectObject(pOldBrush);
		pOldBrush->DeleteObject();
		pBrush->DeleteObject();
		
		//����id
		CGetLabelDlg dlg;
		int ret;
		ret = dlg.DoModal();

		if(out.x > 0 )
			temp.x = rc.right - rc.left;
		if(out.y > 0 )
			temp.y = rc.bottom - rc.top;

		m_curBox.w = (temp.x/m_fWRatio - m_curBox.x);
		m_curBox.h = (temp.y/m_fHRatio - m_curBox.y);
		m_curBox.id = ret;
		m_Boxes.push_back(m_curBox);

		HWND hWnd = NULL;
		GetDlgItem(IDC_FRAME,&hWnd);
		HDC hdc = ::GetDC(hWnd);
		CString str;
		str.Format("%d",this->m_curBox.id);
		TextOut(hdc,m_ptOrigin.x - m_iOffsetX,m_ptOrigin.y - m_iOffsetY ,str,str.GetLength());

		m_ctrlUndo.EnableWindow();
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void COpenLabelDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bDrawFlag)
	{

		CRect rc;
		GetDlgItem(IDC_FRAME)->GetWindowRect(&rc);
		ScreenToClient(rc);

		CPoint temp;
		temp.x = point.x-rc.left;
		temp.y = point.y-rc.top;

		if(temp.x <0 || temp.y < 0)
		{
			return;
		}

		temp.x = point.x - rc.right;
		temp.y = point.y - rc.bottom;

		if(temp.x > 0 || temp.y > 0)
		{
			return;
		}

		temp.x = point.x - m_ptOrigin.x;
		temp.y = point.y - m_ptOrigin.y;
		if(temp.x < 0 || temp.y < 0)
			return;

		CClientDC dc(this);
		dc.SetROP2(R2_NOT);
		CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)) ;
		CBrush *pOldBrush=dc.SelectObject(pBrush);//ѡ���豸������

		dc.Rectangle(CRect(this->m_ptOrigin, m_ptOld));
		dc.Rectangle(CRect(this->m_ptOrigin, point));
		
		dc.SelectObject(pOldBrush);
		pOldBrush->DeleteObject();
		pBrush->DeleteObject();

		m_ptOld = point;
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void COpenLabelDlg::OnBnClickedNext()
{
	// TODO: Add your control notification handler code here
	if(m_pCapture != NULL)
	{
		 if(m_pCurrentFrame = cvQueryFrame(m_pCapture))
		 {
			 for( int i = 0; i<m_Boxes.size(); i++)
			 {
				 fprintf(this->m_pLabelFile,"%d ",m_iCurrentNum-1);
				 fprintf(this->m_pLabelFile,"%d ",m_Boxes[i].id);
				 fprintf(this->m_pLabelFile,"%d %d %d %d\n",m_Boxes[i].x,m_Boxes[i].y,m_Boxes[i].w,m_Boxes[i].h);
				 fflush(this->m_pLabelFile);
			 }

			CString strText;
			strText.Format("Frame No: %d",m_iCurrentNum);
			this->m_ctrlTextFrameNum.SetWindowTextA(strText);
			this->m_iCurrentNum ++;
		
			HWND hWnd = NULL;
			GetDlgItem(IDC_FRAME,&hWnd);
			DisplayImage(hWnd,m_pCurrentFrame);

			m_Boxes.clear();

		 }

		 else
		 {
			 MessageBox("Done! Please click the done button to save the label file!");
			 this->m_ctrlNextFrame.EnableWindow(false);
		 }

	}
}

void COpenLabelDlg::OnBnClickedUndo()
{
	// TODO: Add your control notification handler code here
	if(m_Boxes.size() > 0)
	{
		m_Boxes.pop_back();

		HWND hWnd = NULL;
		GetDlgItem(IDC_FRAME,&hWnd);
		DisplayImage(hWnd,m_pCurrentFrame);

		CClientDC dc(this);
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		CBrush *pOldBrush=dc.SelectObject(pBrush);//ѡ���豸������

		HDC hdc = ::GetDC(hWnd);
		CString str;

		for(int i =0; i< m_Boxes.size() ;i++)
		{	
			dc.Rectangle(m_Boxes[i].x*m_fWRatio + m_iOffsetX,m_Boxes[i].y*m_fHRatio + m_iOffsetY,
				(m_Boxes[i].x+m_Boxes[i].w)*m_fWRatio + m_iOffsetX ,(m_Boxes[i].y+m_Boxes[i].h)*m_fHRatio + m_iOffsetY);
			
			str.Format("%d",m_Boxes[i].id);

			TextOut(hdc,m_Boxes[i].x*m_fWRatio,m_Boxes[i].y*m_fHRatio ,str,str.GetLength());
		}

		dc.SelectObject(pOldBrush);
		pOldBrush->DeleteObject();
		pBrush->DeleteObject();

		if(m_Boxes.size() == 0)
			this->m_ctrlUndo.EnableWindow(false);
	}
}

void COpenLabelDlg::OnBnClickedClearcurrent()
{
	// TODO: Add your control notification handler code here
	m_Boxes.clear();

	HWND hWnd = NULL;
	GetDlgItem(IDC_FRAME,&hWnd);
	this->DisplayImage(hWnd,m_pCurrentFrame);
}


void COpenLabelDlg::OnBnClickedRestart()
{
	// TODO: Add your control notification handler code here
}


void COpenLabelDlg::OnBnClickedDone()
{
	// TODO: Add your control notification handler code here
	if( m_Boxes.size() > 0)
	{
		for( int i = 0; i<m_Boxes.size(); i++)
		{
			fprintf(this->m_pLabelFile,"%d ",m_iCurrentNum-1);
			fprintf(this->m_pLabelFile,"%d ",m_Boxes[i].id);
			fprintf(this->m_pLabelFile,"%d %d %d %d \n",m_Boxes[i].x,m_Boxes[i].y,m_Boxes[i].w,m_Boxes[i].h);
			fflush(this->m_pLabelFile);
		}

		m_Boxes.clear();
	}

	if( m_pLabelFile != NULL)
	{
		fclose(this->m_pLabelFile);
		cvReleaseCapture(&this->m_pCapture);
	}

	this->m_iCurrentNum = 0;
	this->m_ctrlNextFrame.EnableWindow(false);
	this->m_ctrlUndo.EnableWindow(false);
	this->m_bStarted = false;
}


BOOL COpenLabelDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_bStarted)
	{
		if(pMsg->message == WM_KEYDOWN) 
		{
			switch   (pMsg-> wParam){
			case 90:
				if(GetKeyState(VK_CONTROL) < 0)
				{
					OnBnClickedUndo();
					return true;
				}
				break;
			case 78:
					OnBnClickedNext();
					return true;
				break;
			default:
				break;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void COpenLabelDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	((COpenLabelApp*)AfxGetApp())->m_hwndDlg =NULL; 
	// TODO: Add your message handler code here
}
