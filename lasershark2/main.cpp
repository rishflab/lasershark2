#include "pointgrey.h"
#include "hough.h"
#include "serial.h"
//#include <Windows.h>
#define RECV_BUF_SIZE 5 
//using namespace FlyCapture2;
using namespace std;

/*variable describing the current status of the program
 0: initialising
 1: waiting for Camera to capture image
 2: requesting LIDAR and servo data from arbotix
 3: waiting to receive data via serial
 4: finding target image
 5: computing 3D position
*/
int status = 0;
PointGrey pgrey;

long long milliseconds_now() {
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else {
		return GetTickCount();
	}
}

void OnImageGrabbed(Image* pImage, const void* pCallbackData)
{
	cout << pImage->GetTimeStamp().microSeconds << endl;


	FlyCapture2::Error error;

	// Convert the raw image
	error = pImage->Convert(PIXEL_FORMAT_MONO8, &pgrey.convertedImage);
	if (error != PGRERROR_OK)
	{
		pgrey.PrintError(error);

	}



	return;
}

int main(int /*argc*/, char** /*argv*/)
{
	char panval[BUFFERSIZE];
	char tiltval[BUFFERSIZE];
	char distance[BUFFERSIZE];
	char* str = (char*)malloc(sizeof(char)*BUFFERSIZE);
	/*char* panval;
	panval = "ron01\r\n";
	char* tiltval;
	tiltval = "ron01\r\n";
	char* distance;
	distance = "ron01\r\n";*/
	
	char* camwait;
	camwait = "end00\r\n";
	char* startstr;
	startstr = "frame\r\n";

	
	
	cv::Mat mat;
	cv::Mat mat2;

	cv::Point target;

	Serial serial;

	long long start;
	long long elapsed;

	//char* str;
	//str = "ron01\r\n";

	if (!serial.InitSerial("COM3", 115200)) {
		return 0;
	}

	if (!pgrey.InitSingleCamera())
	{
		return 0;
	}
	FlyCapture2::Error error = pgrey.cam.StartCapture(OnImageGrabbed);
	if (error != PGRERROR_OK)  
	{
		cout << "start capture failed" << endl;
		return 0;
	}

	for (;;)
	{
		//start = milliseconds_now();
		if (status == 0) {
			if (serial.Read(&str)) {
				if (*str == *camwait) {
					//status = 1;
				}
				else if (str[0] == 'P') {
					strncpy(panval, str, sizeof(str));
					
				}
				else if (str[0] == 'T') {
					strncpy(tiltval, str, sizeof(str));
				}
				else if (str[0] == 'D') {
					strncpy(distance, str, sizeof(str));
				}
				else if (*str == *startstr) {
					status = 1;
					
				}
			}


			if (status == 1) {
				FlyCapture2::Error error;
				// Retrieve an image
				start = milliseconds_now();

				//pgrey.cam.tim

				error = pgrey.cam.RetrieveBuffer(&pgrey.rawImage);
				FlyCapture2::TimeStamp frametime = pgrey.rawImage.GetTimeStamp();
				cout << frametime.seconds << frametime.microSeconds << endl;
				//FlyCapture2::TimeStamp

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

				/*start = milliseconds_now();

				if (HoughFind(mat2, &target))
				{

				}

				elapsed = milliseconds_now() - start;
				cout << "hough time:" << elapsed << endl;*/

				
				cv::imshow("DSfwef", mat2);
				status = 2;

				elapsed = milliseconds_now() - start;
				cout << "hough time:" << elapsed << endl; 


			}

			if (status == 2) {
				//printf("tracking done\n");
				printf("pan: %s", panval);
				printf("tilt: %s", tiltval);
				printf("dist: %s", distance);
				char* msg;
				msg = "s05120512\0";
				serial.WriteToCom(msg);
				status = 0;
			}

			if (cv::waitKey(1) == 27)
			{
				pgrey.CloseSingleCamera();
				return 0;
				//break;
			}


			//elapsed = milliseconds_now() - start;
			//cout << "grab time:" << elapsed << endl;
		}
		
	}

	pgrey.CloseSingleCamera();
	return 0;


}

