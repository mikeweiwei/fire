
// FireDetectionDlg.h : ͷ�ļ�
//
#include "BtnST.h"
#pragma once

extern int g_FrameNum;
// CFireDetectionDlg �Ի���
class CFireDetectionDlg : public CDialogEx
{
// ����
public:
	CFireDetectionDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FIREDETECTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedImage();
	afx_msg void OnBnClickedVideo();
	afx_msg void OnBnClickedCamera();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CvCapture* capture;
	HDC hDC;
	CDC *pDC;
	CWnd *pwnd;
	CRect rect;
	IplImage* fireImage;
	CvvImage m_CvvImage;
	IplImage *frame;

	//IplImage *frame;
public:
	afx_msg void OnClose();
private:
	CButtonST m_btnImage;
	CButtonST m_btnVideo;
	CButtonST m_btnCamera;
};
