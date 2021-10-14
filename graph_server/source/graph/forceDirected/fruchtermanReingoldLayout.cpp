#pragma once
#include "../../data_structure/z_include.h"
#include "../../draw/drawManager.cpp"
#include "../yifanhu/quadTree.cpp"
#include "../yifanhu/barnesHut.cpp"

int maxIter = 1000; //算法迭代次数
double k; //节点之间的距离
int ejectFactor;//斥力
int condenseFactor;//引力
int initEjectfactor = 4;//初始斥力
int initCondensefactor = 1;//初始引力


void layoutByForceDirectedInner(vector<nodeEntity *> &nodes, vector<edge *> &edges){
    
    double deltaX, deltaY,deltaZ, deltaLength;
    ejectFactor = initEjectfactor;
    condenseFactor = initCondensefactor;
    
    //2计算每次迭代局部区域内两两节点间的斥力所产生的单位位移（一般为正值）
    for (nodeEntity *nodeV:nodes) {
        nodeV->forceVectorItem.reset();
        for (nodeEntity *nodeU:nodes) {
            if (nodeU != nodeV) {
                deltaX = nodeV->mCenterX - nodeU->mCenterX;
                deltaY = nodeV->mCenterY - nodeU->mCenterY;
                deltaZ = nodeV->mCenterZ - nodeU->mCenterZ;
                deltaLength = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
                if (deltaLength < initEjectfactor * 5){
                    ejectFactor = initEjectfactor / 2;
                }
                if (deltaLength > 0) {
                    forceVector forceV(deltaX / deltaLength * k * k / deltaLength*ejectFactor,
                                       deltaY / deltaLength * k * k / deltaLength*ejectFactor,
                                       deltaZ / deltaLength * k * k / deltaLength*ejectFactor);
                    nodeV->forceVectorItem.add(forceV);
                }
            }
        }
    }
//    float barnesHutTheta = 1.2f;
//    float relativeStrength = 0.2f;
//    float optimalDistance = 100;
//    barnesHut barnesPtr;
//    barnesPtr.setValue(relativeStrength, optimalDistance,barnesHutTheta);
//    quadTree *treePtr = buildTree(nodes, quadTreeMaxLevel);
//    for (nodeEntity *nodeEntity : nodes) {
//        forceVector forceVectorPtr;
//        barnesPtr.bCalculateForce(forceVectorPtr,nodeEntity, treePtr);
//        nodeEntity->forceVectorItem.add(forceVectorPtr);
//    }
    
    //3. 计算每次迭代每条边的引力对两端节点所产生的单位位移（一般为负值）
    for (edge *edgeItem:edges) {
        nodeEntity *mSource = edgeItem->mSource;
        nodeEntity *mTarget = edgeItem->mTarget;
        deltaX = mSource->mCenterX - mTarget->mCenterX;
        deltaY = mSource->mCenterY - mTarget->mCenterY;
        deltaZ = mSource->mCenterZ - mTarget->mCenterZ;
        deltaLength = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
        float xDisp = deltaX * deltaLength / k * condenseFactor;
        float yDisp = deltaY * deltaLength / k * condenseFactor;
        float zDisp = deltaZ * deltaLength / k * condenseFactor;
        forceVector forceV(xDisp,yDisp,zDisp);
        mSource->forceVectorItem.subtract(forceV);
        mTarget->forceVectorItem.add(forceV);
    }
    
    //set x,y
    int maxtx = 4, maxty = 4, maxtz = 4;
    for (nodeEntity *node:nodes) {
        float dx = node->forceVectorItem.x;
        float dy = node->forceVectorItem.y;
        float dz = node->forceVectorItem.z;
        if (dx < -maxtx) dx = -maxtx;
        if (dx > maxtx) dx = maxtx;
        if (dy < -maxty) dy = -maxty;
        if (dy > maxty) dy = maxty;
        if (dz < -maxtz) dz = -maxtz;
        if (dz > maxtz) dz = maxtz;
        
        node->mCenterX += dx;
        node->mCenterY += dy;
        node->mCenterZ += dz;
    }
}


void layoutByForceDirected(forest &forestResult){
    for(tree* tree:forestResult.trees){
        long size = tree->nodes.size();
        if(size>1){
            long sideLength = size * 1;
            k = (sideLength * sideLength) / size;
            for (int i = 0; i < maxIter; i++) {
//                progressPrint(maxIter,i);
//                cout << size << ":" << i << endl;
                layoutByForceDirectedInner(tree->nodes,tree->edges);
            }
        }
    }
}
