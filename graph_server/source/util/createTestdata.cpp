#pragma once
#include "../json/z_include.h"

void
fullNodesAndEdges(requestParam &requestParamObj,unsigned long nodeSize,unsigned long edgeSize) {

    srand(time(NULL));
    
    for(unsigned long i = 0; i<nodeSize; i++){
        nodeEntity* node = new nodeEntity();
        node->mCenterX = rand()/double(RAND_MAX);
        node->mCenterY = rand()/double(RAND_MAX);
        if(requestParamObj.yifanhu_3d){
            node->mCenterZ = rand()/double(RAND_MAX);
        }
        getValue(i,node);
        requestParamObj.graphObj.nodes.push_back(node);
    }

    unordered_map<string, int> hashmap;

    while(requestParamObj.graphObj.edges.size() < edgeSize){
        int index = requestParamObj.graphObj.edges.size();
        int sourceNodeindex = (rand()/double(RAND_MAX)) * nodeSize;
        int targetNodeindex = (rand()/double(RAND_MAX)) * nodeSize;
        while(sourceNodeindex == targetNodeindex){
            targetNodeindex = (rand()/double(RAND_MAX)) * nodeSize;
        }
        string str1 = std::to_string(sourceNodeindex);
        string str2 = std::to_string(targetNodeindex);

        string key1 = str1 + str2;
        string key2 = str2 + str1;

        unordered_map<string,int>::iterator  iter1 = hashmap.find(key1);
        unordered_map<string,int>::iterator  iter2 = hashmap.find(key2);

        if( iter1 == hashmap.end() && iter2 == hashmap.end()){
            edge *edgeItem = new edge();
            edgeItem->mSource = requestParamObj.graphObj.nodes[sourceNodeindex];
            edgeItem->mTarget = requestParamObj.graphObj.nodes[targetNodeindex];

            getValue(index,edgeItem);

            requestParamObj.graphObj.edges.push_back(edgeItem);
            hashmap[key1] = 1;
            hashmap[key2] = 1;
        }
    }
    hashmap.clear();
}






