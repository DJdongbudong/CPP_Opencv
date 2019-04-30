#include "Segment_WaterSeg.h"
int main()
{
	
	Mat Mat_Image = imread("./resource/A.bmp");//模板
	Mat Mat_dst, Mat_dst_bw;
	Segment_WaterSeg demo;
	demo.Segment_WaterSegment(Mat_Image, Mat_dst, Mat_dst_bw);
	namedWindow("着色结果", 0);
	imshow("着色结果", Mat_dst);
	namedWindow("二值结果", 0);
	imshow("二值结果", Mat_dst_bw);
	waitKey(0);

	system("pasue");
	return 0;
}
