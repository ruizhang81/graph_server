#pragma once
#include "refCounter.cpp"

class posClass : public refCounter{
    
public:
    float x;
    float y;
    
    explicit posClass(float px,float py):refCounter("posClass"){
        x = px;
        y = py;
    }
    
    explicit posClass():refCounter("posClass"){
        x = 0;
        y = 0;
    }
    
    explicit posClass(const posClass&):refCounter("posClass"){
        cout << "posClass&" << endl;
    }
    
    explicit posClass(const posClass&&):refCounter("posClass"){
        cout << "posClass&&" << endl;
    }
    
    ~posClass(){

    }
};
