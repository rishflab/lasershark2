#ifndef HOUGH_H
#define HOUGH_H
//#include <stdio.h>
//#include <string.h>
//#include "timestamp.h"
//#include "opencv2\opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>


//using namespace cv;


bool HoughFind(cv::Mat src, cv::Point* center);


#endif
