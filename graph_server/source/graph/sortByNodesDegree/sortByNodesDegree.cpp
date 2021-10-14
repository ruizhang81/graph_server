#pragma once
#include "../../data_structure/z_include.h"


void sortByNodesDegree(requestParam &requestParamObj){
    
    for(edge *edgeItem:requestParamObj.graphObj.edges){
        edgeItem->mSource->degree += 1;
        edgeItem->mTarget->degree += 1;
    }
    
    
//    sort(requestParamObj.graphObj.nodes.begin(),requestParamObj.graphObj.nodes.end(),[](nodeEntity *node1,nodeEntity *node2){
//        if (node1->degree > node2->degree){
//            return true;
//        }else{
//            return false;
//        }
//    });
//
    
 
}


