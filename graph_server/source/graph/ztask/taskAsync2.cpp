#pragma once
#include "taskABase.cpp"
#include <pthread.h>

void runTaskAsync2(requestParam *requestParamObj,const long size){
    
#ifndef WEBASSEMBLY
    int threadNum = hardware_concurrency;
#else
    int threadNum = maxThreadNum;
#endif
    double step = ((double)size / (double)threadNum);
    step = ceil(step);
//    cout << "taskAsync :"<< step << endl;
    
    vector<std::thread> threads;
    threads.reserve(threadNum);
    long startIndex = 0;
    long endIndex = step;
    
    for(long i=0;i < threadNum; i++){
        
        std::thread t(runTaskByThread, requestParamObj,size,startIndex,endIndex,nullptr);
        threads.push_back(std::move(t));
        
        startIndex += step;
        endIndex += step;

        if(endIndex > size ){
            endIndex = size;
        }
    }
    
    if(innerDebugPrint){
        cout << "runTaskAsync2 thread start" << endl;
    }
    for(thread &threadObj:threads) {
        if (threadObj.joinable()){
            threadObj.join();
        }
    }
    threads.clear();
    
    if(innerDebugPrint){
        cout << "runTaskAsync2 thread finish" << endl;
    }
}

