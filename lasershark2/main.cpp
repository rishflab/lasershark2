#include "pointgrey.h"
#include "hough.h"

using namespace FlyCapture2;
using namespace std;

int main(int /*argc*/, char** /*argv*/){
	PointGrey pgrey;
	cv::Mat mat;
	cv::Mat mat2;
	cv::Point target;

	if (!pgrey.InitSingleCamera()) {
		return 0;
	}

	for (;;) {
		FlyCapture2::Error error;
		// Retrieve an image
		error = pgrey.cam.RetrieveBuffer(&pgrey.rawImage);
		
		if (error != PGRERROR_OK)
		{
			pgrey.PrintError(error);
			return -1;
		}

		// Create a converted image
		Image convertedImage;

		// Convert the raw image
		error = pgrey.rawImage.Convert(PIXEL_FORMAT_MONO8, &convertedImage);
		if (error != PGRERROR_OK)
		{
			pgrey.PrintError(error);
			return -1;
		}


		unsigned int rowBytes = (double)convertedImage.GetReceivedDataSize() / (double)convertedImage.GetRows();
		mat2 = cv::Mat(convertedImage.GetRows(), convertedImage.GetCols(), CV_8UC1, convertedImage.GetData());
		
		if (HoughFind(mat2, &target)) {

		}

		cv::imshow("DSfwef", mat2);
	
		
		if (cv::waitKey(1) == 27) {

			pgrey.CloseSingleCamera();
			return 0;
			//break;
		}
	}

	return 0;
}

