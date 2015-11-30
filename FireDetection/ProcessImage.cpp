#include "StdAfx.h"
#include "ProcessImage.h"

using namespace cv;

IplImage* imgToShow;
CvMemStorage * storage = cvCreateMemStorage(0);  
CvSeq * contour = 0; 

IplImage * pBkImg;
IplImage * pFrImg;
CvMat* pBkMat;
CvMat* pFrMat;
CvMat* pFrameMat;

extern int g_FrameNum;
//An Early Fire-Detection Method Based on Image Processing
IplImage* ProcessColorModel(IplImage *RGBimg)
{
	IplImage * pImgFire = cvCreateImage(cvSize(RGBimg->width,RGBimg->height), IPL_DEPTH_8U, 3);
	IplImage *dst = cvCreateImage( cvGetSize(RGBimg), IPL_DEPTH_8U, 1 );
	
	cvSet(pImgFire, cvScalar(0,0,0),0);
	int RedThreshold=115;  //115~135 
	int SaturationThreshold=55;  //55~65

	for(int j = 0;j < RGBimg->height;j++)
	{
		for (int i = 0;i < RGBimg->widthStep;i+=3)
		{
			uchar B = (uchar)RGBimg->imageData[j*RGBimg->widthStep+i];
			uchar G = (uchar)RGBimg->imageData[j*RGBimg->widthStep+i+1];
			uchar R = (uchar)RGBimg->imageData[j*RGBimg->widthStep+i+2];
			uchar maxv=max(max(R,G),B); 
			uchar minv=min(min(R,G),B); 
			double S = (1 - 3.0*minv/(R+G+B));

			//(1)R>RT   (2)R>=G>=B   (3)S>=( (255-R) * ST / RT )
			if( R>RedThreshold && R>=G && G>=B && S>0.20 && S>(255-R)/20  &&S>=((255-R)*SaturationThreshold/RedThreshold))
			{
				pImgFire->imageData[i+RGBimg->widthStep*j] = 255;
				pImgFire->imageData[i+1+RGBimg->widthStep*j] = 255;
				pImgFire->imageData[i+2+RGBimg->widthStep*j] = 255;
			}
			else
			{
				pImgFire->imageData[i+RGBimg->widthStep*j] = 0;
				pImgFire->imageData[i+1+RGBimg->widthStep*j] = 0;
				pImgFire->imageData[i+2+RGBimg->widthStep*j] = 0;
			}

		}
	}
	cvCvtColor(pImgFire,dst,CV_BGR2GRAY);
	cvReleaseImage(&pImgFire);
	return dst;

}

