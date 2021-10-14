#pragma once
#include "../../data_structure/z_include.h"

class gridClass :public refCounter
{
private:
    float xSize;
    float ySize;
    vector<vector<float>> grid;
    
    rectClass *getMinRect(){
        int wMax = 0;
        int hMax = 0;
        for(int i=0; i<ySize; i++){
            for(int j=0; j<xSize; j++){
                if(grid[i][j]){
                    wMax = max(j + 1, wMax);
                    hMax = max(i + 1, hMax);
                }
            }
        }
        
        if(wMax>0 && hMax> 0){
            return new rectClass(0,0,wMax,hMax);
        }else{
            return nullptr;
        }
    }
    
    rectClass *getMaxSpareRect(rectClass *minRect){
        int iStart, jStart;
        float xRatio, yRatio;
        char X='X';
        char Y='Y';
        char XY='Z';
        char expandDirection;
        if(nullptr == minRect){
            return new rectClass(0,0,xSize,ySize);
        }else{
            jStart = minRect->x + minRect->w - 1;
            iStart = minRect->y + minRect->h - 1;
            
            xRatio = minRect->w / xSize;
            yRatio = minRect->h / ySize;
            
            if(xRatio < yRatio){
                expandDirection = X;
            }else if(yRatio < xRatio){
                expandDirection = Y;
            }else {
                expandDirection = XY;//XY
            }
            
            if(grid[iStart][jStart]){
                if(iStart < ySize - 1 && expandDirection != X){
                    iStart++;
                }
                
                if(jStart < xSize - 1 && expandDirection != Y){
                    jStart++;
                }
                
                if(iStart == ySize - 1 && jStart == xSize - 1) {
                    return nullptr;
                }
            }
            
            float w = 0;
            float h = 0;
            float wMax = __FLT_MAX__;
            int i,j;
            vector<rectClass*> areas;
            
            for(i=iStart; i >= 0; i--){
                h++;
                w = 0;
                for(j=jStart; j >= 0; j--){
                    w++;
                    if(grid[i][j]){
                        wMax = min(w - 1, wMax);
                        rectClass *rc = new rectClass(jStart - wMax + 1,i,wMax,h);
                        areas.push_back(rc);
                        break;
                    }
                }
                if(j < 0){
                    wMax = min(w, wMax);
                    rectClass *rc = new rectClass(0,i,wMax,h);
                    areas.push_back(rc);
                }
            }
            
            
            sort(areas.begin(),areas.end(),[](const rectClass *rect1,const rectClass *rect2){
                return rect2->area < rect1->area;
            });
            
            rectClass *rectTemp = areas[0];
            rectClass *maxSpareRect = new rectClass(rectTemp->x,rectTemp->y,rectTemp->w,rectTemp->h);
            clearVector(areas);
            return maxSpareRect;
            
        }
    }
    
    void addBlock(tree *treePtr){
        rectClass *rect = &(treePtr->rect);
        posClass *pos = &(rect->pos);
        for(int i = pos->y; i < pos->y + rect->h && i< ySize; i++){
            for(int j = pos->x; j < pos->x + rect->w && j < xSize; j++){
                if(grid[i][j] == 0){
                    grid[i][j] = 1;
                }else {
                    string message = "grid[' + i + '][' + j + '] is occupied!";
                    cout << message << endl;
                    throw message;
                }
            }
        }
    }
public:
    
    explicit gridClass(requestParam &requestParamObj):refCounter("gridClass"){
        xSize = requestParamObj.gridXSize;
        ySize = requestParamObj.gridYSize;
        
        for(int i=0; i<ySize; i++){
            vector<float> v;
            for(int j=0; j<xSize; j++){
                v.push_back(0);
            }
            grid.push_back(move(v));
        }
    }
    
    explicit gridClass(const gridClass&):refCounter("gridClass"){
        cout << "gridClass&" << endl;
    }
    
    explicit gridClass(const gridClass&&):refCounter("gridClass"){
        cout << "gridClass&&" << endl;
    }
    
    ~gridClass(){
        if(!grid.empty()){
            for (vector<float> item:grid) {
                if (!item.empty()) {
                    item.clear();
                }
            }
            grid.clear();
        }
    }
    
    void placeBlock(tree *treePtr){
        rectClass *minRect = getMinRect();
        posClass *pos = &(treePtr->rect.pos);
        
        if(nullptr == minRect){
            pos->x = 0;
            pos->y = 0;
        }else {
            rectClass *maxSpareRect = getMaxSpareRect(minRect);
            if(nullptr == maxSpareRect) {
                pos->x = minRect->x + minRect->w;
                pos->y = 0;
            }else {
                pos->x = maxSpareRect->x;
                pos->y = maxSpareRect->y;
                delete maxSpareRect;
                maxSpareRect = nullptr;
            }
            delete minRect;
            minRect = nullptr;
        }
        addBlock(treePtr);
    }
    
    
};

