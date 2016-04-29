#include "hough.h"



using namespace cv;
using namespace std;
// int main(){

// 	Mat src;
//  	cv::VideoCapture cap;
//  	Point target;
//     cap.open(0);

//     if (!cap.isOpened()){
//         printf("cam index 0 is incorrect\r\n");
//         cap.release();
//         cap.open(1);
//     }

//     if (cap.isOpened()){
//         printf("camera worked\r\n");
//     }

//  	while(1){
//  		cap.read(src);
//  		target = HoughFind(src);
// 	  	if(waitKey(1) == 27){
//             break;
//         }
//     }

//     cap.release();
//   	return 0;
// }

bool HoughFind(Mat src_gray, Point* center){
	//Point center;
	//    long startTime;
	//    long endTime;
	//  	startTime = timestamp_usec();
	//Mat src_gray;
	//cvtColor( src, src_gray, CV_BGR2GRAY );
	//GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);
	//vector<Vec3f> circles;
	vector<Vec3f> circles;
	HoughCircles(src_gray, circles,CV_HOUGH_GRADIENT, 2, src_gray.rows / 4, 200, 100, 0, 0);

	if (circles.empty()){
		printf("notfound\r\n");
		return false;
	}
	else {
		for (size_t i = 0; i < circles.size(); i++){


			center->x = cvRound(circles[0][0]);
			center->y = cvRound(circles[0][1]);

			printf("x =%d, y = %d\r\n", center->x, center->y);
			//int radius = cvRound(circles[0][2]);
			// circle center
			//circle( src, *center, 3, Scalar(0,255,0), -1, 8, 0 );
			// circle outline
			//circle( src, *center, radius, Scalar(0,0,255), 3, 8, 0 );
		}
	}

	//endTime = timestamp_usec();
	//printf("time: %ld\r\n", (endTime - startTime));
	/// Show your results
	//namedWindow( "Hough Circle ", CV_WINDOW_AUTOSIZE );
	//imshow( "Hough Circle ", src ); 

	return true;


}

