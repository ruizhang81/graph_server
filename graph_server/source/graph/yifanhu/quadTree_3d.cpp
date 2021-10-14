#pragma once
#include "quadTree.cpp"

class quadTree_3d : public quadTree {
public:
    float posX;
    float posY;
    float posZ;
    float centerMassX;
    float centerMassY;
    float centerMassZ;
    
    explicit quadTree_3d(float minX, float minY,float minZ, float pSize, int pMaxLevel) {
        mCenterX = 0;
        mCenterY = 0;
        mCenterZ = 0;
        centerMassX = 0;
        centerMassY = 0;
        centerMassZ = 0;
        posX = minX;
        posY = minY;
        posZ = minZ;
        size = pSize;
        maxLevel = pMaxLevel;
        isLeaf = true;
        mass = 0;
        addType = 0;
    }

    ~quadTree_3d() {
        
    }
    
    virtual bool addnodeEntity(nodeEntity *nodeEntityItem) {
        if ( nodeEntityItem->mCenterX >= posX &&
            nodeEntityItem->mCenterX <= posX + size &&
            nodeEntityItem->mCenterY >= posY &&
            nodeEntityItem->mCenterY <= posY + size &&
            nodeEntityItem->mCenterZ >= posZ &&
            nodeEntityItem->mCenterZ <= posZ + size) {
            return addNode(nodeEntityItem);
        } else {
            return false;
        }
    }
    
    
    
private:
    void assimilatenodeEntity(nodeEntity *nodeEntityItem) {
        centerMassX = (mass * centerMassX + nodeEntityItem->mCenterX) / (mass + 1);
        centerMassY = (mass * centerMassY + nodeEntityItem->mCenterY) / (mass + 1);
        centerMassZ = (mass * centerMassZ + nodeEntityItem->mCenterZ) / (mass + 1);
        mCenterX = centerMassX;
        mCenterY = centerMassY;
        mCenterZ = centerMassZ;
        mass++;
    }
    
    void divideTree() {
        float childSize = size / 2;
        children[0] = new quadTree_3d(posX, posY, posZ,
                                   childSize, maxLevel - 1);
        children[1] = new quadTree_3d(posX + childSize, posY,posZ,
                                   childSize, maxLevel - 1);
        children[2] =new quadTree_3d(posX, posY + childSize,posZ,
                                   childSize, maxLevel - 1);
        children[3] =new quadTree_3d(posX + childSize, posY + childSize, posZ,
                                   childSize, maxLevel - 1);
        children[4] =new quadTree_3d(posX, posY, posZ+childSize,
                                   childSize, maxLevel - 1);
        children[5] =new quadTree_3d(posX + childSize, posY,posZ+childSize,
                                   childSize, maxLevel - 1);
        children[6] =new quadTree_3d(posX, posY + childSize,posZ+childSize,
                                   childSize, maxLevel - 1);
        children[7] =new quadTree_3d(posX + childSize, posY + childSize, posZ+childSize,
                                   childSize, maxLevel - 1);
        isLeaf = false;
    }

    
    
    
    bool addNode(nodeEntity *nodeEntityItem) {
        if(addType == 0){
            mass = 1;
            centerMassX = nodeEntityItem->mCenterX;
            centerMassY = nodeEntityItem->mCenterY;
            centerMassZ = nodeEntityItem->mCenterZ;
            mCenterX = centerMassX;
            mCenterY = centerMassY;
            mCenterZ = centerMassZ;
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


