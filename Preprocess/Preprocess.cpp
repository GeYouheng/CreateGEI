// testCreateGEI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "string.h"  
#include <io.h>  
#include <stdio.h>  
#include <iostream>  

#include <opencv2/opencv.hpp>

#include "GenerateGEI.h"  

using namespace std;
using namespace cv;


//@argv[1]:文件夹列表地址  
int _tmain(int argc, char* argv[])
{
	String rootPath = "C:\\Users\\GeYouheng\\Desktop\\DATA\\VIDEO\\V0\\";

	intptr_t  lf;
	_finddata_t file;
	if ((lf = _findfirst((rootPath+"*.*").c_str(), &file)) == -1l) {
		cout << "Can't find files";
		system("pause");
		return -1;
	}
	while (_findnext(lf, &file) == 0) {
		cout << file.name << endl;
		string str(file.name);
		int te = str.find(".mp4");
		if (te == -1) continue;

		string filename = rootPath + str;
		string temp(filename);
		VideoCapture capture(temp);

		std::size_t pos = filename.find("mp4");
		string file = filename.substr(0, filename.length() - 4);
		GenerateGEI* generate = new GenerateGEI(file.c_str());
		int count = 0; 
		Mat image;
		while (true) {
			capture >> image;
			if (image.empty()|| count++ > 50)
				break;
			generate->InsertImage(image);
			//cout << count<<endl;
		}
		generate->WriteImg();
		cout << "OK" << endl;
		delete generate;
	}
	return 0;
}
