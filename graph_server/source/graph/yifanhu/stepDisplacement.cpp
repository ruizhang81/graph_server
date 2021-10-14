#pragma once
#include "../../data_structure/z_include.h"


bool assertValue(float value) {
    return !std::isnan(value) && !std::isinf(value);
}

void movenodeEntity(nodeEntity *nodeEntity,float step,requestParam *mRequestParamObj) {
    forceVector forceVectorPtr;
    nodeEntity->forceVectorItem.normalize(forceVectorPtr);
    forceVectorPtr.multiply(step,mRequestParamObj->repulsionRatioWithCluster);
    
    
    float x = nodeEntity->mCenterX + forceVectorPtr.x;
    float y = nodeEntity->mCenterY + forceVectorPtr.y;
    float z = nodeEntity->mCenterZ + forceVectorPtr.z;
    
    if(assertValue(x)){
        nodeEntity->mCenterX = x;
    }
    if(assertValue(y)){
        nodeEntity->mCenterY = y;
    }
    if(mRequestParamObj->yifanhu_3d){
        if(assertValue(z)){
            nodeEntity->mCenterZ = z;
        }
    }else{
        nodeEntity->mCenterZ = 0;
    }
//    adaptive(x,y,nodeEntity, 50000, 50000, 10);
    
    
}


