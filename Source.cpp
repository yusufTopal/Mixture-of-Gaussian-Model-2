#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <sstream>
using namespace std;
using namespace cv;
int main()
{
	int operation;
	Ptr<BackgroundSubtractorMOG2> pMOG2;
	Mat frame, resize_blur_Img, binaryImg, ContourImg;
	
	Mat fgMaskMOG2;
	double largest_area = 0;
	int largest_contour_index = 0;
	Rect bounding_rect;
	int key = 1;
	
	pMOG2 = createBackgroundSubtractorMOG2(500, 300, false);
	namedWindow("Blur Resized Output", 0);
	VideoCapture cap("deneme.avi");
	

	while (key != 0)
	{
		Mat cameraFrame;
		if (!(cap.read(frame)))
			break;
		Mat element = getStructuringElement(MORPH_RECT, Size(20, 20), Point(5, 5));
		resize(frame, resize_blur_Img, Size(frame.size().width, frame.size().height));
		blur(resize_blur_Img, resize_blur_Img, Size(4, 4));
		pMOG2->apply(resize_blur_Img, fgMaskMOG2);
		morphologyEx(fgMaskMOG2, binaryImg,MORPH_CLOSE, element);
		threshold(binaryImg, binaryImg, 0, 255, THRESH_BINARY);
		ContourImg = binaryImg.clone();
		vector< vector< Point> > contours;
		findContours(ContourImg, contours,RETR_EXTERNAL, CHAIN_APPROX_NONE);
		for (int i = 0; i < contours.size(); i++)
		{
			double a = contourArea(contours[i], false);
			if (a >= largest_area)
			{
				largest_area = a;
				largest_contour_index = i;

			}

			bounding_rect = boundingRect(contours[largest_contour_index]);
			if (bounding_rect.area() > 50) {
				rectangle(resize_blur_Img, bounding_rect, CV_RGB(255, 0, 0));
				Point centerPoint = (bounding_rect.br() + bounding_rect.tl())*0.5;
				int x = centerPoint.x;
				int y = centerPoint.y;
				circle(resize_blur_Img, centerPoint, 3, CV_RGB(0, 255, 0), -1);
			}
			//imshow("Shadow_Removed Output", binaryImg);
			//imshow("MOG2 Output", fgMaskMOG2);
			//setMouseCallback("Blur_Resize", CallBackFunc, NULL);
			imshow("Blur Resized Output", resize_blur_Img);
		}
		key = waitKey(30);
	}
	return EXIT_SUCCESS;
}
