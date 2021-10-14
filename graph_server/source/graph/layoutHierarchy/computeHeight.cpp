#pragma once
#include "../../data_structure/z_include.h"

int computeHeight_depthTravel(nodeEntity *node) {
    int subTreeHeight = 0;
    vector<nodeEntity*> children = node->wt_children;
    for(nodeEntity *child:children){
        subTreeHeight = max( subTreeHeight, computeHeight_depthTravel( child ));
    }
    node->wt_height = subTreeHeight + 1;
    return node->wt_height;
}


void computeHeight(forest &forestResult){
    for(graphTree *tree:forestResult.trees){
        nodeEntity *root = tree->nodes[0];
        computeHeight_depthTravel(root);
    }
    
}
