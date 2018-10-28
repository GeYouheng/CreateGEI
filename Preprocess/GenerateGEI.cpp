#include "StdAfx.h"
#include "GenerateGEI.h"
#include <cmath>
#include <random>

#define L 500  
#define BASE_RATIO 2  
#define TS 100
#define Rotate 5
  
GenerateGEI::GenerateGEI(const char* folderpath)  
{  
    gei_name=new char[100];  
    sprintf(gei_name,"%s",folderpath); //产生的GEI储存位置  
    num=0;  
	init = true;
}  
  
GenerateGEI::~GenerateGEI(void)  
{  
    delete gei_name;  
}  
  
int GenerateGEI::InsertImage(Mat image)  
{  
    num++;  
    Process(image);  
    return 1;  
}  
  
int GenerateGEI::Process(Mat image)  
{  
    Mat origin=FindContours(image);
    Mat normal=Resize(origin);  
    Mat show_gei=CalGei(normal);
    return 1;  
}  
  
Mat GenerateGEI::FindContours(Mat image)  
{  
    Mat image_tmp=image.clone(); //用于findContours,因为该函数使用后会导致原图改变，所以要复制一个用于该函数
	//图像预处理
	cvtColor(image_tmp, image_tmp, CV_BGR2GRAY);
	bitwise_not(image_tmp, image_tmp);
	threshold(image_tmp, image_tmp, TS, 200.0, CV_THRESH_BINARY);
    //每个轮廓的所有角点理论上只需要contours[0]  
	std::vector<std::vector<cv::Point>> contours;
	contours.clear();
    cv::findContours(image_tmp,contours,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE); //findContours只能处理8UC1和32SC1 
	auto MaxContour = contours[0];
	for (auto contour : contours) {
		if (contour.size() > MaxContour.size()) {
			MaxContour = contour;
		}
	}
    cv::Rect rect_roi = cv::boundingRect(cv::Mat(MaxContour));
	if (init) {
		scale = (double)L / (sqrt(pow(rect_roi.width, 2) + pow(rect_roi.height, 2))*BASE_RATIO);
		length = sqrt(pow(rect_roi.width, 2) + pow(rect_roi.height, 2))*BASE_RATIO;
		r0 = cv::Rect(rect_roi.x + (rect_roi.width - length) / 2.0, rect_roi.y + (rect_roi.height - length) / 2.0, length, length);
		init = false;
	}
	
	Mat gait_img2 = Mat::zeros(length, length, CV_8UC1);
	for (int i = 0; i < rect_roi.height; i++) {
		uchar* p_tmp = image_tmp.ptr<uchar>(i+ rect_roi.y);
		uchar* p = gait_img2.ptr<uchar>(i + rect_roi.y-r0.y);
		for (int j = 0;j<rect_roi.width; j++) {
			p[j+ rect_roi.x - r0.x] = p_tmp[j+ rect_roi.x];
		}
	}
	return gait_img2;
}  
  
Mat GenerateGEI::Resize(Mat image)  
{  
	Mat gait_img2;
	cv::resize(image, gait_img2, cv::Size(), scale, scale); //按比例缩放  
    return gait_img2;  
}  
  
Mat GenerateGEI::CalGei(Mat image)  
{  
    if(num==1)  
    {  
        gei=image.clone();  
    }  
    else  
    {  
        //新的图像与原来的gei形成新的gei  
        for(int i=0;i<image.rows;i++)  
        {  
            uchar* p_old_gei=gei.ptr<uchar>(i);  
            uchar* p=image.ptr<uchar>(i);  
            for(int j=0;j<image.cols;j++)  
            {  
                p_old_gei[j]=(p_old_gei[j]*(num-1)+p[j])/num;  
            }  
        }  
    }  
    return gei;  
} 

void GenerateGEI::WriteImg() {
	default_random_engine e{ std::random_device{}() };
	uniform_real_distribution<double> scale(0.8, 1.2); //随机数分布对象 
	uniform_real_distribution<double> angel(0, 360); //随机数分布对象 
	for (int i = 0; i < Rotate; i++) {
		Point center = Point(gei.cols / 2, gei.rows / 2);
		Mat R = getRotationMatrix2D(center, angel(e), scale(e));
		Mat geiR;
		cv::warpAffine(gei, geiR, R, Size(L, L));
		imwrite(string(gei_name)+string("_")+to_string(i)+string(".jpg"), geiR);
	}
	//imwrite(string(gei_name) + string(".jpg"), gei);
	cout << endl;
}