IplImage* ProcessColorModelCamera(IplImage *RGBimg)
{
	g_FrameNum ++;
	if (g_FrameNum == 1)
	{
		pBkImg = cvCreateImage(cvGetSize(RGBimg),  IPL_DEPTH_8U,1);					
		pFrImg = cvCreateImage(cvGetSize(RGBimg),  IPL_DEPTH_8U,1);

		pBkMat = cvCreateMat(RGBimg->height, RGBimg->width, CV_32FC1);					
		pFrMat = cvCreateMat(RGBimg->height, RGBimg->width, CV_32FC1);					
		pFrameMat = cvCreateMat(RGBimg->height, RGBimg->width, CV_32FC1);

		cvCvtColor(RGBimg, pBkImg, CV_BGR2GRAY);					
		cvCvtColor(RGBimg, pFrImg, CV_BGR2GRAY);					
		cvConvert(pFrImg, pFrameMat);					
		cvConvert(pFrImg, pFrMat);					
		cvConvert(pFrImg, pBkMat);
	}
	else
	{
		cvCvtColor(RGBimg, pFrImg, CV_BGR2GRAY);					
		cvConvert(pFrImg, pFrameMat);
		cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 5, 0, 0);
		cvAbsDiff(pFrameMat, pBkMat, pFrMat);

		cvThreshold(pFrMat, pFrImg, 60, 255.0, CV_THRESH_BINARY);
		cvSmooth(pFrImg, pFrImg, CV_BLUR,15,15,0,0);
		cvThreshold(pFrImg,pFrImg,60,255,CV_THRESH_BINARY);
		cvSmooth(pFrImg, pFrImg, CV_MEDIAN, 15, 15 ,0 ,0);
		cvErode(pFrImg, pFrImg, 0, 1);					
		cvDilate(pFrImg, pFrImg, 0, 1);
		cvRunningAvg(pFrameMat, pBkMat, 0.3, 0);
		cvConvert(pBkMat, pBkImg);
	}

	return pFrImg;

	/*int RedThreshold=115;  //115~135 
	int SaturationThreshold=55;  //55~65

	for(int j = 0;j < RGBimg->height;j++)
	{
		for (int i = 0;i < RGBimg->widthStep;i+=3)
		{
			uchar B = (uchar)RGBimg->imageData[j*RGBimg->widthStep+i];
			uchar G = (uchar)RGBimg->imageData[j*RGBimg->widthStep+i+1];
			uchar R = (uchar)RGBimg->imageData[j*RGBimg->widthStep+i+2];
			uchar maxv=max(max(R,G),B); 
			uchar minv=min(min(R,G),B); 
			double S = (1 - 3.0*minv/(R+G+B));

			//(1)R>RT   (2)R>=G>=B   (3)S>=( (255-R) * ST / RT )
			if( R>RedThreshold && R>=G && G>=B && S>0.20 && S>(255-R)/20  &&S>=((255-R)*SaturationThreshold/RedThreshold))
			{
				pImgFire->imageData[i+RGBimg->widthStep*j] = 255;
				pImgFire->imageData[i+1+RGBimg->widthStep*j] = 255;
				pImgFire->imageData[i+2+RGBimg->widthStep*j] = 255;
			}
			else
			{
				pImgFire->imageData[i+RGBimg->widthStep*j] = 0;
				pImgFire->imageData[i+1+RGBimg->widthStep*j] = 0;
				pImgFire->imageData[i+2+RGBimg->widthStep*j] = 0;
			}

		}
	}*/
	/*cvCvtColor(pImgFire,dst,CV_BGR2GRAY);
	cvReleaseImage(&pImgFire);
	return dst;*/

}

void FindContours( IplImage* dst )
{
	cvFindContours(dst,storage,&contour,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
	//cvRectangle(imgToShow,cvPoint(137,117),cvPoint(200,200),cv::Scalar(255,0,0),2);
	if(contour)
		cvDrawContours(dst,contour,cvScalarAll(255),cvScalarAll(255),100);   //画轮廓
	//cvShowImage("Contours",dst);
	CvMoments m;
	CvMat mat;
	double M00;
	CvSeq*	contour2=contour;
	IplImage *pTmp=cvCreateImage(cvGetSize(dst),IPL_DEPTH_8U,1);
	cvSet(pTmp, cvScalar(0,0,0),0);
	IplImage *pTmp1=cvCreateImage(cvGetSize(dst),IPL_DEPTH_8U,1);
	cvSet(pTmp1, cvScalar(0,0,0),0);
	for(;contour2!=0;contour2=contour2->h_next)
	{ 
		CvRect rect=cvBoundingRect(contour2); //得到边界的矩形框
		cvDrawContours(pTmp,contour2,CV_RGB(255,255,255),CV_RGB(255,255,255),-1, 1, 8);
		cvRectangle(pTmp,cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),CV_RGB(255,255,255),3,CV_AA,0);

		int n=contour->total;
		CvRect r=((CvContour*)contour2)->rect;
		CvPoint2D32f center;
		cvMoments(cvGetSubRect(pTmp,&mat,r),&m,0);
		M00=cvGetSpatialMoment(&m,0,0);
		center.x=cvGetSpatialMoment(&m,1,0)/M00; //重心为(centerx.+r.x,center.y+r.y)
		center.y=cvGetSpatialMoment(&m,0,1)/M00;
		double nHalfW,nHalfH;
		nHalfW=center.x<(r.width-center.x)?center.x:(r.width-center.x);
		nHalfH=center.y<(r.height-center.y)?center.y:(r.height-center.y);
		cvDrawContours(pTmp1,contour2,CV_RGB(255,255,255),CV_RGB(255,255,255),-1, 1, 8);
		cvRectangle(pTmp1,cvPoint(center.x+r.x-nHalfW,center.y+r.y-nHalfH),cvPoint(center.x+r.x+nHalfW,center.y+r.y+nHalfH),CV_RGB(255,255,255),3,CV_AA,0);
		cvRectangle(imgToShow,cvPoint(center.x+r.x-nHalfW,center.y+r.y-nHalfH),cvPoint(center.x+r.x+nHalfW,center.y+r.y+nHalfH),CV_RGB(107,180,229),3,CV_AA,0);
		//cvSaveImage("contoursss.bmp",pTmp);
		//cvSaveImage("contoursss1.bmp",pTmp1);
		
		//MessageBox(FindWindow("CFireDetectionApp","FireDetection"),"find!","1122",MB_OK);
	}
	cvReleaseImage(&pTmp);
	cvReleaseImage(&pTmp1);
}

