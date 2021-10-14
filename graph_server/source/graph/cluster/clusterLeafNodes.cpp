#pragma once
#include "../../data_structure/z_include.h"
#include "clustersNodesNew.cpp"

void computeNetworkInfo(vector< edge * > *edges){
    for(edge *edgeEntity:*edges){
        nodeEntity *source = edgeEntity->mSource;
        nodeEntity *target = edgeEntity->mTarget;
        if(source->neighbors.find(target) == source->neighbors.end()){
            source->neighbors[target] = 1;
        }else{
            source->neighbors[target] += 1;
        }
        if(target->neighbors.find(source) == target->neighbors.end()){
            target->neighbors[source] = 1;
        }else{
            target->neighbors[source] += 1;
        }
    }
}



void layoutStar(nodeEntity *root,requestParam &requestParamObj){
    if(innerDebugPrint){
        cout << "layoutStar" << endl;
    }
    vector<nodeEntity *> &leaves = root->leaves;
    size_t size = leaves.size();
    if(size > 1){
        double angle = M_PI / 2;
        double angleStep = 2 * M_PI / size;
        for(nodeEntity *node:leaves){
            node->mCenterX = root->mCenterX + requestParamObj.layoutStarRadius * cos( angle );
            node->mCenterY = root->mCenterY + requestParamObj.layoutStarRadius * sin( angle );
            angle += angleStep;
        }
    }
    
}

float getAngleInput(nodeEntity *fromNode,nodeEntity *toNode){
    float dx = fromNode->mCenterX - toNode->mCenterX;
    float dy = fromNode->mCenterY - toNode->mCenterY;
    float angleInput;

    float sin = dy / sqrt(pow(dx, 2) + pow(dy, 2));
    float cos = dx / sqrt(pow(dx, 2) + pow(dy, 2));
    
    if( sin >= 0 ) {
        angleInput = acos(cos);
    }else {
        angleInput = 2 * M_PI - acos(cos);
    }
    return angleInput;
}

void clusterLeafNodes(requestParam &requestParamObj){
    //获取邻节点
    for(tree *tree:requestParamObj.graphObj.forestResult.trees){
        vector<edge*> *edges = &(tree->edges);
        computeNetworkInfo(edges);
        
        unordered_map<nodeEntity *, int> parentNodesHash;//父节点
        for(edge *edgeEntity:*edges){
            nodeEntity *source = edgeEntity->mSource;
            nodeEntity *target = edgeEntity->mTarget;
            if ( source != target ) {
                if (target->degree == 1 && source->degree > 1) {
                   parentNodesHash[source] = 1;
                }
                if (source->degree == 1 && target->degree > 1 ) {
                   parentNodesHash[target] = 1;
                }
            }else{
//                cout << source->id << " " <<  target->id << endl;
            }
        }
//        cout << parentNodesHash.size() << endl;
        auto iter = parentNodesHash.begin();
        while(iter != parentNodesHash.end()) {
            nodeEntity *bigNode = iter->first;//某一个簇的中心点
   
            unordered_map<nodeEntity*, int> &neighborsMap = bigNode->neighbors;//某个簇的叶子群
            auto neighbors_iter = neighborsMap.begin();
            while(neighbors_iter != neighborsMap.end()){
                nodeEntity *neighbor = neighbors_iter->first;
                if(bigNode->id != neighbor->id){
                    if (neighbor->degree > 1 ) {
                        bigNode->nonLeaves.push_back(neighbor);//叶子超过1的叶子
                    }else if (neighbor->degree > 0 ) {
                        bigNode->leaves.push_back(neighbor);//全部叶子
                    }
                }
                neighbors_iter++;
            }
            
//            cout << "id:" << bigNode->id << " neighbors:"<< bigNode->neighbors.size() << " nonLeaves:" << bigNode->nonLeaves.size() << " leaves:" << bigNode->leaves.size() <<endl;
            
            if (bigNode->nonLeaves.size() == 0 &&
                bigNode->leaves.size() <= requestParamObj.layoutStarThreshold ) {
                layoutStar(bigNode,requestParamObj);//星形布局
            }else{
                vector<float> angleInputs;
                for (nodeEntity *nonleave_node : bigNode->nonLeaves) {
                    float angleInput = getAngleInput(nonleave_node, bigNode);
                    angleInputs.push_back(angleInput);
                }
                clustersNodesNew(requestParamObj,angleInputs,bigNode,bigNode->leaves);
                clearVectorR(angleInputs);
            }
            //        bigNode->deleteExtra();
            iter++;
        }
        clearMap(parentNodesHash);
        
    }
}

