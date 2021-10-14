#pragma once
#include "quadTree.cpp"

class quadTree_2d : public quadTree {
public:
    float posX;
    float posY;
    float centerMassX;
    float centerMassY;
    
    explicit quadTree_2d(float minX, float minY, float pSize, int pMaxLevel) {
        mCenterX = 0;
        mCenterY = 0;
        centerMassX = 0;
        centerMassY = 0;
        posX = minX;
        posY = minY;
        size = pSize;
        maxLevel = pMaxLevel;
        isLeaf = true;
        mass = 0;
        addType = 0;
    }
    
    ~quadTree_2d() {

    }
    
    
    virtual bool addnodeEntity(nodeEntity *nodeEntityItem) {
        if ( nodeEntityItem->mCenterX >= posX &&
                nodeEntityItem->mCenterX <= posX + size &&
                nodeEntityItem->mCenterY >= posY &&
                nodeEntityItem->mCenterY <= posY + size) {
            return addNode(nodeEntityItem);
        } else {
            return false;
        }
    }


    
private:
    void assimilatenodeEntity(nodeEntity *nodeEntityItem) {
        centerMassX = (mass * centerMassX + nodeEntityItem->mCenterX) / (mass + 1);
        centerMassY = (mass * centerMassY + nodeEntityItem->mCenterY) / (mass + 1);
        mCenterX = centerMassX;
        mCenterY = centerMassY;
        mass++;
    }

    void divideTree() {
        float childSize = size / 2;
        children[0] = new quadTree_2d(posX, posY, childSize, maxLevel - 1);
        children[1] = new quadTree_2d(posX + childSize, posY,
                                        childSize, maxLevel - 1);
        children[2] = new quadTree_2d(posX, posY + childSize,
                                        childSize, maxLevel - 1);
        children[3] = new quadTree_2d(posX + childSize, posY + childSize,
                                        childSize, maxLevel - 1);
        isLeaf = false;
    }

    bool addNode(nodeEntity *nodeEntityItem) {
        if(addType == 0){
            mass = 1;
            centerMassX = nodeEntityItem->mCenterX;
            centerMassY = nodeEntityItem->mCenterY;
            mCenterX = centerMassX;
            mCenterY = centerMassY;
            if (maxLevel == 0) {
                addType = 3;
            } else {
                addType = 1;
            }
            return true;
        }else if(addType == 1){
            divideTree();
            addType = 2;
            addToChildren(this);
            return addNode(nodeEntityItem);
        }else if(addType == 2){
            assimilatenodeEntity(nodeEntityItem);
            return addToChildren(nodeEntityItem);
        }else{
            assimilatenodeEntity(nodeEntityItem);
            return true;
        }
        
    }
};




