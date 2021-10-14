#pragma once
#include "../data_structure/z_include.h"

#ifndef WEBASSEMBLY
#ifdef MAC_OS
void getNodesRect(vector<nodeEntity *> nodes,rectClass &rectClassItem){
    float minX = __FLT_MAX__;
    float maxX = __FLT_MIN__;
    float minY = __FLT_MAX__;
    float maxY = __FLT_MIN__;
    for(nodeEntity *node:nodes){
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
    rectClassItem.setValue(minX,minY,maxX-minX,maxY-minY);
}

void draw2d(requestParam &requestParamObj){
    auto start  = getTimeStamp(true);

    rectClass rectClassItem;
    getNodesRect(requestParamObj.graphObj.nodes, rectClassItem);
    
    int sideLength = max(rectClassItem.w,rectClassItem.h);
    int huge = 50000;
    if(rectClassItem.w > huge || rectClassItem.h > huge){
        sideLength = huge;
    }
    cout << "start draw: 最大边长"<< sideLength << "("<<rectClassItem.x<<","<<rectClassItem.y<<")"<< endl;
    int padding = sideLength/50;
    int realSide = sideLength-padding*2;
    float scaleX = realSide / rectClassItem.w;
    float scaleY = realSide / rectClassItem.h;
    
    for(nodeEntity *node:requestParamObj.graphObj.nodes){
        node->mCenterX_draw = (node->mCenterX - rectClassItem.x) * scaleX + padding;
        node->mCenterY_draw = (node->mCenterY - rectClassItem.y) * scaleY + padding;
    }
    
    Mat img = Mat::zeros(Size(sideLength,sideLength), CV_8UC3);
    img.setTo(255);
    
    auto lineScalar = Scalar(33, 33, 133);
    auto scalar = Scalar(0, 255, 0);
    
    
    for(edge *edgeItem:requestParamObj.graphObj.edges){
        if(edgeItem->mSource == NULL || edgeItem->mSource == nullptr ||
           edgeItem->mTarget == NULL || edgeItem->mTarget == nullptr ){
            continue;
        }
        Point p1(edgeItem->mSource->mCenterX_draw, edgeItem->mSource->mCenterY_draw);
        Point p2(edgeItem->mTarget->mCenterX_draw, edgeItem->mTarget->mCenterY_draw);
        line(img, p1, p2, lineScalar, 1);
    }
    
    for(nodeEntity *node:requestParamObj.graphObj.nodes){
        Point p(node->mCenterX_draw, node->mCenterY_draw);
        circle(img, p, 2,scalar);
        if(drawNodeName){
            putText(img,
                    node->id,
                    p,FONT_HERSHEY_COMPLEX,1,scalar,2,4);
        }
    }

    
    if(httpTestSaveImg){
        string path = "img";
        mkdir(path.c_str(),S_IRWXU);
        
        string timeStr = getTime();
        path.append("/");
        path.append(timeStr);
        path.append(".jpg");
        imwrite(path.c_str(), img);
    }else{
        imshow("生成图形", img);
        waitKey();
    }

    GetCostTime("----end draw ",start);
}
#endif
#endif
