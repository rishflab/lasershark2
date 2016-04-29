#ifndef POINTGREY_H
#define POINTGREY_H

#include "FlyCapture2.h"
#include "opencv2\opencv.hpp"

using namespace FlyCapture2;

class PointGrey{
public:	
	void PrintBuildInfo();
	void PrintCameraInfo(CameraInfo* pCamInfo);
	void PrintError(Error error);
	bool RunSingleCamera(cv::Mat* mat2);
	int InitSingleCamera();
	cv::Mat PointGreyImageToMat(Image convertedImage);
	bool CloseSingleCamera();
	Camera cam;
	Image rawImage;

private:
	PGRGuid guid;
	PGRGuid guidTemp;
	BusManager busMgr;
	

};

#endif // !POINTGREY_H