#include<opencv2/opencv.hpp>
#include<iostream>
#include<conio.h>  


using namespace std;
using namespace cv;

Mat gaussian_blur(Mat imgOriginal) {
	Mat imgBlurred = imgOriginal;
	int H = imgBlurred.rows;
	int W = imgBlurred.cols;
	int gaussianMask[5][5];
	int newPixel, colTotal = 0, rowTotal = 0, colOffset, rowOffset;
	gaussianMask[0][0] = 2;	 gaussianMask[0][1] = 4;  gaussianMask[0][2] = 5;  gaussianMask[0][3] = 4;  gaussianMask[0][4] = 2;
	gaussianMask[1][0] = 4;	 gaussianMask[1][1] = 9;  gaussianMask[1][2] = 12; gaussianMask[1][3] = 9;  gaussianMask[1][4] = 4;
	gaussianMask[2][0] = 5;	 gaussianMask[2][1] = 12; gaussianMask[2][2] = 15; gaussianMask[2][3] = 12; gaussianMask[2][4] = 2;
	gaussianMask[3][0] = 4;	 gaussianMask[3][1] = 9;  gaussianMask[3][2] = 12; gaussianMask[3][3] = 9;  gaussianMask[3][4] = 4;
	gaussianMask[4][0] = 2;	 gaussianMask[4][1] = 4;  gaussianMask[4][2] = 5;  gaussianMask[4][3] = 4;  gaussianMask[4][4] = 2;


	for (int row = 2; row < H - 2; row++) {
		for (int col = 2; col < W - 2; col++) {
			newPixel = 0;
			for (int rowOffset = -2; rowOffset <= 2; rowOffset++) {
				for (int colOffset = -2; colOffset <= 2; colOffset++) {
					rowTotal = row + rowOffset;
					colTotal = col + colOffset;
					newPixel += imgBlurred.at<uchar>(rowTotal * 3 , colTotal * 3) * gaussianMask[2 + rowOffset][2 + colOffset];
				}
			}
			imgBlurred.at<uchar>(row*3, col*3) = newPixel / 159;
		}
	}
	return imgBlurred;

}

Mat Canny_edge(Mat imgBlurred) {
	Mat imgCanny;
	int GxMask[3][3], GyMask[3][3];
	GxMask[0][0] = -1; GxMask[0][1] = 0; GxMask[0][2] = 1;
	GxMask[1][0] = -2; GxMask[1][1] = 0; GxMask[1][2] = 2;
	GxMask[2][0] = -1; GxMask[2][1] = 0; GxMask[2][2] = 1;

	GyMask[0][0] = 1; GyMask[0][1] = 2; GyMask[0][2] = 1;
	GyMask[1][0] = 0; GyMask[1][1] = 0; GyMask[1][2] = 0;
	GyMask[2][0] = -1; GyMask[2][1] = -2; GyMask[2][2] = -1;


	int edgeDir[240][320];		
	float gradient[240][320];
	int H = imgBlurred.rows;
	int W = imgBlurred.cols;
	double Gx, Gy, thisAngle, newAngle;
	int rowTotal, colTotal;
	for (int row = 1; row < H - 1; row++) {
		for (int col = 1; col < W - 1; col++) {			
			Gx = 0;
			Gy = 0;
			for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
				for (int colOffset = -1; colOffset <= 1; colOffset++) {
					rowTotal = row + rowOffset;
					colTotal = col + colOffset;
					Gx += imgBlurred.at<uchar>(rowTotal * 3, colTotal * 3) * GxMask[rowOffset + 1][colOffset + 1];
					Gy += imgBlurred.at<uchar>(rowTotal * 3, colTotal * 3) * GyMask[rowOffset + 1][colOffset + 1];
				}
			}

			gradient[row][col] = sqrt(pow(Gx, 2.0) + pow(Gy, 2.0));		
			thisAngle = (atan2(Gx, Gy) / 3.14159) * 180.0;		

																
			if (((thisAngle < 22.5) && (thisAngle > -22.5)) || (thisAngle > 157.5) || (thisAngle < -157.5))
				newAngle = 0;
			if (((thisAngle > 22.5) && (thisAngle < 67.5)) || ((thisAngle < -112.5) && (thisAngle > -157.5)))
				newAngle = 45;
			if (((thisAngle > 67.5) && (thisAngle < 112.5)) || ((thisAngle < -67.5) && (thisAngle > -112.5)))
				newAngle = 90;
			if (((thisAngle > 112.5) && (thisAngle < 157.5)) || ((thisAngle < -22.5) && (thisAngle > -67.5)))
				newAngle = 135;

			edgeDir[row][col] = newAngle;		
		}
	}
	return imgCanny;
}