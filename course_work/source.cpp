#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <numeric>
using namespace std;
using namespace cv;

cv::Mat get_hist_in_HSV(cv::Mat frame) {
	cv::Mat hsv;
	cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
	float h_ranges[] = { 0, 180 }; // hue is [0, 180]
	float s_ranges[] = { 0, 256 };
	float v_ranges[] = { 0, 256 };
	const float* full_ranges[] = { h_ranges, s_ranges, v_ranges };
	const float* ranges[] = { h_ranges, s_ranges };
	const float* ranges1[] = { s_ranges };
	int full_histSize[] = { 30, 32, 32 }, full_ch[] = { 0,1,2 };
	int histSize[] = { 30, 32 }, ch[] = { 0,1 };
	int histSize1 = 32;
	cv::Mat hist;
	vector<Mat> bgr_planes;
	split(hsv, bgr_planes);
	cv::calcHist(&hsv, 1, full_ch, cv::noArray(), hist, 3, full_histSize, full_ranges, true);
	//cv::calcHist(&hsv, 1, ch, cv::noArray(), hist, 2, histSize, ranges, true);
	//cv::calcHist(&bgr_planes[1], 1, 0, Mat(), hist, 1, &histSize1, ranges1, true);
	cv::normalize(hist, hist, 0, 1, cv::NORM_MINMAX);
	return hist;
}

cv::Mat get_hist_in_BGR(cv::Mat frame) {
	float b_ranges[] = { 0, 255 }; 
	float g_ranges[] = { 0, 256 };
	float r_ranges[] = { 0, 256 };
	const float* full_ranges[] = { b_ranges, g_ranges, r_ranges };
	int full_histSize[] = { 32, 32, 32 }, full_ch[] = { 0,1,2 };
	cv::Mat hist;
	cv::calcHist(&frame, 1, full_ch, cv::noArray(), hist, 3, full_histSize, full_ranges, true);
	cv::normalize(hist, hist, 0, 1, cv::NORM_MINMAX);
	return hist;
}

bool compare_two_frames(cv::Mat frame_1, cv::Mat frame_2, int which_method, int color_space) { //метод сравнивает кадры
	bool result = false;
	cv::Mat hist_1, hist_2;
	switch (color_space) {
	case 0: // HSV
		hist_1 = get_hist_in_HSV(frame_1);
		hist_2 = get_hist_in_HSV(frame_2);
		break;
	case 1: // BGR
		hist_1 = get_hist_in_BGR(frame_1);
		hist_2 = get_hist_in_BGR(frame_2);
		break;
	default:
		break;
	}
	double result_of_comparison = compareHist(hist_1, hist_2, which_method);
	switch (which_method) {
	case 0: // Correlation
		//cout << result_of_comparison;
		if ((1.0 - result_of_comparison) > 0.2) {
			result = true;
		}
		break;
	case 1: // Chi-square
		if (result_of_comparison < 30.0) {
			result = true;
		}
		break;
	case 2: // Intersection
		if ((1.0 - result_of_comparison) > 0.2) {
			result = true;
		}
		break;
	case 3: // Bhattacharyya
		if ((1.0 - result_of_comparison) > 0.2) {
			result = true;
		}
		break;
	default:
		break;
	}
	return result;
}

double compare_frames(cv::Mat frame_1, cv::Mat frame_2, int which_method, int color_space) { //метод сравнивает кадры
	//double result = 0.0;
	cv::Mat hist_1, hist_2;
	switch (color_space) {
	case 0: // HSV
		hist_1 = get_hist_in_HSV(frame_1);
		hist_2 = get_hist_in_HSV(frame_2);
		break;
	case 1: // BGR
		hist_1 = get_hist_in_BGR(frame_1);
		hist_2 = get_hist_in_BGR(frame_2);
		break;
	default:
		break;
	}
	double result_of_comparison = compareHist(hist_1, hist_2, which_method);
	return result_of_comparison;
}

vector<cv::Mat> get_key_frames(string video_path, int comparing_method, int color_space) {
	//cv::namedWindow("Video_viewer", cv::WINDOW_AUTOSIZE);
	cv::VideoCapture cap;
	cap.open(video_path);
	vector<cv::Mat> key_frame_store;
	cv::Mat frame;
	int i = 0;
	int j = 0;
	cv::Mat frame_prev;
	for (;;) {
		cap >> frame;
		if (frame.empty()) break; // Ran out of film
		//cv::imshow("Video_viewer", frame);

		cv::Mat hist;
		if (i == 0) {
			frame.copyTo(frame_prev);
			cv::Mat frame_key;
			frame.copyTo(frame_key);
			key_frame_store.push_back(frame_key);
			//cv::imshow("Key_frames", frame);
			j++;
		}
		if (i >= 0) {
			if (compare_two_frames(frame_prev, frame, comparing_method, color_space)) {
				//cout << "we are here)";
				frame.copyTo(frame_prev);
				cv::Mat frame_key;
				frame.copyTo(frame_key);
				key_frame_store.push_back(frame_key);
				//cout << endl;
				//cv::imshow("Key_frames", frame);
				j++;
			}
		}
		i++;
		//if (cv::waitKey(33) >= 0) break;
	}
	cout << endl;
	cout << "Number of keyframes: " << j << endl;
	return key_frame_store;
}

