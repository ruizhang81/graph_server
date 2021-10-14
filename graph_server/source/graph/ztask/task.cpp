#pragma once
#include "../z_include.h"

#include "taskAsync.cpp"
#include "taskAsync2.cpp"
#include "taskSync.cpp"


void mySortInner(requestParam &requestParamObj) {
    auto start   = getTimeStamp(innerDebugPrint);
    
    long size = requestParamObj.graphObj.forestResult.trees.size();
    if(innerDebugPrint){
        cout << "start yifanHuAlgorithm: 一共" << size << "棵树"  << endl;
    }
    
    #ifdef OPEN_THREAD
        if(requestParamObj.threadPollType == 1){
            runTaskAsync(&requestParamObj,size);
        }else if(requestParamObj.threadPollType == 2){
            runTaskAsync2(&requestParamObj,size);
        }else{
            runTaskSync(&requestParamObj,size);
        }
    #else
        runTaskSync(&requestParamObj,size);
    #endif

    
    if(innerDebugPrint){
        GetCostTime("总共费时",start);
    }
}
