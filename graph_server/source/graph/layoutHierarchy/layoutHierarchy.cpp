#pragma once
#include "computeLeaves.cpp"

void layoutHierarchy_depthTravel(nodeEntity *node,int unit, int parentX){
    vector<nodeEntity*> children = node->wt_children;
    int leaves = node->wt_leaves;
    int level = node->wt_level - 1;
    int currentX = 0;
    
    node->mCenterX = parentX + unit * leaves / 2;
    node->mCenterY = unit * level;
    
    if(children.size() > 0){
        for(nodeEntity *child:children){
            layoutHierarchy_depthTravel(child,unit, parentX + currentX);
            currentX += unit * child->wt_leaves;
        }
    }
    node->deleteExtra();
}

void layoutHierarchy(requestParam &requestParamObjt){
    computeLeaves(requestParamObjt.graphObj.forestResult);
    int treeOffsetX = 0;
    int unit = requestParamObjt.unit;
    
    for(tree *treePtr:requestParamObjt.graphObj.forestResult.trees){
        nodeEntity *root = treePtr->nodes[0];
        layoutHierarchy_depthTravel(root,unit, treeOffsetX);
        treeOffsetX += unit * root->wt_leaves;
    }
}



