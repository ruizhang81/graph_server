#pragma once
#include "../../data_structure/z_include.h"

void getChildren(unordered_map<nodeEntity *, vector<edge*>> &networkMap,
                 unordered_map<nodeEntity *, int> &visitedNodesMap,
                 unordered_map<edge *, int> &visitedEdgesMap,
                 nodeEntity *root,
                 tree *tree,
                 nodeEntity *node,
                 vector<nodeEntity*> &children){
    if(networkMap.find(node) != networkMap.end()){
        vector<edge*> &edges = networkMap[node];
        for(edge *edgeItem:edges) {
            nodeEntity *childNode = edgeItem->mSource;
            if(node == childNode){
                childNode = edgeItem->mTarget;
            }
            if(childNode != NULL && childNode != nullptr){
                if( visitedNodesMap.find(childNode) == visitedNodesMap.end()){
                    visitedNodesMap[childNode] = 1;
                    childNode->wt_level = node->wt_level + 1;
                    root->wt_maxlevel = max(childNode->wt_level, root->wt_maxlevel );
                    children.push_back(childNode);
                }
                if( visitedEdgesMap.find(edgeItem) == visitedEdgesMap.end()){
                    visitedEdgesMap[edgeItem] = 1;
                    tree->edges.push_back(edgeItem);
                }
            }
        }
    }
}


tree *getTree(unordered_map<nodeEntity *, vector<edge*>> &networkMap,
                   unordered_map<nodeEntity *, int> &visitedNodesMap,
                   unordered_map<edge *, int> &visitedEdgesMap,
                   nodeEntity *root){
    list<nodeEntity *> queue;
    queue.push_back(root);
    tree *treePtr = new tree();
    
    root->wt_level = 1;
    while(!queue.empty()){
        nodeEntity* node = queue.front();
        queue.pop_front();
        treePtr->nodes.push_back(node);
        
        vector<nodeEntity*> children;
        getChildren(networkMap,
                    visitedNodesMap,
                    visitedEdgesMap,
                    root,
                    treePtr,
                    node,
                    children);
        queue.insert(queue.end(),children.begin(),children.end());
        node->wt_children.insert(node->wt_children.end(),children.begin(),children.end());
        children.clear();
    }
    queue.clear();
    return treePtr;
}












