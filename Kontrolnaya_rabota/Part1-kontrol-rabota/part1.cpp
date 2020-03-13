#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
	
	int height = 100, width = 100;
	Mat result_image(Mat::zeros(height * 2, width * 3, CV_8UC1));//результирующее изображение

	Mat img(Mat::zeros(height, width, CV_8UC1));
	circle(img, Point(50, 50), 20, 255, -1, 8, 0);
	img.copyTo(result_image(Rect(0, 0, 100, 100)));
	
	for (int col = 0; col < width; col += 1) {
		for (int row = 0; row < height; row += 1) {
			img.at<uchar>(Point(col, row)) = 127;
		}
	}
	circle(img, Point(50, 50), 20, 0, -1, 8, 0);
	img.copyTo(result_image(Rect(0, 100, 100, 100)));

	circle(img, Point(50, 50), 20, 255, -1, 8, 0);
	img.copyTo(result_image(Rect(100, 0, 100, 100)));
	for (int col = 0; col < width; col += 1) {
		for (int row = 0; row < height; row += 1) {
			img.at<uchar>(Point(col, row)) = 255;
		}
	}
	circle(img, Point(50, 50), 20, 127, -1, 8, 0);
	img.copyTo(result_image(Rect(100, 100, 100, 100)));
	for (int col = 0; col < width; col += 1) {
		for (int row = 0; row < height; row += 1) {
			img.at<uchar>(Point(col, row)) = 255;
		}
	}
	circle(img, Point(50, 50), 20, 0, -1, 8, 0);
	img.copyTo(result_image(Rect(200, 0, 100, 100)));
	for (int col = 0; col < width; col += 1) {
		for (int row = 0; row < height; row += 1) {
			img.at<uchar>(Point(col, row)) = 0;
		}
	}
	circle(img, Point(50, 50), 20, 127, -1, 8, 0);
	img.copyTo(result_image(Rect(200, 100, 100, 100)));
	
	namedWindow("Result", 1);
	imshow("Result", result_image);
	
	waitKey(0);
	return 0;
}
