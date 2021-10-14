#pragma once
#include "../z_include.h"

int yifanhu(vector<nodeEntity *> &nodes,
            vector<edge *> &edges,
            requestParam *requestParamObj,
            void* ptr){
    long size = nodes.size();
    if(size == 2){
        //2个点固定布局
        float dis = requestParamObj->optimalDistance;
        nodes[0]->mCenterX = 0;
        nodes[0]->mCenterY = 0;
        nodes[1]->mCenterX = 0;
        nodes[1]->mCenterY = dis;
        return 1;
    }else{
        
//        yifanHuAlgorithm yifanHuAlgorithmObjPtr(&nodes, &edges,requestParamObj);
//        int ifanhuCount = requestParamObj->maxIterations;//最大迭代次数
//        int count = 0;
//        for (int i = 0; i < ifanhuCount; i++) {
//            count = i;
//        if (!yifanHuAlgorithmObjPtr.converged) {
//            yifanHuAlgorithmObjPtr.goAlgo(i);
//        } else {
//            break;
//        }
        
        int ifanhuCount = requestParamObj->maxIterations;//最大迭代次数
        int count = 0;
        if((size > printNum) && innerDebugPrint){
            cout << "该树有" <<size << "节点 最大迭代总次数为:" << ifanhuCount << endl;
        }
        
        if(ptr != nullptr){
            yifanHuAlgorithm *yifanHuAlgorithm_ptr = (yifanHuAlgorithm*)ptr;
            yifanHuAlgorithm_ptr->reset();
            yifanHuAlgorithm_ptr->init(&nodes, &edges,requestParamObj);
            for (int i = 0; i < ifanhuCount; i++) {
                count = i;
                if (!yifanHuAlgorithm_ptr->converged) {
                    yifanHuAlgorithm_ptr->goAlgo(i);
                } else {
                    break;
                }
            }
        }else{
            yifanHuAlgorithm yifanHuAlgorithmObjPtr(&nodes, &edges,requestParamObj);
            for (int i = 0; i < ifanhuCount; i++) {
                count = i;
                if (!yifanHuAlgorithmObjPtr.converged) {
                    yifanHuAlgorithmObjPtr.goAlgo(i);
                } else {
                    break;
                }
            }
        }
        return count+1;
    }
}




bool runTaskByThread(requestParam* requestParamObj,
                      const long size,
                      const long startIndex,
                      const long endIndex,
                      void* ptr){
    for(long i = startIndex;i < endIndex;i++){
        
        tree *treePtr = requestParamObj->graphObj.forestResult.trees[i];
        long tree_node_size = treePtr->nodes.size();
        int count = 0;
        if(tree_node_size > 1){
            auto startInner = getTimeStamp(innerDebugPrint);
            count = yifanhu(treePtr->nodes,treePtr->edges,requestParamObj,ptr);
            if(innerDebugPrint){
                GetCostTime("费时",startInner);
            }
        }
        if(innerDebugPrint){
            cout
            << "共"
            << size
            << "棵树，第"
            << i
            << "棵树计算完成 (节点数:"
            << tree_node_size
            << ")"
            << "迭代次数("
            << count
            << ")"
            << endl;
        }
    }

    return true;

}

