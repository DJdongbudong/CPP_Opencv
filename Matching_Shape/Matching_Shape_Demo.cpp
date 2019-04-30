/*
只能输入处理后的CV_8UC1图（最好是二值图）
#include <iostream>
#include <time.h>

#include <opencv/cxcore.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;
*/
int Matchint_Shape_HU(Mat Mat_T_base, Mat Mat_S_test)
{	
	if (Mat_T_base.type() != CV_8UC1 && Mat_S_test.type() != CV_8UC1)
	{
		return -1;
	}
#if 1
	Mat imageresult1;
	Canny(Mat_T_base, imageresult1, 0, 255);	
	imshow("imageresult1", imageresult1);
	waitKey(0);
	vector<vector<Point>> contours1;
	vector<Vec4i> hierarchy1;
	findContours(imageresult1, contours1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
	cout << "模板轮廓:" << contours1.size() << endl;
	RNG rng(12345);
	int T_w, T_h;
	Mat drawing1(Size(Mat_T_base.cols, Mat_T_base.rows), CV_8UC3, Scalar(0, 0, 0));
	for (int i = 0; i<contours1.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing1, contours1, i, color, 1, 8, hierarchy1, 0, cv::Point());
	}
	namedWindow("imageresult1", 0);
	imshow("imageresult1", drawing1);
#endif

#if 1
	Mat imageresult2;
	Canny(Mat_S_test, imageresult2, 0, 255);
	imshow("imageresult2", imageresult2);
	waitKey(0);
	vector<vector<Point>> contours2;
	vector<Vec4i> hierarchy2;	
	findContours(imageresult2, contours2, hierarchy2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
	
	cout << "检测轮廓:" << contours2.size() << endl;
	vector<RotatedRect> box(contours2.size());
	Mat drawing2(Size(Mat_S_test.cols, Mat_S_test.rows), CV_8UC3, Scalar(0, 0, 0));;
	for (int i = 0; i<contours2.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing2, contours2, i, color, 1, 8, hierarchy2, 0, cv::Point());
		box[i] = minAreaRect(Mat(contours2[i]));
	}
	namedWindow("imageresult2", 0);
	imshow("imageresult2", drawing2);
#endif

	for (int i = 0; i<contours1.size(); i++)
	{
		for (size_t j = 0; j < contours2.size(); j++)
		{
			/*匹配值*/
			double ans = matchShapes(contours1[i], contours2[j], CV_CONTOURS_MATCH_I1, 0);
			std::cout << "第" << i << " 和 " << j << "的匹配值：" << ans << " " << endl;
		
			string label;	stringstream stream;	stream << (float)ans;	stream >> label;
			putText(drawing2, label, Point(box[j].center.x, box[j].center.y), CV_FONT_BLACK, 2, Scalar(0, 255, 0), 8);	
		}
	}
	namedWindow("result", 0);
	imshow("result", drawing2);
	waitKey(0);
	return 0;
}
