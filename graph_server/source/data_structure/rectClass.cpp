#pragma once
#include "refCounter.cpp"
#include "posClass.cpp"

class rectClass : public refCounter{
public:

    float x;
    float y;
    float w;
    float h;
    float area;
    posClass pos;
    
    explicit rectClass():refCounter("rectClass"){
        
    }
    
    explicit rectClass(const rectClass&):refCounter("rectClass"){
        cout << "rectClass&" << endl;
    }
    
    explicit rectClass(const rectClass&&):refCounter("rectClass"){
        cout << "rectClass&&" << endl;
    }
    
    explicit rectClass(float px,float py,float pw,float ph):refCounter("rectClass"){
        x = px;
        y = py;
        w = pw;
        h = ph;
        area = w * h;
    }
    
    void setValue(float px,float py,float pw,float ph){
        x = px;
        y = py;
        w = pw;
        h = ph;
        area = w * h;
    }

    ~rectClass(){

    }
};