void get_min_max_avg(vector<double> data) {
	vector<double> min_max_avg_data;
	auto min = std::min_element(data.begin(), data.end());
	auto max = std::max_element(data.begin(), data.end());
	auto avg = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
	cout << "Min: " << *min << endl;
	cout << "Max: " << *max << endl;
	cout << "Avg: " << avg << endl;
}

vector<double> get_comparing_data(vector<cv::Mat> key_frame_store1, vector<cv::Mat> key_frame_store2, int comparing_method, int color_space) {
	vector<double> comparing_data;
	double compare_res;
	for (int i = 0; i < key_frame_store1.size(); i++) {
		for (int j = 0; j < key_frame_store2.size(); j++) {
			//compare_res = compare_frames(key_frame_store1[i], key_frame_store2[j], comparing_method, color_space);
			comparing_data.push_back(compare_frames(key_frame_store1[i], key_frame_store2[j], comparing_method, color_space));
		}
	}
	for (int i = 0; i < comparing_data.size(); i++) {
		cout << comparing_data[i] << "  ";
	}
	cout << endl;
	cout << endl;
	cout << endl;
	cout << "comparing_method: " << comparing_method << endl;
	cout << "color_space: " << color_space << endl;
	get_min_max_avg(comparing_data);
	return comparing_data;
	
}
 
void compare_two_videos(string video_path_1, string video_path_2, int comparing_method, int color_space) {
	vector<cv::Mat> key_frame_store1 = get_key_frames(video_path_1, comparing_method, color_space);
	cout << endl;
	vector<cv::Mat> key_frame_store2 = get_key_frames(video_path_2, comparing_method, color_space);
	cout << endl;
	vector<double> comparing_data = get_comparing_data(key_frame_store1, key_frame_store2, comparing_method, color_space);
}

void full_compare_two_videos(string video_path_1, string video_path_2, int color_space) {
	vector<cv::Mat> key_frame_store1 = get_key_frames(video_path_1, 0, color_space);
	cout << endl;
	vector<cv::Mat> key_frame_store2 = get_key_frames(video_path_2, 0, color_space);
	cout << endl;
	vector<double> comparing_data;
	for (int comparing_method = 0; comparing_method < 4; comparing_method++) {
		comparing_data = get_comparing_data(key_frame_store1, key_frame_store2, comparing_method, color_space);
	}
}

int main(int argc, char** argv) {
	//string p1 = "C:/Users/oleg/Desktop/171124_B1_HD_001.mp4";
	string p1 = "C:/Users/oleg/Desktop/CloseFallsTimelapse_1.mp4";
	
	//vector<cv::Mat> key_frame_store1 = get_key_frames(p1, 0, 0);
	//string p2 = "C:/Users/oleg/Desktop/190416_10_Drone1_07.mp4";
	string p2 = "C:/Users/oleg/Desktop/171124_H1_005.mp4";
	//vector<cv::Mat> key_frame_store2 = get_key_frames(p2, 0, 0);
	cout << "Comparing two similar videos" << endl;
	full_compare_two_videos(p1, p2, 0);
	full_compare_two_videos(p1, p2, 1);
	/*compare_two_videos(p1, p2, 0, 0);
	cout << endl;
	compare_two_videos(p1, p2, 1, 0);
	cout << endl;
	compare_two_videos(p1, p2, 2, 0);
	cout << endl;
	compare_two_videos(p1, p2, 3, 0);
	cout << endl;
	compare_two_videos(p1, p2, 0, 1);
	cout << endl;
	compare_two_videos(p1, p2, 1, 1);
	cout << endl;
	compare_two_videos(p1, p2, 2, 1);
	cout << endl;
	compare_two_videos(p1, p2, 3, 1);*/
	cout << endl;
	cout << "Comparing two non-similar videos" << endl;
	p2 = "C:/Users/oleg/Desktop/171124_B1_HD_001.mp4";
	full_compare_two_videos(p1, p2, 0);
	full_compare_two_videos(p1, p2, 1);
	/*compare_two_videos(p1, p2, 0, 0);
	cout << endl;
	compare_two_videos(p1, p2, 1, 0);
	cout << endl;
	compare_two_videos(p1, p2, 2, 0);
	cout << endl;
	compare_two_videos(p1, p2, 3, 0);
	cout << endl;
	compare_two_videos(p1, p2, 0, 1);
	cout << endl;
	compare_two_videos(p1, p2, 1, 1);
	cout << endl;
	compare_two_videos(p1, p2, 2, 1);
	cout << endl;
	compare_two_videos(p1, p2, 3, 1);*/
	cout << endl;
	cv::waitKey(0);
	return 0;
}


