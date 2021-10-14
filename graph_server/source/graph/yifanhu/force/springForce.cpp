#pragma once
#include "../../../data_structure/z_include.h"

class springForce :public refCounter{
public:
    requestParam *mRequestParamObj;
    springForce():refCounter("springForce"){}
    
    explicit springForce(const springForce&):refCounter("springForce"){
        cout << "springForce&" << endl;
    }
    
    explicit springForce(const springForce&&):refCounter("springForce"){
        cout << "springForce&&" << endl;
    }
    
    ~springForce(){
        mRequestParamObj = nullptr;
    }
    
    void setValue(requestParam *requestParamObj) {
        mRequestParamObj = requestParamObj;
    }

    void calculateForce(forceVector &forceVectorPtr,nodeEntity* nodeEntity1, nodeEntity* nodeEntity2) {
        calculateForce(forceVectorPtr,nodeEntity1, nodeEntity2,
                              distance(nodeEntity1, nodeEntity2,mRequestParamObj->yifanhu_3d));
    }

    void calculateForce(forceVector &forceVectorPtr,nodeEntity* nodeEntity1, nodeEntity* nodeEntity2,
                                float distance) {
        if(mRequestParamObj->yifanhu_3d){
            forceVectorPtr.setValue(nodeEntity2->mCenterX - nodeEntity1->mCenterX,
                                    nodeEntity2->mCenterY - nodeEntity1->mCenterY,
                                    nodeEntity2->mCenterZ - nodeEntity1->mCenterZ);
        }else{
            forceVectorPtr.setValue(nodeEntity2->mCenterX - nodeEntity1->mCenterX,
                                    nodeEntity2->mCenterY - nodeEntity1->mCenterY,
                                    0);
        }
        forceVectorPtr.multiply(distance / mRequestParamObj->optimalDistance,mRequestParamObj->repulsionRatioWithCluster);
    }

};







