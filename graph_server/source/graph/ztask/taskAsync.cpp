#pragma once
#include "taskABase.cpp"

void runTaskAsync(requestParam *requestParamObj,const long size){
    
#ifndef WEBASSEMBLY
    int threadNum = hardware_concurrency;
#else
    int threadNum = maxThreadNum;
#endif
    double step = ((double)size / (double)threadNum);
    step = ceil(step);
//    cout << "taskAsync :"<< step << endl;
    
    vector<future<bool>> futures;
    futures.reserve(threadNum);
    
    long startIndex = 0;
    long endIndex = step;
    
    for(long i=0;i < threadNum; i++){
        
        auto future = async(launch::async,runTaskByThread,requestParamObj,size,startIndex,endIndex,nullptr);

        futures.push_back(move(future));
        
        startIndex += step;
        endIndex += step;

        if(endIndex > size ){
            endIndex = size;
        }
    }
    
    for(auto& future : futures){
        future.wait();
    }
    
    futures.clear();
}
  
