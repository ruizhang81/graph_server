#pragma once
#include "../data_structure/z_include.h"
#include "../../lib/json/include/rapidjson/document.h"
#include "../../lib/json/include/rapidjson/prettywriter.h"
#include "../../lib/json/include/rapidjson/stringbuffer.h"
#include "../../lib/json/include/rapidjson/filewritestream.h"
#include "../../lib/json/include/rapidjson/ostreamwrapper.h"

using namespace rapidjson;


//参数
const char* url_json = "url";

//config
const char* threadPollType_json = "threadPollType";
const char* isCreateForestFun_json = "isCreateForestFun";
const char* isSortDegree_json = "isSortDegree";
const char* isLayoutHierarchy_json = "isLayoutHierarchy";
const char* isYifanhu_json = "isYifanhu";
const char* clusterAllLeaves_json = "clusterAllLeaves";

//yifanhu count
const char* is_yifanhu_3d_json = "yifanhu_3d";
const char* stepRatio_json = "stepRatio";
const char* relativeStrength_json = "relativeStrength";
const char* repulsionRatioWithCluster_json = "repulsionRatioWithCluster";
const char* optimalDistance_json = "optimalDistance";
const char* layoutBalanced_json = "layoutBalanced";
const char* initialStep_json = "initialStep";
const char* quadTreeMaxLevel_json = "quadTreeMaxLevel";
const char* barnesHutTheta_json = "barnesHutTheta";
const char* convergenceThreshold_json = "convergenceThreshold";
const char* maxIterations_json = "maxIterations";
const char* gridXSize_json = "gridXSize";
const char* gridYSize_json = "gridYSize";

//cluster相关
const char* layoutStarThreshold_json = "layoutStarThreshold";
const char* layoutStarRadius_json = "layoutStarRadius";
const char* radiusStep_json = "radiusStep";
const char* nodeOccupiedSpace_json = "nodeOccupiedSpace";
const char* firstLevelMinAngleSpacing_json = "firstLevelMinAngleSpacing";
const char* angleWellDistributed_json = "angleWellDistributed";
const char* radiusWellDistributed_json = "radiusWellDistributed";
const char* angleAdjustmentRatio_json = "angleAdjustmentRatio";
const char* angleSpacingRatio_json = "angleSpacingRatio";
const char* randomRadius_json = "randomRadius";
const char* sortMethod_json = "sortMethod";

//tree
const char* layoutBalancedSpace_json = "layoutBalancedSpace";


