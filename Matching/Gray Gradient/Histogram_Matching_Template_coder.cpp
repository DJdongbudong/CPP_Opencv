/* 基于灰度匹配Gray gradient matching的程序模板 
输入 彩图灰度图都行
Mat Mat_T 模板图小图
Mat Mat_S 检索的图大图
*/
//method: 
//    CV_TM_SQDIFF   ： 平方差匹配，最好时候为0；——不具备旋转
//    CV_TM_SQDIFF_NORMED ：归一化平方差匹配，最好时候为0；——不具备旋转
//    CV_TM_CCORR      ：相关性匹配法，最差为0；
//    CV_TM_CCORR_NORMED  ：归一化相关性匹配法，最差为0；
//    CV_TM_CCOEFF     ：系数匹配法，最好匹配为1；
//    CV_TM_CCOEFF_NORMED  ：化相关系数匹配法，最好匹配为1；——匹配度较好-可以抵抗轻度旋转
void Histogram_Matching_Template_coder(Mat Mat_T, Mat Mat_S)
{

	Mat resultImg(Size(Mat_S.cols - Mat_T.cols + 1, Mat_S.rows - Mat_T.rows + 1), CV_32FC1);
	Mat TabelImage = Mat_S.clone();
	imshow("template", Mat_T);

	// 1.模版匹配
	matchTemplate(Mat_S, Mat_T, resultImg, CV_TM_CCOEFF_NORMED);

	// 2.正则化(归一化到0-1)
	normalize(resultImg, resultImg, 0, 1, NORM_MINMAX, -1);
	imshow("result", resultImg);

	// 3.自动筛选 maxPosition 的点
	double minValue = 0;
	double maxValue = 0;
	Point minPosition;
	Point maxPosition;
	minMaxLoc(resultImg, &minValue, &maxValue, &minPosition, &maxPosition);

#if 1
	// 4.自动筛选——画	
	cout << "自动筛选：" << endl;
	char table_auto[10] = { 0 };
	rectangle(TabelImage, maxPosition, Point(maxPosition.x + Mat_T.cols, maxPosition.y + Mat_T.rows), Scalar(0, 0, 255), 1, 8);
	double matchValue = resultImg.at<float>(maxPosition.y, maxPosition.x);
	cout << "匹配值：" << matchValue << endl;
	sprintf_s(table_auto, "%.2f", matchValue);
	putText(TabelImage, table_auto, Point(maxPosition.x, maxPosition.y + 50), CV_FONT_BLACK, 1, Scalar(0, 0, 255), 2);
	imshow("自动筛选", TabelImage);
	
#endif

#if 1
	// 4.手工筛选——画
	cout << "手工筛选：" << endl;
	int tempX = 0;
	int tempY = 0;
	char prob[10] = { 0 };
	for (int i = 0; i < resultImg.rows; i++)
	{
		for (int j = 0; j < resultImg.cols; j++)
		{
			//4.1获得resultImg中(i,j)位置的匹配值matchValue
			double matchValue = resultImg.at<float>(i, j);
			sprintf_s(prob, "%.2f", matchValue);
			//4.2给定筛选条件
			//条件1:概率值大于0.9
			//条件2:任何选中的点在x方向和y方向上都要比上一个点大30——放置重复检测同一个目标
			if (matchValue > 0.8 && abs(i - tempX) > 20 && abs(j - tempY) > 20)
			{
				cout << "匹配值：" << matchValue << endl;
				//4.3给筛选出的点画出边框和文字
				rectangle(Mat_S, Point(j, i), Point(j + Mat_T.cols, i + Mat_T.rows), Scalar(0, 255, 255), 2, 8);
				putText(Mat_S, prob, Point(j, i + 100), CV_FONT_BLACK, 1, Scalar(0, 0, 255), 2);
				tempX = i;
				tempY = j;
			}
		}
	}
	imshow("手工筛选", Mat_S);	
#endif
	waitKey(0);
}
