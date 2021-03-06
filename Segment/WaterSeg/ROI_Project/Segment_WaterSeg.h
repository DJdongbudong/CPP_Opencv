#include <opencv2/opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/*
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
*/


class Segment_WaterSeg
{
public:
	void Segment_WaterSegment(Mat Mat_Image, Mat& Mat_dst, Mat& Mat_dst_bw, vector<Point2f> &mc_result, double &angle, Point No_ROI = Point(0, 0));
};

void Segment_WaterSeg::Segment_WaterSegment(Mat Mat_Image, Mat& Mat_dst, Mat& Mat_dst_bw, vector<Point2f> &mc_result, double &angle, Point No_ROI)
{
	// 目标质心寻找，仅使用二值化 和 非重心寻找 Point
	Mat grayImage;
	cvtColor(Mat_Image, grayImage, CV_BGR2GRAY);

	Mat bwImage;

	threshold(grayImage, bwImage, 50, 255, THRESH_BINARY | THRESH_OTSU);

	morphologyEx(bwImage, bwImage, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(9, 9), Point(-1, -1)));

	Mat Labels;
	distanceTransform(bwImage, bwImage, Labels, CV_DIST_L2, CV_DIST_MASK_3, 5);
	normalize(bwImage, bwImage, 0, 1, NORM_MINMAX);
	bwImage.convertTo(grayImage, CV_8UC1);
	threshold(grayImage, bwImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
	morphologyEx(bwImage, grayImage, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(9, 9), Point(-1, -1)));

	namedWindow("grayImage", 0);
	imshow("grayImage", grayImage);
	waitKey(0);

	/*找轮廓8uC1\32sC1的输入图*/
	vector<vector<Point>> contours_mask;
	vector<Vec4i> hierarchy_mask;
	findContours(grayImage, contours_mask, hierarchy_mask, RETR_CCOMP, CHAIN_APPROX_SIMPLE);


	/*画轮廓——可省略*/
#if 1
	Mat drawing = Mat(Mat_Image.size(), CV_8UC1);
	drawing = Scalar::all(0);;
	for (int i = 0; i< contours_mask.size(); i++)
	{
		RNG rng(12345);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours_mask, i, color, 2, 8, hierarchy_mask, 0, Point());
	}
	namedWindow("drawContours", 0);
	imshow("drawContours", drawing);
	waitKey(0);
#endif

	/* 【定位到一个ROI的位置】 */
	// 仅运行轮廓为ROI
	if (contours_mask.size() != 1)
	{
		cout << contours_mask.size() << "个轮廓" << ";	" << "轮廓ROI定位失败！" << endl;
		return;
	}

	cout << contours_mask.size() << "个轮廓" << ";	" << "轮廓定位成功！" << endl;
	vector<Moments> mu(1);
	vector<Point2f> mc(1);
	mu[0] = moments(contours_mask[0], false);
	mc[0] = Point2d(mu[0].m10 / mu[0].m00, mu[0].m01 / mu[0].m00);
	//cout << mc[0].x <<","<< mc[0].y << endl;

	// 返回 ROI 中心点
	//vector<Point2f> mc_result(1);
	mc_result[0].x = mc[0].x;
	mc_result[0].y = mc[0].y;

	// 返回 ROI 旋转角度
	RotatedRect box = minAreaRect(Mat(contours_mask[0]));
	//double angle;
	//利用仿射变换进行旋转        另一种方法，透视变换
	if (0 < abs(box.angle) && abs(box.angle) <= 45)
		angle = box.angle;//负数，顺时针旋转
	else if (45 < abs(box.angle) && abs(box.angle) < 90)
		angle = 90 - abs(box.angle);//正数，逆时针旋转
	//cout << angle  << endl;


	/* 【分水分割】 */
	/*ROI + NO_ROI*/
	Mat Mask_ROI_Image = Mat(Mat_Image.size(), CV_8UC1);
	// No_ROI点 
	circle(Mask_ROI_Image, No_ROI, 5, Scalar::all(255), 50, 40, 0);
	// ROI点
	circle(Mask_ROI_Image, Point(mc[0].x, mc[0].y), 5, Scalar::all(255), 50, 40, 0);

	findContours(Mask_ROI_Image, contours_mask, hierarchy_mask, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	if (contours_mask.size() != 2)
	{
		cout << contours_mask.size() << "个轮廓" << ";	" << "轮廓ROI + NO_ROI定位失败！" << endl;
		return;
	}

	// MaskImage 掩膜标记 
	Mat MaskWaterShed;
	MaskWaterShed = Mat(Mat_Image.size(), CV_32S);
	MaskWaterShed = Scalar::all(0);

	/* 在maskWaterShed上绘制轮廓 */
	for (int index = 0; index < contours_mask.size(); index++)
	{
		Rect boundRect = boundingRect(contours_mask[index]);
		rectangle(grayImage, boundRect, Scalar(0, 0, 255), 30);

		drawContours(MaskWaterShed, contours_mask, index, Scalar::all(index + 1), -1, 8, hierarchy_mask, INT_MAX);
	}

	// 分水岭 	size()一样
	watershed(Mat_Image, MaskWaterShed);  // 

	// 区域着色
	Mat resImage = Mat(MaskWaterShed.size(), CV_8UC3);  // 声明一个最后要显示的图像
	Mat bw_resImage = Mat(MaskWaterShed.size(), CV_8UC1);  // 声明一个最后要显示的图像
	for (int i = 0; i < MaskWaterShed.rows; i++)
	{
		for (int j = 0; j < MaskWaterShed.cols; j++)
		{
			int index = MaskWaterShed.at<int>(i, j);
			if (index == MaskWaterShed.at<int>(mc[0].y, mc[0].x))	//ROI一类
			{
				resImage.at<Vec3b>(i, j) = Mat_Image.at<Vec3b>(i, j);
				bw_resImage.at<uchar>(i, j) = 255;
			}
			else
			{
				resImage.at<Vec3b>(i, j) = Vec3b(0, 0, 0);	//非ROI一类
				bw_resImage.at<uchar>(i, j) = 0;
			}
		}
	}
	Mat_dst = resImage.clone();
	Mat_dst_bw = bw_resImage.clone();
};
