#include "ROI_Segment.h"
#include "Segment_WaterSeg.h"

int main(int argc, char* argv)
{
	
	Mat Mat_template = imread("./cizhuan/F.bmp");//模板	

	Segment_WaterSeg seg;	
	Mat Mat_dst, Mat_dst_bw;
	vector<Point2f> mc_result(1);
	double angle;
	seg.Segment_WaterSegment(Mat_template, Mat_dst, Mat_dst_bw, mc_result, angle, Point(0, 0));

	cout << "[x" << "," << "y" << "," << "angle]" << endl;
	cout << mc_result[0].x << "," << mc_result[0].y << "," << angle << endl;
	
	
	Point2f center = Point(mc_result[0].x, mc_result[0].y);
	ROI_Segment demo;
	Mat A = demo.Image_ROI_rotate(Mat_dst, center, angle);
	Mat B = demo.Image_ROI_rotate(Mat_dst_bw, center, angle);
	namedWindow("A", 0);
	imshow("A", A);

	/*底下仅仅针对contours[0]的轮廓操作*/
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	findContours(B, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓——如果假如偏移量则轮廓就会一个变成两个
	if (contours.size() != 1)
	{
		return 0;
	}	
	Rect boundRect = boundingRect(contours[0]);
	Mat ROI = A(Rect(boundRect));
	namedWindow("ROI", 0);
	imshow("ROI", ROI);
	waitKey(0);
	//imwrite("./cizhuanROI/ROI_F.jpg", ROI);


	system("pause");
	return 0;
}
