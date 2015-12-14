
// FireDetectionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FireDetection.h"
#include "FireDetectionDlg.h"
#include "afxdialogex.h"

#include "ProcessImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFireDetectionDlg �Ի���//
int currentState = 0;
int g_FrameNum = 0;
//cv::CascadeClassifier fire_cascade;


CFireDetectionDlg::CFireDetectionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFireDetectionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFireDetectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnImage);
	DDX_Control(pDX, IDC_BUTTON2, m_btnVideo);
	DDX_Control(pDX, IDC_BUTTON3, m_btnCamera);
}

BEGIN_MESSAGE_MAP(CFireDetectionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFireDetectionDlg::OnBnClickedImage)
	ON_BN_CLICKED(IDC_BUTTON2, &CFireDetectionDlg::OnBnClickedVideo)
	ON_BN_CLICKED(IDC_BUTTON3, &CFireDetectionDlg::OnBnClickedCamera)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFireDetectionDlg ��Ϣ�������

BOOL CFireDetectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	/*CRect rc;
	CWnd *pWnd=GetDlgItem(IDC_STATIC1);
	pWnd->GetClientRect(&rc);
	cv::namedWindow("INPUT", 1);
	cv::resizeWindow("INPUT",rc.Width(),rc.Height());
	HWND hWnd = (HWND) cvGetWindowHandle("INPUT");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, pWnd->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);*/

	/*if( !fire_cascade.load("cascade.xml") )
		MessageBox("Error loading classifier");*/

	pwnd = GetDlgItem(IDC_STATIC1);
	this->MoveWindow(410,160,765,598);
	pwnd->MoveWindow(24,85,526,416);

	pDC =pwnd->GetDC();   
	hDC= pDC->GetSafeHdc();  
	pwnd->GetClientRect(&rect);
	
	m_btnImage.MoveWindow(600,137,10,10);
	m_btnVideo.MoveWindow(600,227,10,10);
	m_btnCamera.MoveWindow(600,317,10,10);
	
	m_btnImage.SetBitmaps(IDB_BITMAP2, RGB(255, 255, 255), (int)BTNST_AUTO_DARKER);
	m_btnImage.SetTooltipText(_T("Choose Image"));
	m_btnImage.DrawTransparent();
	m_btnImage.DrawBorder(FALSE, FALSE);
	m_btnImage.SizeToContent();

	m_btnVideo.SetBitmaps(IDB_BITMAP3, RGB(255, 255, 255), (int)BTNST_AUTO_DARKER);
	m_btnVideo.SetTooltipText(_T("Choose Video"));
	m_btnVideo.DrawTransparent();
	m_btnVideo.DrawBorder(FALSE, FALSE);
	m_btnVideo.SizeToContent();

	m_btnCamera.SetBitmaps(IDB_BITMAP4, RGB(255, 255, 255), (int)BTNST_AUTO_DARKER);
	m_btnCamera.SetTooltipText(_T("Open Camera"));
	m_btnCamera.DrawTransparent();
	m_btnCamera.DrawBorder(FALSE, FALSE);
	m_btnCamera.SizeToContent();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFireDetectionDlg::OnPaint()
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
		CPaintDC dc(this); // device context for painting
		CRect rect;   
		GetClientRect(&rect);
		CDC dcMem;   
		dcMem.CreateCompatibleDC(&dc);   
		CBitmap bmpBackground;   
		bmpBackground.LoadBitmap(IDB_BITMAP1); 
		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);   
		CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
		dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFireDetectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFireDetectionDlg::OnBnClickedImage()
{
	CString FilePathName;
	char szFileFilter[]= 
		"Image File(*.jpg;*.jpeg;*.bmp;*.png;*.tiff;*.gif;*.tga)|*.jpg;*.jpeg;*.bmp;*.png;*.tiff;*.gif;*.tga|";
	/*"All File(*.*)|*.*||";*/
	CFileDialog dlg(TRUE, NULL,NULL,NULL,szFileFilter,this);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���

	if ( currentState == 1 )
	{
		KillTimer(1);
		cvReleaseCapture( &capture );
	}
	if ( currentState == 2 )
	{
		KillTimer(2);
		cvReleaseCapture( &capture );
	}

	if(dlg.DoModal()==IDOK)
	{
		FilePathName=dlg.GetPathName();
		currentState = 0;
		m_btnImage.SetBitmaps(IDB_BITMAP5, RGB(255, 255, 255), (int)BTNST_AUTO_DARKER);
		m_btnVideo.SetBitmaps(IDB_BITMAP3, RGB(255, 255, 255), (int)BTNST_AUTO_DARKER);
		m_btnCamera.SetBitmaps(IDB_BITMAP4, RGB(255, 255, 255), (int)BTNST_AUTO_DARKER);
	}
		
	//cv::Mat img = cv::imread((LPCTSTR)FilePathName);
	IplImage* img = cvLoadImage((LPCTSTR)FilePathName);

	if (!img)
		return;
	
	/*std::vector<cv::Rect> fires;
	fire_cascade.detectMultiScale( img, fires, 1.1, 3, 0, cv::Size(30, 30));

	for( size_t i = 0; i < fires.size(); i++ )
	{
		//Mat fireROI = frame_gray( fires[i] );
		rectangle(img,fires[i],cv::Scalar( 255, 0, 0 ),2,8,0);
	}*/

	IplImage* fireImage = ProcessImage::Process(img);
	
	//CvvImage m_CvvImage;
	m_CvvImage.CopyOf(fireImage,1);
	m_CvvImage.DrawToHDC(hDC, &rect);
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CFireDetectionDlg::OnBnClickedVideo()
{
	CString FilePathName;
	char szFileFilter[]= 
		"Video File(*.mp4;*.avi;*.rmvb;*.rm;*.MOV;*.mpeg;*.mpg;*.flv;*.wmv)|*.mp4;*.avi;*.rmvb;*.rm;*.MOV;*.mpeg;*.mpg;*.flv;*.wmv|";
		/*"All File(*.*)|*.*||";*/
	CFileDialog dlg(TRUE, NULL,NULL,NULL,szFileFilter,this);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	
	if ( currentState == 1 )
	{
		KillTimer(1);
		cvReleaseCapture( &capture );
	}
	if ( currentState == 2 )
	{
		KillTimer(2);
		cvReleaseCapture( &capture );
	}

	if(dlg.DoModal()==IDOK)
	{
		FilePathName=dlg.GetPathName();
		currentState = 1;
		m_btnImage.SetBitmaps(IDB_BITMAP2, RGB(255, 255, 255), (int)BTNST_AUTO_DARKER);
		m_btnVideo.SetBitmaps(IDB_BITMAP6, RGB(255, 255, 255), (int)BTNST_AUTO_DARKER);
		m_btnCamera.SetBitmaps(IDB_BITMAP4, RGB(255, 255, 255), (int)BTNST_AUTO_DARKER);
	}
	capture = 0;
	//IplImage *frame = 0;

	capture = cvCaptureFromFile((LPCTSTR)FilePathName);

	IplImage *frame = cvQueryFrame( capture );
	if( !frame ) return;

	SetTimer(1,40,NULL);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CFireDetectionDlg::OnBnClickedCamera()
{
	if ( currentState == 1 )
	{
		KillTimer(1);
		
		cvReleaseCapture( &capture );
	}
	if ( currentState == 2 )
	{
		KillTimer(2);
		cvReleaseCapture( &capture );
	}
	currentState = 2;
	m_btnImage.SetBitmaps(IDB_BITMAP2, RGB(255, 255, 255), (int)BTNST_AUTO_DARKER);
	m_btnVideo.SetBitmaps(IDB_BITMAP3, RGB(255, 255, 255), (int)BTNST_AUTO_DARKER);
	m_btnCamera.SetBitmaps(IDB_BITMAP7, RGB(255, 255, 255), (int)BTNST_AUTO_DARKER);
	capture = 0;
	capture = cvCaptureFromCAM(0);
	if (!capture)
	{
		AfxMessageBox("No camera!");
		return;
	}
	
	SetTimer(2,40,NULL);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CFireDetectionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case 1:
		{
			frame = cvQueryFrame( capture );
			if( !frame ) 
			{
				KillTimer(1);
				Invalidate(TRUE);
				break;
			}

			CRect rc;
			CWnd* pWnd = GetDlgItem(IDC_STATIC1);
			pWnd->GetClientRect(rc);

			fireImage = ProcessImage::Process(frame);

			m_CvvImage.CopyOf(fireImage,1);
			m_CvvImage.DrawToHDC(hDC, &rect);
			//m_CvvImage.Destroy();
			//cvReleaseCapture( &capture );
			break;
		}
	case 2:
		{
			frame = cvQueryFrame( capture );
			if( !frame ) 
			{
				/*KillTimer(2);
				cvReleaseCapture(&capture);
				currentState = 0;*/
				break;
			}
			CRect rc;
			CWnd* pWnd = GetDlgItem(IDC_STATIC1);
			pWnd->GetClientRect(rc);

			fireImage = ProcessImage::Process(frame);
			//fireImage = frame;
			m_CvvImage.CopyOf(fireImage,1);
			m_CvvImage.DrawToHDC(hDC, &rect);
			//m_CvvImage.Destroy();
			//cvReleaseCapture( &capture );

			break;
		}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CFireDetectionDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if ( currentState == 1 )
	{
		KillTimer(1);
		cvReleaseCapture( &capture );
	}
	if ( currentState == 2 )
	{
		KillTimer(2);
		cvReleaseCapture( &capture );
	}
	CDialogEx::OnClose();
}
