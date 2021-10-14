#pragma once
#include "gridClass.cpp"

void normalizeTreeRect(tree *treePtr,requestParam &requestParamObj){
    int unit = requestParamObj.layoutBalancedSpace;
    rectClass *rect = &(treePtr->rect);
    int extendRatio = 1;
    float w = rect->w;
    float h = rect->h;
    size_t size = treePtr->nodes.size();
    if(size > 1){
        extendRatio = 1;
    }
    rect->w = ceil(w * extendRatio / unit);
    rect->h = ceil(h * extendRatio / unit);
    if(rect->w * unit - w < unit
       && size >= 1){
        rect->w++;
    }
    if(rect->h * unit - h < unit
       && size >= 1){
        rect->h++;
    }
    rect->area = rect->w * rect->h;
}

void computeTreeRect(tree *treePtr){
    float minX = __FLT_MAX__;
    float maxX = __FLT_MIN__;
    float minY = __FLT_MAX__;
    float maxY = __FLT_MIN__;
    
    float size = 0.2;
    for(nodeEntity *node:treePtr->nodes){
//        float size = node->size;
//        if(size == 0){
//            size = 0.2;
//        }
        
        if(node->mCenterX < minX){
            minX = node->mCenterX;
        }
        if(node->mCenterX > maxX){
            maxX = node->mCenterX;
        }
        if(node->mCenterY < minY){
            minY = node->mCenterY;
        }
        if(node->mCenterY > maxY){
            maxY = node->mCenterY;
        }
    }
    minX = minX - size;
    maxX = maxX + size;
    minY = minY - size;
    maxY = maxY + size;
    treePtr->rect.setValue(minX,minY,maxX-minX,maxY-minY);
}

void layoutTreeByGrid(requestParam &requestParamObj){
    gridClass grid(requestParamObj);
    int unit = requestParamObj.layoutBalancedSpace;

    //计算出每个树的矩形
    for(tree *treePtr : requestParamObj.graphObj.forestResult.trees){
        computeTreeRect(treePtr);
    }
    
    //按矩形大小排序，由大到小
    sort(requestParamObj.graphObj.forestResult.trees.begin(),requestParamObj.graphObj.forestResult.trees.end(),[](tree *tree1,tree *tree2){
        return tree2->rect.area < tree1->rect.area;
    });
    
    for(tree *treePtr:requestParamObj.graphObj.forestResult.trees){
        //规格化矩形尺寸
        normalizeTreeRect(treePtr,requestParamObj);
        //插入到grid
        grid.placeBlock(treePtr);
    }

    //更新每个点的坐标
    for(tree *treePtr : requestParamObj.graphObj.forestResult.trees){
        rectClass *rect = &(treePtr->rect);
        float dx = rect->pos.x * unit - rect->x;
        float dy = rect->pos.y * unit - rect->y;

        for(nodeEntity *node:treePtr->nodes){
            node->mCenterX += dx;
            node->mCenterY += dy;
        }
    };

}
