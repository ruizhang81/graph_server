#pragma once
#include "../../data_structure/z_include.h"

void interpolatesAngleNew(requestParam &requestParamObj,
                    int totalNum,
                    vector<vector<float>> &angles,
                    vector<float> &angleInputs) {

    float firstLevelMinAngleSpacing = requestParamObj.firstLevelMinAngleSpacing;
    float angleAdjustmentRatio = requestParamObj.angleAdjustmentRatio;
    float angleSpacingRatio = requestParamObj.angleSpacingRatio;
    bool angleWellDistributed = requestParamObj.angleWellDistributed;
    bool radiusWellDistributed = requestParamObj.radiusWellDistributed;
    
    int level;
    float radius;
    float firstLevelAngleStep;
    float currentLevelAngleStep;
    float minCurrentLevelAngleStep;
    float realCurrentLevelAngleStep;
    
    vector<vector<float>> angleRanges;
    
    float slotStart;
    float slotEnd;
    int len;
    int i;
    bool isCircle = false;
    
    
    for(float angle : angleInputs) {
        if (isnan(angle)){
            cout << "angleInput is isnan" << endl;
            return;
        }
    };
    if (angleInputs.size() <= 0 ) {
        vector<float> angleRange_inner;
        angleRange_inner.push_back(-0.5 * M_PI);
        angleRange_inner.push_back(1.5 * M_PI);
        angleRanges.push_back(move(angleRange_inner));
        isCircle = true;
    }else{
        sort(angleInputs.begin(),angleInputs.end(),[&](float a,float b){
            return a < b ;
        });
        len = (int)angleInputs.size();
        for(i = 0; i < len; i++ ) {
            slotStart = angleInputs[i];
            if(i < len - 1){
                slotEnd = angleInputs[i+1];
            }else{
                slotEnd = angleInputs[0] + 2 * M_PI;
            }
            
//            cout << "slotStart:" << slotStart <<  " slotEnd:" << slotEnd << endl;
//            cout << (slotEnd - slotStart) <<  " " << (2 * firstLevelMinAngleSpacing) << endl;
            if ( (slotEnd - slotStart) > 2 * firstLevelMinAngleSpacing ) {
                vector<float> angleRange_inner;
                angleRange_inner.push_back(slotStart);
                angleRange_inner.push_back(slotEnd);
                angleRanges.push_back(move(angleRange_inner));
            }
        }
        
        // the bigger space first
        sort(angleRanges.begin(),angleRanges.end(),[&](vector<float> a,vector<float> b ){
            return (b[1] - b[0])  < (a[1] - a[0]);
        });
        
    }
    
//    cout << "angleInputs.size():" << angleInputs.size() << " angleRanges.size()" << angleRanges.size() << endl;
    if (angleRanges.size() > 0 ) {
        
        firstLevelAngleStep = requestParamObj.nodeOccupiedSpace / ( 2 * requestParamObj.radiusStep );
        realCurrentLevelAngleStep = currentLevelAngleStep = firstLevelAngleStep;
        vector<float> currentLevelAngles;
        
//        cout << currentLevelAngleStep << endl;
        
        
        if(isCircle){
            angleSpacingRatio = 0;
        }
        
        int i = 0; // trace how many nodes
        level = 1;
        
        while ( true ) {
            
            for(vector<float> range : angleRanges) {
                float start = range[0];
                float end = range[1];
                float mid = ( start + end ) / 2;
                int k = 1;
                float angleOffset;
                float angleAdjustment = currentLevelAngleStep
                                        * ( (level % 2 == 0) ? 1 : -1 )
                                        * ( (level == 2) ? 0.5 : angleAdjustmentRatio )
                                        * ( (level == 1) ? 0 : 1 );
                bool breakFlag = false;
                if ( angleWellDistributed ) {
                    angleAdjustment = currentLevelAngleStep / 2 * ( (level == 1) ? 0 : 1 );
                }
                float midAdjusted = mid + angleAdjustment;
                currentLevelAngles.push_back(midAdjusted);
                
//                cout << start << " "
//                << end << " "
//                << mid << " "
//                << angleAdjustment << " "
//                << midAdjusted << endl;
                
                i++;
                while(true) {
                    angleOffset = realCurrentLevelAngleStep * k;
//                    cout << "angleOffset:" << angleOffset  << " realCurrentLevelAngleStep:" << realCurrentLevelAngleStep << " K:"<< k  <<" angleSpacingRatio:" << angleSpacingRatio<< endl;
                    breakFlag = true;
                    if ( (midAdjusted + angleOffset)
                        <= (end - realCurrentLevelAngleStep * angleSpacingRatio) ) {
                        currentLevelAngles.push_back( midAdjusted + angleOffset );
                        i++;
                        breakFlag = false;
//                        cout << "*1:" << midAdjusted + angleOffset << endl;
                    }
                    
                    // if circular, we need to prevent node overlapping.
                    if ( isCircle ) {
                        if ( (midAdjusted - angleOffset)
                            > (start + realCurrentLevelAngleStep * angleSpacingRatio) ) {
                            currentLevelAngles.push_back( midAdjusted - angleOffset );
                            i++;
                            breakFlag = false;
//                            cout << "*2:" << midAdjusted - angleOffset << endl;
                        }
                    }else{
                        if ( (midAdjusted - angleOffset)
                            >= (start + realCurrentLevelAngleStep * angleSpacingRatio) ) {
                            currentLevelAngles.push_back( midAdjusted - angleOffset );
                            i++;
                            breakFlag = false;
//                            cout << "*3:" << midAdjusted - angleOffset << endl;
                        }
                    }
                    
                    if ( !breakFlag ) {
                        k++;
                    }else {
                        break;
                    }
                    
                }
            }
            
            angles.push_back(currentLevelAngles);
            
            if ( i > totalNum ) {
                break;
            }
            level++;
            if ( level > 2 ) {
//                cout << "level > 2" << endl;
                if ( radiusWellDistributed ) {
                    // 2r, 3r, 4r, 5r, ...
                    radius = ( level + 1 ) * requestParamObj.radiusStep;
                }
                else {
                    // 2r, 3r, 5r, 7r, ...
                    radius = ( 2 * level - 1 ) * requestParamObj.radiusStep;
                }
                
                if(angleWellDistributed){
                    minCurrentLevelAngleStep = requestParamObj.nodeOccupiedSpace / radius;
                    currentLevelAngleStep /= 2;
                    realCurrentLevelAngleStep = currentLevelAngleStep;
                    while( realCurrentLevelAngleStep < minCurrentLevelAngleStep ) {
                        realCurrentLevelAngleStep += currentLevelAngleStep;
                    }
                }else{
                    currentLevelAngleStep = requestParamObj.nodeOccupiedSpace / radius;
                    realCurrentLevelAngleStep = currentLevelAngleStep;
                }
            }
            currentLevelAngles.clear();
        }
    }
    
    
    for(vector<float> angleRange:angleRanges){
        clearVectorR(angleRange);
    }
    clearVectorR(angleRanges);
}