inline string string2RequestParam(requestParam &requestParamObj,const char *requestString,bool needUrl){
    Document document;
    size_t size = strlen(requestString);
    document.Parse(requestString,size);
    
    if (!document.IsObject()){
        return "json解析失败";
    }
    
    if(document.HasMember(url_json)){
        requestParamObj.url = document[url_json].GetString();
    }else{
        if(needUrl){
            return "必须包含url字段";
        }
    }
    
    if(document.HasMember(threadPollType_json)){
        requestParamObj.threadPollType = document[threadPollType_json].GetInt();
    }
    if(document.HasMember(isCreateForestFun_json)){
        requestParamObj.isCreateForestFun = document[isCreateForestFun_json].GetBool();
    }
    if(document.HasMember(isSortDegree_json)){
        requestParamObj.isSortDegree = document[isSortDegree_json].GetBool();
    }
    if(document.HasMember(isLayoutHierarchy_json)){
        requestParamObj.isLayoutHierarchy = document[isLayoutHierarchy_json].GetBool();
    }
    if(document.HasMember(isYifanhu_json)){
        requestParamObj.isYifanhu = document[isYifanhu_json].GetBool();
    }
    if(document.HasMember(clusterAllLeaves_json)){
        requestParamObj.clusterAllLeaves = document[clusterAllLeaves_json].GetBool();
    }
    
    //yifanhu
    if(document.HasMember(is_yifanhu_3d_json)){
        requestParamObj.yifanhu_3d = document[is_yifanhu_3d_json].GetBool();
    }
    if(document.HasMember(stepRatio_json)){
        requestParamObj.stepRatio = document[stepRatio_json].GetFloat();
    }
    if(document.HasMember(relativeStrength_json)){
        requestParamObj.relativeStrength = document[relativeStrength_json].GetFloat();
    }
    if(document.HasMember(repulsionRatioWithCluster_json)){
        requestParamObj.repulsionRatioWithCluster = document[repulsionRatioWithCluster_json].GetFloat();
    }
    if(document.HasMember(optimalDistance_json)){
        requestParamObj.optimalDistance = document[optimalDistance_json].GetInt();
    }
    if(document.HasMember(layoutBalanced_json)){
        requestParamObj.layoutBalanced = document[layoutBalanced_json].GetBool();
    }
    if(document.HasMember(initialStep_json)){
        requestParamObj.initialStep = document[initialStep_json].GetFloat();
    }
    if(document.HasMember(quadTreeMaxLevel_json)){
        requestParamObj.quadTreeMaxLevel = document[quadTreeMaxLevel_json].GetInt();
    }
    if(document.HasMember(barnesHutTheta_json)){
        requestParamObj.barnesHutTheta = document[barnesHutTheta_json].GetFloat();
    }
    if(document.HasMember(convergenceThreshold_json)){
        requestParamObj.convergenceThreshold = document[convergenceThreshold_json].GetFloat();
    }
    if(document.HasMember(maxIterations_json)){
        requestParamObj.maxIterations = document[maxIterations_json].GetInt();
    }
    if(document.HasMember(gridXSize_json)){
        requestParamObj.gridXSize = document[gridXSize_json].GetFloat();
    }
    if(document.HasMember(gridYSize_json)){
        requestParamObj.gridYSize = document[gridYSize_json].GetFloat();
    }

    
    //cluster
    if(document.HasMember(layoutStarThreshold_json)){
        requestParamObj.layoutStarThreshold = document[layoutStarThreshold_json].GetFloat();
    }
    if(document.HasMember(layoutStarRadius_json)){
        requestParamObj.layoutStarRadius = document[layoutStarRadius_json].GetFloat();
    }
    if(document.HasMember(radiusStep_json)){
        requestParamObj.radiusStep = document[radiusStep_json].GetFloat();
    }
    if(document.HasMember(nodeOccupiedSpace_json)){
        requestParamObj.nodeOccupiedSpace = document[nodeOccupiedSpace_json].GetFloat();
    }
    if(document.HasMember(firstLevelMinAngleSpacing_json)){
        requestParamObj.firstLevelMinAngleSpacing = document[firstLevelMinAngleSpacing_json].GetFloat();
    }
    if(document.HasMember(angleWellDistributed_json)){
        requestParamObj.angleWellDistributed = document[angleWellDistributed_json].GetBool();
    }
    if(document.HasMember(radiusWellDistributed_json)){
        requestParamObj.radiusWellDistributed = document[radiusWellDistributed_json].GetBool();
    }
    if(document.HasMember(angleAdjustmentRatio_json)){
        requestParamObj.angleAdjustmentRatio = document[angleAdjustmentRatio_json].GetFloat();
    }
    if(document.HasMember(angleSpacingRatio_json)){
        requestParamObj.angleSpacingRatio = document[angleSpacingRatio_json].GetFloat();
    }
    if(document.HasMember(randomRadius_json)){
        requestParamObj.randomRadius = document[randomRadius_json].GetBool();
    }
    if(document.HasMember(sortMethod_json)){
        requestParamObj.sortMethod = document[sortMethod_json].GetInt();
    }
    
    //tree
    if(document.HasMember(layoutBalancedSpace_json)){
        requestParamObj.layoutBalancedSpace = document[layoutBalancedSpace_json].GetFloat();
    }
    
    
#ifdef MAC_OS
    requestParamObj.print();
#endif
    
    document.GetAllocator().Clear();
    return "";
}

