#include <opencv2/opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class ROI_Segment
{
public:
	ROI_Segment();
	~ROI_Segment();

	Mat Image_ROI_rotate(Mat Mat_input, Point2f center, double angle);
private:

};

ROI_Segment::ROI_Segment()
{
}

ROI_Segment::~ROI_Segment()
{
}
int ROI_Segment::test()
{

	return 0;
}


Mat ROI_Segment::Image_ROI_rotate(Mat Mat_input, Point2f center, double angle)
{
	//Point2f center = Point(113, 96);	double angle = 90;
	Mat Mat_dst = Mat_input.clone();	
	Mat roateM = getRotationMatrix2D(center, angle, 1);//获得旋转矩阵,顺时针为负，逆时针为正
	Mat Mat_rotate_Image;
	warpAffine(Mat_dst, Mat_rotate_Image, roateM, Mat_dst.size()); //仿射变换结果frame

	namedWindow("Mat_dst", 0);
	imshow("Mat_dst", Mat_rotate_Image);

	waitKey(0);
	return Mat_rotate_Image;
}
