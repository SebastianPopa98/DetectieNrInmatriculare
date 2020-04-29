#include<opencv2/opencv.hpp>
#include<iostream>
#include<conio.h>     
using namespace std;
using namespace cv;

int main() {
	Mat imgOriginal;       
	Mat imgGrayscale;       
	Mat imgBlurred;        
	Mat imgCanny;           

	imgOriginal = imread("1.jpg");         

	if (imgOriginal.empty()) {                                  
		cout << "error: image not read from file\n\n";     
		_getch();                                               
		return(0);                                              
	}

	cvtColor(imgOriginal, imgGrayscale, COLOR_BGR2GRAY);    

	GaussianBlur(imgGrayscale, imgBlurred, Size(5, 5), 1.4); 

	Canny(imgBlurred, imgCanny, 30, 250);                     
						
	namedWindow("imgOriginal", WINDOW_AUTOSIZE);    
	namedWindow("imgCanny", WINDOW_AUTOSIZE);        
															
	imshow("imgOriginal", imgOriginal);     
	imshow("imgCanny", imgCanny);
	vector<vector<Point> > contours;
	findContours(imgCanny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
	Mat drawing = Mat::zeros(imgCanny.rows, imgCanny.cols, CV_8UC3);;
	for (int i = 0; i< contours.size(); i++){
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(drawing, contours, i, color);
	}

	namedWindow("Contoururi", WINDOW_AUTOSIZE);
	imshow("Contoururi", drawing);
	//accesare pixeli
	//for (int y = 0; y < imgCanny.cols; y++) {
	//	for (int x = 0; x < imgCanny.rows; x++) {
	//		Scalar pixel = imgCanny.at<uchar>(x, y);
	//		cout << pixel.val[0] << ",";
	//	}
	//	cout << endl;
	//}
	waitKey(0);                 

	return(0);
}
