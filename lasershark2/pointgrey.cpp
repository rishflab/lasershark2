#include "pointgrey.h"

using namespace FlyCapture2;
using namespace std;


void PointGrey::PrintBuildInfo()
{
	FC2Version fc2Version;
	Utilities::GetLibraryVersion(&fc2Version);

	ostringstream version;
	version << "FlyCapture2 library version: " << fc2Version.major << "." << fc2Version.minor << "." << fc2Version.type << "." << fc2Version.build;
	cout << version.str() << endl;

	ostringstream timeStamp;
	timeStamp << "Application build date: " << __DATE__ << " " << __TIME__;
	cout << timeStamp.str() << endl << endl;
}

void PointGrey::PrintCameraInfo(CameraInfo* pCamInfo)
{
	cout << endl;
	cout << "*** CAMERA INFORMATION ***" << endl;
	cout << "Serial number -" << pCamInfo->serialNumber << endl;
	cout << "Camera model - " << pCamInfo->modelName << endl;
	cout << "Camera vendor - " << pCamInfo->vendorName << endl;
	cout << "Sensor - " << pCamInfo->sensorInfo << endl;
	cout << "Resolution - " << pCamInfo->sensorResolution << endl;
	cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
	cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl << endl;

}

void PointGrey::PrintError(Error error)
{
	error.PrintErrorTrace();
}

int PointGrey::InitSingleCamera() {
	PrintBuildInfo();

	Error error;

	unsigned int numCameras;
	error = busMgr.GetNumOfCameras(&numCameras);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	cout << "Number of cameras detected: " << numCameras << endl;

	for (unsigned int i = 0; i < numCameras; i++)
	{
		error = busMgr.GetCameraFromIndex(i, &guidTemp);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			return -1;
		}
		else {
			guid = guidTemp;
		}
	}

	// Connect to a camera
	error = cam.Connect(&guid);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Get the camera information
	CameraInfo camInfo;
	error = cam.GetCameraInfo(&camInfo);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	PrintCameraInfo(&camInfo);

	// Start capturing images
	error = cam.StartCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}
}

cv::Mat PointGrey::PointGreyImageToMat(Image convertedImage) {
	cv::Mat mat;
	unsigned int rowBytes = (double)convertedImage.GetReceivedDataSize() / (double)convertedImage.GetRows();
	mat = cv::Mat(convertedImage.GetRows(), convertedImage.GetCols(), CV_8UC1, convertedImage.GetData());
	return mat;
}

bool PointGrey::CloseSingleCamera() {
	// Stop capturing images
	Error error;
	error = cam.StopCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return false;
	}

	// Disconnect the camera
	error = cam.Disconnect();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return false;
	}

	return true;
}

bool PointGrey::RunSingleCamera(cv::Mat* mat2){

	Error error;
	
	//cv::Mat mat;
	
	//for (;;) {
		// Retrieve an image
		error = cam.RetrieveBuffer(&rawImage);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			//return false;
		}

		// Create a converted image
		Image convertedImage;

		// Convert the raw image
		error = rawImage.Convert(PIXEL_FORMAT_MONO8, &convertedImage);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			return false;
		}

		
		unsigned int rowBytes = (double)convertedImage.GetReceivedDataSize() / (double)convertedImage.GetRows();
		//mat = cv::Mat(convertedImage.GetRows(), convertedImage.GetCols(), CV_8UC1, convertedImage.GetData());
		*mat2 = cv::Mat(convertedImage.GetRows(), convertedImage.GetCols(), CV_8UC1, convertedImage.GetData());
		

			//mat = pgrey.RunSingleCamera();
			//cv::imshow("DSfwef", *mat2);

			//if (cv::waitKey(1) == 27) {

			//	//CloseSingleCamera();
			//	
			//	return false;
			//}
			
		//}

		return true;
	//return 0;
}
