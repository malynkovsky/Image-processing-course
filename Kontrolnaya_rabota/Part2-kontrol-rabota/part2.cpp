#include <opencv2/opencv.hpp>
using namespace cv;

int main() {

	int height = 100, width = 100;
	Mat result_image(Mat::zeros(height * 2, width * 3, CV_8UC1));

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

	//теперь делаем линейные фильтры
	result_image.convertTo(result_image, CV_32FC1);
	Mat I1(Mat::zeros(height * 2, width * 3, CV_32FC1));
	Mat I2(Mat::zeros(height * 2, width * 3, CV_32FC1));
	Mat I3(Mat::zeros(height * 2, width * 3, CV_32FC1));
	Mat src_1 = result_image.clone();
	Mat src_2 = result_image.clone();

	cv::Mat kernelH(3, 3, CV_32F);
	kernelH.at<float>(0, 0) = 1.0f;
	kernelH.at<float>(0, 1) = 2.0f;
	kernelH.at<float>(0, 2) = 1.0f;

	kernelH.at<float>(1, 0) = 0.0f;
	kernelH.at<float>(1, 1) = 0.0f;
	kernelH.at<float>(1, 2) = 0.0f;

	kernelH.at<float>(2, 0) = -1.0f;
	kernelH.at<float>(2, 1) = -2.0f;
	kernelH.at<float>(2, 2) = -1.0f;


	filter2D(result_image, I1, -1, kernelH, Point(-1, -1), 0, BORDER_DEFAULT);
	filter2D(result_image, I2, -1, kernelH.t(), Point(-1, -1), 0, BORDER_DEFAULT);
	normalize(I1, I1, 0, 255, cv::NORM_MINMAX, CV_32FC1);
	normalize(I2, I2, 0, 255, cv::NORM_MINMAX, CV_32FC1);
	pow(I1, 2, src_1);
	pow(I2, 2, src_2);
	normalize(I1, I1, 0, 255, cv::NORM_MINMAX, CV_8U);
	normalize(I2, I2, 0, 255, cv::NORM_MINMAX, CV_8U);

	namedWindow("Filter1", 1);
	imshow("Filter1", I1);
	namedWindow("Filter2", 1);
	imshow("Filter2", I2);

	I3 = src_1 + src_2;
	sqrt(I3, I3);
	normalize(I3, I3, 0, 255, cv::NORM_MINMAX, CV_8U);
	result_image.convertTo(result_image, CV_8UC1);
	namedWindow("Filter3", 1);
	imshow("Filter3", I3);
	
	waitKey(0);
	return 0;
}