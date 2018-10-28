#pragma once
//#ifdef _DEBUG
//#undef _DEBUG
//#endif 

#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <vector>  
  
using namespace cv;  
using namespace std;  
  
typedef std::vector<cv::Point> Contours;  
  
class GenerateGEI  
{  
public:  
    int num; //当前计数  
    explicit GenerateGEI(const char* folderpath);
    ~GenerateGEI(void);  
    int InsertImage(Mat image);  
    int Process(Mat image);  
    Mat FindContours(Mat image);  
    Mat Resize(Mat image);    
    Mat CalGei(Mat image);  
	void WriteImg();
private:  
    char* gei_name;  
    Mat gei; //当前gei图  
	bool init;
	Rect r0;
	double scale;
	int length;
};  
