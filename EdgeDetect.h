#include<opencv2/opencv.hpp>
#include<iostream>
#include<conio.h>  
#include"Functions.h"
#include <cmath>
#include <iomanip> 

using namespace std;
using namespace cv;

Mat gaussian_blur(Mat imgOriginal, float sigma) {
	Mat imgBlurred = imgOriginal;
	int H = imgBlurred.rows;
	int W = imgBlurred.cols;
	int gaussianMask[5][5] ;
	int colTotal = 0, rowTotal = 0, colOffset, rowOffset;
	double newPixel;
	Scalar value;
	double r, s = 2.0 * sigma * sigma;
	double sum = 0.0;
	double M_PI = 3.1415;
	gaussianMask[0][0] = 2;	 gaussianMask[0][1] = 4;  gaussianMask[0][2] = 5;  gaussianMask[0][3] = 4;  gaussianMask[0][4] = 2;
	gaussianMask[1][0] = 4;	 gaussianMask[1][1] = 9;  gaussianMask[1][2] = 12; gaussianMask[1][3] = 9;  gaussianMask[1][4] = 4;
	gaussianMask[2][0] = 5;	 gaussianMask[2][1] = 12; gaussianMask[2][2] = 15; gaussianMask[2][3] = 12; gaussianMask[2][4] = 5;
	gaussianMask[3][0] = 4;	 gaussianMask[3][1] = 9;  gaussianMask[3][2] = 12; gaussianMask[3][3] = 9;  gaussianMask[3][4] = 4;
	gaussianMask[4][0] = 2;	 gaussianMask[4][1] = 4;  gaussianMask[4][2] = 5;  gaussianMask[4][3] = 4;  gaussianMask[4][4] = 2;
	/*for (int x = -2; x <= 2; x++) {
		for (int y = -2; y <= 2; y++) {
			r = sqrt(x * x + y * y);
	gaussianMask[x + 2][y + 2] = (double)(exp(-(r * r) / s)) / (M_PI * s);
			cout << gaussianMask[x+2][y+2] << "\t";
			sum += gaussianMask[x + 2][y + 2];
		}
	}
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			gaussianMask[i][j] /= sum;

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j)
			cout << gaussianMask[i][j] << "\t";
		cout << endl;
	}*/

	for (int row = 2; row < H - 2; row++) {
		for (int col = 2; col < W - 2; col++) {
			newPixel = 0;
			for (int rowOffset = -2; rowOffset <= 2; rowOffset++) {
				for (int colOffset = -2; colOffset <= 2; colOffset++) {
					rowTotal = row + rowOffset;
					colTotal = col + colOffset; 
					value = imgBlurred.at<uchar>(rowTotal , colTotal);
					newPixel += value.val[0] * gaussianMask[2 + rowOffset][2 + colOffset];
				}
			}
			imgBlurred.at<uchar>(row, col) =  newPixel /159 ;
		}
	}
	imshow("Blur1", imgBlurred);
	return imgBlurred;

}

Mat Canny_edge(Mat imgBlurred, int upperThreshold, int lowerThreshold){
	Mat imgCanny = imgBlurred;
	int H = imgCanny.rows;
	int W = imgCanny.cols;
	int GxMask[3][3], GyMask[3][3];
	bool edgeEnd;


	
	GxMask[0][0] = -1; GxMask[0][1] = 0; GxMask[0][2] = 1;
	GxMask[1][0] = -2; GxMask[1][1] = 0; GxMask[1][2] = 2;
	GxMask[2][0] = -1; GxMask[2][1] = 0; GxMask[2][2] = 1;

	GyMask[0][0] = 1; GyMask[0][1] = 2; GyMask[0][2] = 1;
	GyMask[1][0] = 0; GyMask[1][1] = 0; GyMask[1][2] = 0;
	GyMask[2][0] = -1; GyMask[2][1] = -2; GyMask[2][2] = -1;

	vector<vector<int>> edgeDir;
	vector<vector<float>> gradient;
	

	double Gx, Gy, thisAngle, newAngle;
	int rowTotal, colTotal;
	for (int row = 0; row < H; row++) {
		for (int col = 0; col < W; col++) {
			edgeDir[row][col] = 0;
		}
	}
	for (int row = 0; row < H; row++) {
		for (int col = 0; col < W; col++) {
			gradient[row][col] = 0;
		}
	}
	for (int row = 1; row < H - 1; row++) {
		for (int col = 1; col < W - 1; col++) {			
			Gx = 0;
			Gy = 0;
			for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
				for (int colOffset = -1; colOffset <= 1; colOffset++) {
					rowTotal = row + rowOffset;
					colTotal = col + colOffset;
					Gx += imgBlurred.at<uchar>(rowTotal , colTotal ) * GxMask[rowOffset + 1][colOffset + 1];
					Gy += imgBlurred.at<uchar>(rowTotal , colTotal ) * GyMask[rowOffset + 1][colOffset + 1];
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
	for (int row = 1; row < H - 1; row++) {
		for (int col = 1; col < W - 1; col++) {
			edgeEnd = false;
			if (gradient[row][col] > upperThreshold) {		
				switch (edgeDir[row][col]){		
					case 0:
						findEdge(0, 1, row, col, 0, lowerThreshold ,imgCanny, edgeDir, gradient);
						break;
					case 45:
						findEdge(1, 1, row, col, 45, lowerThreshold, imgCanny, edgeDir, gradient);
						break;
					case 90:
						findEdge(1, 0, row, col, 90, lowerThreshold, imgCanny, edgeDir, gradient);
						break;
					case 135:
						findEdge(1, -1, row, col, 135, lowerThreshold, imgCanny, edgeDir, gradient);
						break;
					default :
						imgCanny.at<uchar>(row, col) = 0;
						break;
					}
				}
			else {
				imgCanny.at<uchar>(row, col) = 0;
			}	
		}
	}

	for (int row = 0; row < H; row++) {
		for (int col = 0; col < W; col++) {
			if (((imgCanny.at<uchar>(row, col) != 255) && (imgCanny.at<uchar>(row, col) != 0)))
			imgCanny.at<uchar>(row, col) = 0; 
		}
	}

	
	for (int row = 1; row < H - 1; row++) {
		for (int col = 1; col < W - 1; col++) {
			if (imgCanny.at<uchar>(row, col) == 255) {		
														
				switch (edgeDir[row][col]) {
				case 0:
					suppressNonMax(1, 0, row, col, 0, lowerThreshold, imgCanny, edgeDir, gradient);
					break;
				case 45:
					suppressNonMax(1, -1, row, col, 45, lowerThreshold, imgCanny, edgeDir, gradient);
					break;
				case 90:
					suppressNonMax(0, 1, row, col, 90, lowerThreshold, imgCanny, edgeDir, gradient);
					break;
				case 135:
					suppressNonMax(1, 1, row, col, 135, lowerThreshold, imgCanny, edgeDir, gradient);
					break;
				default:
					break;
				}
			}
		}
	}

	imshow("Canny1", imgCanny);
	return imgCanny;
}
