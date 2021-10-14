#pragma once
#include "computeLeaves.cpp"
#include "computeHeight.cpp"

void layoutHierarchy2_depthTravel(nodeEntity *node,int unit, int parentX){
    vector<nodeEntity*> children = node->wt_children;
    int leaves = node->wt_leaves;
    int level = node->wt_level - 1;
    int currentX = 0;
    
    node->mCenterX = parentX + unit * leaves / 2;
    node->mCenterY = unit * level;
    
    if(children.size() > 0){
        for(nodeEntity *child:children){
            layoutHierarchy2_depthTravel(child,unit, parentX + currentX);
            currentX += unit * child->wt_leaves;
        }
    }
    node->deleteExtra();
}

void getNodesFromTree_depthTravel(graphTree *tree,vector<nodeEntity *> &nodes){
    UNUSED(tree);
    UNUSED(nodes);
//    nodeEntity *root = tree->nodes[0];
//    nodes.push_back( root );
//    vector<nodeEntity*> children = tree->nodes;
//    if (children.size()>0) {
//        for(nodeEntity *child:children){
//            getNodesFromTree_depthTravel( child ,nodes);
//        }
//    }
}


void computeLENodes(graphTree *tree){
    UNUSED(tree);
//    vector<nodeEntity *> nodes;
//    getNodesFromTree(tree,nodes)
//    , nodeIds = nodes.map( function( node ) {
//        return node.id;
//    } )
//    , subGraph = me.graph.getSubGraph({
//    filter: function( node ) {
//        return nodeIds.indexOf( node.id ) >= 0;
//    }
//    })
//    ;
//    sigma.utils.
//    (
//                                             subGraph
//                                             , tree
//                                             , {
//                                             sortBySubTreeSize: !opt.noSortBySubTreeSize
//                                             }
//                                             );
}


void layoutHierarchy2(forest &forestResult){
    computeLeaves(forestResult);
    computeHeight(forestResult);
    int treeOffsetX = 0;
    int unit =  100;
    
    for(graphTree *tree:forestResult.trees){
        nodeEntity *root = tree->nodes[0];
        layoutHierarchy2_depthTravel(root,unit, treeOffsetX);
        treeOffsetX += unit * root->wt_leaves;
    }
    
}



