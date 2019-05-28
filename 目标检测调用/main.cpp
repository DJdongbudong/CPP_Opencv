// tensorflow object detection api 导出的魔心识别
// faster_rcnn 和 ssd 的模型
#include<opencv2\opencv.hpp>
#include<opencv2\dnn.hpp>
#include <iostream>
#include<map>
#include<string>
#include<time.h>

using namespace std;
using namespace cv;

int main()
{
	// 初始化
	String weights = "frozen_inference_graph.pb";
	String prototxt = "label_map.pbtxt";
	const char* classNames[] = { "s","h","k" };//faster_rcnn从目标0，1，2开始_Size(600,500)
	//const char* classNames[] = { "b","s","h","k" };//ssd从背景0，目标1，2，3开始_Size(600,500)

	//dnn从磁盘直接加载序列化模型
	dnn::Net net = cv::dnn::readNetFromTensorflow(weights, prototxt);
	
	//——————————————————————————————//
	// 循环读图
	int n =10;
	string ImgName;
	Mat frame;
	for (size_t i = 0; i < n; i++)
	{
		cout << "图片"<< i <<"————————————————————————"<< endl; 
		/*——————————————————————————————————计时0*/
		double dur;
		clock_t start, end;
		start = clock();

		// 图片读取---------------------------------
		stringstream ss;
		string str;
		ss << i;
		ss >> str;
		ImgName = "./images/image" + str +".bmp";		
		frame = cv::imread(ImgName);

		// dnn从磁盘加载图片---------------------------
		cv::Mat blob = cv::dnn::blobFromImage(frame, 1, Size(600, 500), false, true);
		net.setInput(blob);

		//dnn用.forward方法来向前传播我们的图像，获取分类结果--------------------------
		Mat output = net.forward();
		Mat detectionMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());

		double SUM_AREA = 0;
		int sum_s = 0;
		int sum_h = 0;
		int sum_k = 0;
		float confidenceThreshold = 0.8;
		for (int i = 0; i < detectionMat.rows; i++)
		{
			float confidence = detectionMat.at<float>(i, 2);

			if (confidence > confidenceThreshold)
			{
				size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));
				int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
				int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
				int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
				int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

				ostringstream ss;
				ss << confidence;
				String conf(ss.str());

				Rect object((int)xLeftBottom, (int)yLeftBottom,	(int)(xRightTop - xLeftBottom),	(int)(yRightTop - yLeftBottom));
				
				// 统计标注点
				cout << objectClass << "的坐标:" << endl;
				cout << "左上点的坐标【" << (int)xLeftBottom << "，" << (int)yLeftBottom << "】" << endl;
				cout << "右下点的坐标【" << (int)xRightTop << "，" << (int)yRightTop << "】" << endl;
				// 统计每点的占有面积
				double sum_area = (int)(xRightTop - xLeftBottom)*(int)(yRightTop - yLeftBottom);
				cout << "该点面积：" << sum_area << endl;
				SUM_AREA = SUM_AREA + sum_area;

				if (String(classNames[objectClass]) == "s")
				{
					sum_s = sum_s + 1;
					rectangle(frame, object, Scalar(255, 0, 0), 2);
					String label = String(classNames[objectClass]) + ": " + conf;
					int baseLine = 0;
					Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
					rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
						Size(labelSize.width, labelSize.height + baseLine)),
						Scalar(255, 0, 0), 3);
					putText(frame, label, Point(xLeftBottom, yLeftBottom),
						FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
				}
				else if (String(classNames[objectClass]) == "h")
				{
					sum_h = sum_h + 1;
					rectangle(frame, object, Scalar(0, 255, 0), 2);
					String label = String(classNames[objectClass]) + ": " + conf;
					int baseLine = 0;
					Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
					rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
						Size(labelSize.width, labelSize.height + baseLine)),
						Scalar(0, 255, 0), 3);
					putText(frame, label, Point(xLeftBottom, yLeftBottom),
						FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
					
				}
				else if (String(classNames[objectClass]) == "k")
				{
					sum_k = sum_k + 1;
					rectangle(frame, object, Scalar(0, 0, 255), 2);
					String label = String(classNames[objectClass]) + ": " + conf;
					int baseLine = 0;
					Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
					rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
						Size(labelSize.width, labelSize.height + baseLine)),
						Scalar(0, 0, 255), 3);
					putText(frame, label, Point(xLeftBottom, yLeftBottom),
						FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
					
				}
				else 
				{
					rectangle(frame, object, Scalar(255, 0, 0), 2);
					String label = String(classNames[objectClass]) + ": " + conf;
					int baseLine = 0;
					Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
					rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
						Size(labelSize.width, labelSize.height + baseLine)),
						Scalar(255 , 0, 0), 3);
					putText(frame, label, Point(xLeftBottom, yLeftBottom),
						FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1, 20);					
				}
			}
		}
		cout << "拥有s" << sum_s << endl;
		cout << "拥有h" << sum_h <<endl;
		cout << "拥有k" << sum_k <<endl;
		cout << "拥有shk个数" << sum_s + sum_h + sum_k << "总占有面积" << SUM_AREA << endl;
		/*——————————————————————————————————————————计时1*/
		//打印时间
		end = clock();
		dur = (double)(end - start);
		printf("Use Time:%f\n", (dur / CLOCKS_PER_SEC));
		putText(frame, to_string(dur / CLOCKS_PER_SEC), Point(20, 20),
			FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1, 50);		

		namedWindow("image", 0);
		imshow("image", frame);	
		stringstream os;
		string SaveName;	
		string Img_Path;
		os << i;
		os >> SaveName;
		Img_Path = "./result/result_" + SaveName + ".bmp";
		imwrite(Img_Path, frame);
		//waitKey(0);
	}		
	system("pause");
	return 0;
}

//const char* classNames[] = { "s","h","k" };//这个需要根据训练的类别定义
//Mat frame = cv::imread("image1.bmp");
//String weights = "frozen_inference_graph.pb";
//String prototxt = "label_map.pbtxt";
