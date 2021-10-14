#pragma once
#include "springForce.cpp"

class electricalForce:public refCounter{
public:
    requestParam *mRequestParamObj;
    explicit electricalForce():refCounter("electricalForce") {

    }
    explicit electricalForce(const electricalForce&):refCounter("electricalForce") {
        cout << "electricalForce&" << endl;
    }
    explicit electricalForce(const electricalForce&&):refCounter("electricalForce") {
        cout << "electricalForce&&" << endl;
    }
    
    ~electricalForce(){
        mRequestParamObj = nullptr;
    }
    void setValue(requestParam *requestParam){
        mRequestParamObj = requestParam;
    }


    void calculateForce(forceVector &forceVectorPtr,nodeEntity* nodeEntity1, nodeEntity* nodeEntity2) {
        calculateForce(forceVectorPtr,nodeEntity1, nodeEntity2,
                              distance(nodeEntity1, nodeEntity2,mRequestParamObj->yifanhu_3d));
    }

    void calculateForce(forceVector &forceVectorPtr,nodeEntity* nodeEntity1, nodeEntity* nodeEntity2,float distance) {
        if(mRequestParamObj->yifanhu_3d){
            forceVectorPtr.setValue(nodeEntity2->mCenterX - nodeEntity1->mCenterX,
                                    nodeEntity2->mCenterY - nodeEntity1->mCenterY,
                                    nodeEntity2->mCenterZ - nodeEntity1->mCenterZ);
        }else{
            forceVectorPtr.setValue(nodeEntity2->mCenterX - nodeEntity1->mCenterX,
                                    nodeEntity2->mCenterY - nodeEntity1->mCenterY,
                                    0);
        }
        float scale = -mRequestParamObj->relativeStrength * mRequestParamObj->optimalDistance * mRequestParamObj->optimalDistance / (distance * distance);
        if (isnan(scale) == 1 || isinf(scale) == 1) {
//            LOGE("electricalForce native");
            scale = -1;
        }
        forceVectorPtr.multiply(scale,mRequestParamObj->repulsionRatioWithCluster);
    }

};








