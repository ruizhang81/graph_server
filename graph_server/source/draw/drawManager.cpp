#pragma once
#include "draw2d.cpp"
#include "draw3d.cpp"


void drawPic(requestParam &requestParamObj){
#ifndef WEBASSEMBLY
#ifdef MAC_OS
    if(innerDebugPrint){
        cout << "drawPic" << endl;
    }
    if(requestParamObj.isDraw3d){
        draw3d(requestParamObj);
    }else{
        draw2d(requestParamObj);
    }
#endif
#endif
    
}












