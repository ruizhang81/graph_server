#pragma once
#include "../../data_structure/z_include.h"

int computeLeaves_depthTravel(nodeEntity *node){
    int leaves = 0;

    if(node->wt_children.size() == 0){
        leaves = 1;
    }else {
        for(nodeEntity *child:node->wt_children){
            int leaves_inner = computeLeaves_depthTravel(child);
            leaves += leaves_inner;
        }
    }
    
    node->wt_leaves = leaves;
    return node->wt_leaves;
}


void computeLeaves(forest &forestResult){
    for(tree *treePtr : forestResult.trees){
        nodeEntity *root = treePtr->nodes[0];
        computeLeaves_depthTravel(root);
    }
    
}



