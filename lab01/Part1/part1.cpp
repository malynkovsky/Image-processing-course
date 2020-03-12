#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
	
	int height = 60, width = 768;
	Mat result_image(Mat::zeros(height * 2, width, CV_8UC1));//результирующее изображение
	//cоздаём картинку (одноканальную, глубина 8 бит), все пискели инициализируем ноликами
	Mat img(Mat::zeros(height, width, CV_8UC1));
	//заполняем значениями так, чтобы получился градиент от черного к белому (делим на 3 из-за размера картинки)
	for (int col = 0; col < width; col += 1) {
		for (int row = 0; row < height; row += 1) {
			img.at<uchar>(Point(col, row)) = col / 3;
		}
	}
	//копируем градиент в верхнюю часть результирующего изображения, а с этим проделаем гамма-коррекцию с коэффициентом 2.4
	img.copyTo(result_image(Rect(0, 0, width, height)));
	img.convertTo(img, CV_64FC1, 1.0 / 255);//переход к float, 1/255 означает, что каждое значение будет поделено на 255
	cv::pow(img, 2.4, img);//возведение в степень значений в каждом пикселе
	img.convertTo(img, CV_8UC1, 255);//возвращение к исходному типу данных
	//копируем скорректированное изображение в нижнюю часть результирующего изображения
	img.copyTo(result_image(Rect(0, height, width, height)));
	//выводим результат
	namedWindow("Gradient and gamma-corrected gradient", 1);
	imshow("Gradient and gamma-corrected gradient", result_image);
	
	waitKey(0);
	return 0;
}
