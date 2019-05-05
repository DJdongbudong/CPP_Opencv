#include "Matching_ColorHist.h"
int main(int argv, char* argc)
{
	
#if 1
	/* [ 颜色匹配，越小越相似 ] */
	Mat Mat_temple = imread("./cizhuanROI/ROI_A.jpg");
	Mat Mat_search = imread("./cizhuanROI/ROI_F.jpg");

	Matching_Color color_match_class;
	double color_value = color_match_class.Matching_ColorHist(Mat_temple, Mat_search);
	cout << color_value << endl;
#endif



	waitKey(0);
	return 0;
}
