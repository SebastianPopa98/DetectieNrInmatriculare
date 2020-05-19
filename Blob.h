#include<opencv2/opencv.hpp>
#include<iostream>
#include<conio.h>    

using namespace std;
using namespace cv;

float distThreshold = 90;

float distSq(float x1, float y1, float x2, float y2) {
	return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
}


class Blob {
public: int minx, maxx, miny, maxy;
		Blob(){}
		Blob(int x, int y) {
			this->minx = x;
			this->miny = y;
			this->maxx = x;
			this->maxy = y;
		}
		void add(int x, int y) {
			this->minx = min(minx, x);
			this->miny = min(miny, y);
			this->maxx = max(maxx, x);
			this->maxy = max(maxy, y);
		}
		bool isNear(float x, float y) {
			float cx = (minx + maxx) / 2;
			float cy = (miny + maxy) / 2;

			float d = distSq(cx, cy, x, y);
			if (d < distThreshold*distThreshold) {
				return true;
			}
			else {
				return false;
			}
		}
		void init(int x, int y) {
			this->minx = x;
			this->miny = y;
			this->maxx = x;
			this->maxy = y;
		}
		void print() {
			cout << this->minx << " " << this->miny << " " << this->maxx << " " << this->maxy<<"  ";
		}
};


void detect(Mat imgOriginal, Mat imgdilation) {
	int H = imgdilation.rows;
	int W = imgdilation.cols;
	Blob b;
	vector<Blob> blobs;
	for (int row = 0; row < H; row++) {
		for (int col = 0; col < W; col++) {
			if (imgdilation.at<uchar>(row, col) == 255) {

				bool found = false;
				for (int i = 0; i < blobs.size(); i++) {
					if (blobs[i].isNear(row, col)) {
						blobs[i].add(row, col);


						found = true;
						break;
					}
				}
				if (!found) {
					b.init(row, col);
					blobs.push_back(b);
				}
			}
		}
	}

	for (int i = 0; i < blobs.size(); i++) {
		//if(blobs[i].maxy-blobs[i].miny >1.5* ( blobs[i].maxx - blobs[i].minx))
		rectangle(imgdilation, Point(blobs[i].miny, blobs[i].minx), Point(blobs[i].maxy, blobs[i].maxx), Scalar(150, 100, 0), 2, 8, 0);
	}
	for (int i = 0; i < blobs.size(); i++) {
		if (blobs[i].maxy - blobs[i].miny >1.3* (blobs[i].maxx - blobs[i].minx) && blobs[i].maxy - blobs[i].miny > 30 && blobs[i].maxx - blobs[i].minx > 30)
			rectangle(imgOriginal, Point(blobs[i].miny, blobs[i].minx), Point(blobs[i].maxy, blobs[i].maxx), Scalar(255, 0, 0), 2, 8, 0);
	}
	imshow("box", imgdilation);
	imshow("detect", imgOriginal);
}

