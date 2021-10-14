#pragma once
#include "../json/z_include.h"
#include "z_include.h"
#include "ztask/task.cpp"


void mySort(requestParam &requestParamObj) {
    
    auto start = getTimeStamp(innerDebugPrint);
    //按度排序
    if(requestParamObj.isSortDegree){
        sortByNodesDegree(requestParamObj);
    }
    if(innerDebugPrint){
        GetCostTime("sort finish 按度排序",start);
    }
    
    //生成森林
    forest forestResult;
    if(requestParamObj.isCreateForestFun){
        createForestFun(requestParamObj);
    }else{
        tree *treePtr = new tree();
        treePtr->nodes = requestParamObj.graphObj.nodes;
        treePtr->edges = requestParamObj.graphObj.edges;
        requestParamObj.graphObj.forestResult.trees.push_back(treePtr);
    }
    
    if(innerDebugPrint){
        GetCostTime("sort finish 生成森林",start);
    }
    
    //层次布局
    if(requestParamObj.isLayoutHierarchy){
        layoutHierarchy(requestParamObj);
    }
    
    if(innerDebugPrint){
        GetCostTime("sort finish 层次布局",start);
    }
    
    //infanhu布局计算
    if(requestParamObj.isYifanhu){
        mySortInner(requestParamObj);
    }
    
    if(innerDebugPrint){
        GetCostTime("sort finish infanhu",start);
    }
    
    //加簇布局
    if(requestParamObj.clusterAllLeaves){
        clusterLeafNodes(requestParamObj);
        if(innerDebugPrint){
            GetCostTime("sort finish 加簇布局",start);
        }
    }
    
    
    
    //        layoutByForceDirected(forestResult);
    //        layoutByBoostForceDirected(forestResult,nodesMap); //boost
    
    //森林布局
    if(requestParamObj.layoutBalanced &&
       requestParamObj.graphObj.forestResult.trees.size() > 1){
        layoutTreeByGrid(requestParamObj);
    }
    
    if(innerDebugPrint){
        GetCostTime("----sort finish",start);
    }
}




