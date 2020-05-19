#include<opencv2/opencv.hpp>
#include<iostream>
#include<conio.h>  

using namespace std;
using namespace cv;

Mat dilation(Mat imgsrc) {
	Mat out = Mat::zeros(imgsrc.rows, imgsrc.cols, CV_8UC1);
	int H = out.rows;
	int W = out.cols;
	for (int row = 1; row < H - 1; row++) {
		for (int col = 1; col < W - 1; col++) {
			if ((imgsrc.at<uchar>(row - 1, col) == 255) || (imgsrc.at<uchar>(row + 1, col) == 255) || (imgsrc.at<uchar>(row, col - 1) == 255) || (imgsrc.at<uchar>(row, col + 1) == 255)) {
				out.at<uchar>(row, col) = 255;
				
			}
		}
	}

	return out;
}

