#pragma once
#include "../util/deleteHelp.cpp"


std::unordered_map<string, int> counterMap;

void printAllCCRefCounter(system_clock::time_point startTime){
    #ifdef MAC_OS
        bool allRecycle = true;
        bool first = true;
        auto iter = counterMap.begin();
        while(iter != counterMap.end()){
            string name = iter->first;
            int count = iter->second;
            if(count > 0){
                if(first){
                    std::cout << "回收情况异常:" << std::endl;
                    first = false;
                }
                allRecycle = false;
                std::cout << "异常回收:" << name << ":" << count << std::endl;
            }else if(count < 0){
                allRecycle = false;
                std::cout << "异常回收" << name << ":" << count << std::endl;
            }
            iter++;
        }
        if(allRecycle){
           GetCostTime("全部回收了!!",startTime); 
        }
        clearMap(counterMap);
    #else
        UNUSED(startTime);
    #endif
}

class refCounter
{
public:
    
    string name;
    
#ifdef MAC_OS
    explicit refCounter(string p_name){
        
        
        name = p_name;
        if(counterMap.find(name) == counterMap.end()){
            counterMap[name] = 1;
        }else{
            counterMap[name] = counterMap[name] + 1;
        }
        
    }

    
    ~refCounter(){
        

//        g_lock.lock();
//        if(counterMap[name] > 0){
        if(counterMap.find(name) != counterMap.end()){
            counterMap[name] = counterMap[name] - 1;
        }
        
//        }
        
    }
    
#else
    explicit refCounter(string p_name){
        UNUSED(p_name);
    }
    
    ~refCounter(){
        
    }
#endif
    
    

};


