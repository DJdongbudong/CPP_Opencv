#include <opencv/cxcore.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Matching_Color
{
public:
	Matching_Color();
	~Matching_Color();
	double Matching_ColorHist(Mat Mat_temple, Mat Mat_search, int method = CV_COMP_BHATTACHARYYA);
private:

};

Matching_Color::Matching_Color()
{
}

Matching_Color::~Matching_Color()
{
}

/*
method:
//相关：CV_COMP_CORREL
//卡方：CV_COMP_CHISQR
//直方图相交：CV_COMP_INTERSECT
//巴氏距离：CV_COMP_BHATTACHARYYA
#include "Matching_ColorHist.h"
int main(int argv, char* argc)
{

#if 1
	//[ 颜色匹配，越小越相似 ] 
	Mat Mat_temple = imread("./cizhuanROI/ROI_A.jpg");
	Mat Mat_search = imread("./cizhuanROI/ROI_F.jpg");

	Matching_Color color_match_class;
	double color_value = color_match_class.Matching_ColorHist(Mat_temple, Mat_search);
	cout << color_value << endl;
#endif

	waitKey(0);
	return 0;
}
*/
double Matching_Color::Matching_ColorHist(Mat Mat_temple, Mat Mat_search, int method)
{
	//Mat src1 = imread("a.jpg");
	//Mat src2 = imread("h.jpg");
	Mat hsv1, hsv2;

	if (Mat_temple.empty() && Mat_search.empty())
	{
		return 0;
	}

	cvtColor(Mat_temple, hsv1, CV_BGR2HSV);
	cvtColor(Mat_search, hsv2, CV_BGR2HSV);

	/*
	hist.at<float>(i)为矩阵内的值的统计值，i为同一个值，返回同一个i的个数统计值
	*/
	int channels[] = { 0, 1 };

	MatND hist_1;
	MatND hist_2;

	int h_bins = 50;	int s_bins = 60;	int histSize[] = { h_bins, s_bins };//统计的条数，即直方图 i 的值

	float h_ranges[] = { 0, 180 };	float s_ranges[] = { 0, 256 };	const float* ranges[] = { h_ranges, s_ranges };

	int dims = 2;

	calcHist(&hsv1, 1, channels, Mat(), // do not use mask
		hist_1, dims, histSize, ranges,
		true, // the histogram is uniform
		false);
	normalize(hist_1, hist_1, 0, 1, NORM_MINMAX, -1, Mat());


	calcHist(&hsv2, 1, channels, Mat(), // do not use mask
		hist_2, dims, histSize, ranges,
		true, // the histogram is uniform
		false);
	normalize(hist_2, hist_2, 0, 1, NORM_MINMAX, -1, Mat());


	double a_a = compareHist(hist_1, hist_1, method);
	double a_b = compareHist(hist_1, hist_2, method);
	cout << "A compare with A correlation value (default): " << a_a << endl;
	cout << "A compare with B correlation value : " << a_b << endl;


	//给每个图像上添加文字，内容为该图片和原始图片的比较结果
	ostringstream os;
	os << a_a;
	os << ":";
	os << a_b;

	//putText(src1, os.str(), Point(50, 50), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 255), 2, 8);
	putText(Mat_search, os.str(), Point(50, 50), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 255), 2, 8);

	namedWindow("Mat_search", 0);
	imshow("Mat_search", Mat_search);

	return a_b;
}
