#pragma once

#include <unistd.h>
#include <vector>
#include <list>
#include <ostream>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <sys/stat.h>
#include <sys/types.h>
#include <future>
#ifndef WEBASSEMBLY
    #ifdef MAC_OS
        #include <opencv2/opencv.hpp>
        using namespace cv;
    #endif
#endif
using namespace std;
using namespace chrono;
#define UNUSED(x) (void)x

#include "util/util.cpp"

//debug
bool debugPrint = false;
bool innerDebugPrint = false;
bool httpTestSaveImg = false;
const bool drawNodeName  = false;//绘制节点名字
const float printNum = 10000;//节点数超过这个树就打印测试

//http
const int responseSuccess = 0;
const int responseErr = 1;
const string codeKey = "code";
const string msgKey = "msg";

//yifanhu相关
const int default_http_port = 8000;//默认端口号
#ifdef OPEN_THREAD
const int hardware_concurrency = std::max(1u, std::thread::hardware_concurrency());//池线程里线程数
#endif
const bool yifanhuAlawyRun = false;//一直计算直到达到平衡的能量
const int defaultPointXYScale = 100;//初始节点坐标放大系数

//webassembly
const int maxThreadNum = 2;
