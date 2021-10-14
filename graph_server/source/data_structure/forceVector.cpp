#pragma once
#include "refCounter.cpp"

class forceVector:public refCounter{
public:
    float x;
    float y;
    float z;

    explicit forceVector(float px, float py,float pz):refCounter("forceVector") {
        x = px;
        y = py;
        z = pz;
    }

    explicit forceVector():refCounter("forceVector") {
        x = 0;
        y = 0;
        z = 0;
    }

    explicit forceVector(const forceVector& forceVectorItem):refCounter("forceVector"){
        cout << "forceVector&" << endl;
        x = forceVectorItem.x;
        y = forceVectorItem.y;
        z = forceVectorItem.z;
    }
    
    explicit forceVector(const forceVector&& forceVectorItem):refCounter("forceVector"){
        cout << "forceVector&&" << endl;
        x = forceVectorItem.x;
        y = forceVectorItem.y;
        z = forceVectorItem.z;
    }
    
    ~forceVector() {
        
    }
    
    void setValue(float px, float py,float pz) {
        x = px;
        y = py;
        z = pz;
    }
    
    void reset() {
        x = 0;
        y = 0;
        z = 0;
    }

    void add(forceVector &f) {
        x += f.x;
        y += f.y;
        z += f.z;
    }

    void multiply(float multiplier,float repulsionRatioWithCluster) {
        float temp = pow(multiplier,repulsionRatioWithCluster);
        x *= temp;
        y *= temp;
        z *= temp;
    }

    void subtract(forceVector &f) {
        x -= f.x;
        y -= f.y;
        z -= f.z;
    }

    float getEnergy() {
        return x * x + y * y + z * z;
    }

    float getNorm() {
        return sqrt(getEnergy());
    }

    void normalize(forceVector &forceVectorPtr) {
        float norm = getNorm();
        forceVectorPtr.setValue(x / norm, y / norm, z / norm);
    }

};

