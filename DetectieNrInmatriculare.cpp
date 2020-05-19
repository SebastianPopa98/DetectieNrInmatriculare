#include<opencv2/opencv.hpp>
#include<iostream>
#include<conio.h>     
#include "EdgeDetect.h"
#include "Dilation.h"
#include "Blob.h"
using namespace std;
using namespace cv;

int main() {
	Mat imgOriginal;       
	Mat imgGrayscale;       
	Mat imgBlurred;        
	Mat imgCanny;           
	Mat imgThresh;
	imgOriginal = imread("6.jpg");         

	if (imgOriginal.empty()) {                                  
		cout << "error: image not read from file\n\n";     
		_getch();                                               
		return(0);                                              
	}

	cvtColor(imgOriginal, imgGrayscale, COLOR_BGR2GRAY);  
	imshow("imgGrayscale", imgGrayscale);
	//Mat imgBlurred1 = gaussian_blur(imgGrayscale, 1.0);
	GaussianBlur(imgGrayscale, imgBlurred, Size(5, 5), 1.4); 
	adaptiveThreshold(imgBlurred, imgThresh, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 7, 20);
	Canny(imgThresh, imgCanny, 30, 250);                     
						
	namedWindow("imgOriginal", WINDOW_AUTOSIZE);    
	namedWindow("imgCanny", WINDOW_AUTOSIZE);        
															
	imshow("imgOriginal", imgOriginal);     
	imshow("imgCanny", imgCanny);
	

	//Mat imgCanny1 = Canny_edge(imgBlurred1);

	
	Mat imgdilation;
	imgdilation = dilation(imgCanny);
	for (int i = 0; i < 15; i++) {
		imgdilation = dilation(imgdilation);
	}
	imshow("imgdilation", imgdilation);
	
	detect(imgOriginal, imgdilation);
	
	waitKey(0);


	return(0);
}
