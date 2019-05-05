//质心 + NO_ROI两个的坐标进行分水分割,得到分水分割图及其二值图、重心、角度
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include "Segment_WaterSeg.h"
using namespace cv;
using namespace std;


int main()
{

	Mat Mat_Image = imread("./cizhuan/A.bmp");//模板
	Mat Mat_dst, Mat_dst_bw;
	vector<Point2f> mc_result(1);
	double angle;
	Segment_WaterSeg demo;
	demo.Segment_WaterSegment(Mat_Image, Mat_dst, Mat_dst_bw, mc_result, angle, Point(0, 0));
	cout << "[x" << "," << "y" << "," << "angle]" << endl;
	cout << mc_result[0].x << "," << mc_result[0].y << "," << angle << endl;
	namedWindow("着色结果", 0);
	imshow("着色结果", Mat_dst);
	namedWindow("二值结果", 0);
	imshow("二值结果", Mat_dst_bw);
	waitKey(0);

	system("pasue");
	return 0;
}
