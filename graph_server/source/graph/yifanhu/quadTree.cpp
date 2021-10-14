#pragma once
#include "../../data_structure/z_include.h"

class quadTree : public nodeEntity {
public:
    int mass;  // Mass of this tree (the number of nodeEntitys it contains)
    int maxLevel;
    float size;
    bool isLeaf;
    int addType;
    quadTree* children[8] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    
    virtual bool addnodeEntity(nodeEntity *nodeEntityItem) {
        UNUSED(nodeEntityItem);
        return false;
    }
    
    virtual ~quadTree(){
        for(quadTree *tree:children){
            if(nullptr != tree){
                delete tree;
                tree = nullptr;
            }
        }
    }
    
    bool addToChildren(nodeEntity *nodeEntityItem) {
        for (quadTree *q : children) {
            if( nullptr != q){
                if (q->addnodeEntity(nodeEntityItem)) {
                    return true;
                }
            }
        }
        return false;
    }
};


