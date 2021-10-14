#pragma once
#include "taskABase.cpp"

void runTaskSync(requestParam *requestParamObj,const long size){
//    cout << "taskSync" << endl;
    yifanHuAlgorithm yifanHuAlgorithm_static;
    runTaskByThread(requestParamObj,size,0,size,(void*)&yifanHuAlgorithm_static);
}
