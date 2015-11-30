
// FireDetectionDlg.h : 头文件
//
#include "BtnST.h"
#pragma once

extern int g_FrameNum;
// CFireDetectionDlg 对话框
class CFireDetectionDlg : public CDialogEx
{
// 构造
public:
	CFireDetectionDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FIREDETECTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
