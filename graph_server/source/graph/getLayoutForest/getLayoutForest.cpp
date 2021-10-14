#pragma once
#include "widthTravel.cpp"


void addNode(nodeEntity *keyNode,
             edge *edgeItem,
             unordered_map<nodeEntity *, vector<edge*>> &networkMap){
    if(networkMap.find(keyNode) == networkMap.end()){
        networkMap[keyNode] = vector<edge*>();
    }
    networkMap[keyNode].push_back(edgeItem);
}


void buildForest(requestParam &requestParamObj){
    
    unordered_map<nodeEntity *, vector<edge*>> networkMap;//节点，和该节点连接的边集合
    unordered_map<nodeEntity *, int> visitedNodesMap;//到达过的点
    unordered_map<edge *, int> visitedEdgesMap;//到达过的边
    list<nodeEntity *> queue;
    size_t nodeSize = requestParamObj.graphObj.nodes.size();
    size_t edgeSize = requestParamObj.graphObj.edges.size();
    
    networkMap.reserve(nodeSize);
    networkMap.rehash(nodeSize);
    visitedNodesMap.reserve(nodeSize);
    visitedNodesMap.rehash(nodeSize);
    visitedEdgesMap.reserve(edgeSize);
    visitedEdgesMap.rehash(edgeSize);
    
    //构造map
    for(edge *edgeItem : requestParamObj.graphObj.edges){
        addNode(edgeItem->mSource,edgeItem,networkMap);
        addNode(edgeItem->mTarget,edgeItem,networkMap);
    }
    
    //广度遍历
    for(nodeEntity *node:requestParamObj.graphObj.nodes){
        if( visitedNodesMap.find(node) == visitedNodesMap.end()){
            visitedNodesMap[node] = 1;
            tree *treePtr = getTree(networkMap,visitedNodesMap,visitedEdgesMap,node);
            requestParamObj.graphObj.forestResult.trees.push_back(treePtr);
        }
    }
    
    //回收
    for (auto& child: networkMap){
        vector<edge*> &childMap = child.second;
        childMap.clear();
    }
    clearMap(networkMap);
    clearMap(visitedNodesMap);
    clearMap(visitedEdgesMap);
}



void createForestFun(requestParam &requestParamObj){
    
    auto start = getTimeStamp(innerDebugPrint);
    buildForest(requestParamObj);
    
    if(innerDebugPrint){
        
        unordered_map<long, int> forestInfoMap;
        for(tree *treePtr:requestParamObj.graphObj.forestResult.trees){
            long id = treePtr->nodes.size();
            if(forestInfoMap.find(id) == forestInfoMap.end()){
                forestInfoMap[id] = 1;
            }else{
                forestInfoMap[id] =  forestInfoMap[id] + 1;
            }
        }
        
        int count = 0;
        for (auto& child: forestInfoMap){
            if(innerDebugPrint){
                cout <<  "节点" << child.first  << "的树有" << child.second << "个" << endl;
            }
            count += (child.second * child.first);
        }
        
        clearMap(forestInfoMap);
        
        if(count != (int)requestParamObj.graphObj.nodes.size()){
            throw "生成树失败";
        }else{
            cout << "节点:"<< requestParamObj.graphObj.nodes.size() << "个,边:" << requestParamObj.graphObj.edges.size() << "条; "
            << "一共生成:" << requestParamObj.graphObj.forestResult.trees.size() << " 棵树;" <<endl;
            GetCostTime("费时",start);
        }
    }
    
}