ProcessImage::ProcessImage(void)
{
}


ProcessImage::~ProcessImage(void)
{
}

IplImage* ProcessImage::Process(IplImage* img)
{
	//cvRectangle(img,cvPoint(137,117),cvPoint(200,200),cv::Scalar(255,0,0),2);
	//dst = process_rgb(img);
	//cvSaveImage("temp.jpg",temp2);
	imgToShow = img;
	IplImage* dst = ProcessColorModel(img);
	
	//cvSmooth(dst, dst, CV_GAUSSIAN,15,15,0,0); //滤波
	cvSmooth(dst, dst, CV_BLUR,15,15,0,0); //滤波
	cvThreshold( dst, dst, 60, 255, CV_THRESH_BINARY);
	cvSmooth(dst, dst, CV_MEDIAN, 15, 15 ,0 ,0);
	cvErode(dst, dst, 0, 1);
	//cvSaveImage("fushi.jpg",dst);
	cvDilate(dst, dst, 0, 1);
	//cvSaveImage("penzhang.jpg",dst);
	//IplImage* dst = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);
	//cvSet(dst,cvScalar(0),0);初始化图片函数
	
	FindContours(dst);
	cvReleaseImage(&dst);
	return imgToShow;
}

IplImage* ProcessImage::ProcessVideo(IplImage* img)
{
	//cvRectangle(img,cvPoint(137,117),cvPoint(200,200),cv::Scalar(255,0,0),2);
	//dst = process_rgb(img);
	//cvSaveImage("temp.jpg",temp2);
	imgToShow = img;
	IplImage* dst = ProcessColorModelCamera(img);
	
	//cvSmooth(dst, dst, CV_GAUSSIAN,15,15,0,0); //滤波
	cvSmooth(dst, dst, CV_BLUR,15,15,0,0); //滤波
	cvThreshold( dst, dst, 60, 255, CV_THRESH_BINARY);
	cvSmooth(dst, dst, CV_MEDIAN, 15, 15 ,0 ,0);
	cvErode(dst, dst, 0, 1);
	//cvSaveImage("fushi.jpg",dst);
	cvDilate(dst, dst, 0, 1);
	//cvSaveImage("penzhang.jpg",dst);
	//IplImage* dst = cvCreateImage(cvSize(img->width,img->height), IPL_DEPTH_8U, 1);
	//cvSet(dst,cvScalar(0),0);初始化图片函数

	FindContours(dst);
	cvReleaseImage(&dst);
	return imgToShow;
}