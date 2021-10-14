#pragma once
#include "../data_structure/z_include.h"
#include "../../lib/json/include/rapidjson/document.h"
#include "../../lib/json/include/rapidjson/prettywriter.h"
#include "../../lib/json/include/rapidjson/stringbuffer.h"
#include "../../lib/json/include/rapidjson/filewritestream.h"
#include "../../lib/json/include/rapidjson/ostreamwrapper.h"

using namespace rapidjson;

//node
const char* node_json_id = "id";
const char* node_json_x = "x";
const char* node_json_y = "y";
const char* node_json_z = "z";
//const char* node_json_size = "size";

const char* node_json_x_yfh = "yfh_x";
const char* node_json_y_yfh = "yfh_y";
const char* node_json_z_yfh = "yfh_z";


void getValue(int value,nodeEntity *nodeEntityItem){
    stringstream ss;
    ss << value;
    nodeEntityItem->id = ss.str();
    ss.clear();
}

bool hasValue(string ch){
    return !ch.empty();
}

//json转对象
nodeEntity *json2node(Value &node_json,bool useJsonXY){
    nodeEntity *nodeEntityItem = new nodeEntity();
    if(node_json.HasMember(node_json_id)){
        nodeEntityItem->id = node_json[node_json_id].GetString();
    }else{
        nodeEntityItem->id = "";
    }
    //第一次进来初始化一下坐标
    if(useJsonXY){
        if(node_json.HasMember(node_json_x)){
            nodeEntityItem->mCenterX = node_json[node_json_x].GetFloat();
        }
        if(node_json.HasMember(node_json_y)){
            nodeEntityItem->mCenterY = node_json[node_json_y].GetFloat();
        }
        if(node_json.HasMember(node_json_z)){
            nodeEntityItem->mCenterZ = node_json[node_json_z].GetFloat();
        }
//        if(node_json.HasMember(node_json_size)){
//            nodeEntityItem->size = node_json[node_json_size].GetFloat();
//        }
    }else{
        nodeEntityItem->mCenterX = rand()/double(RAND_MAX) * defaultPointXYScale;
        nodeEntityItem->mCenterY = rand()/double(RAND_MAX) * defaultPointXYScale;
        nodeEntityItem->mCenterZ = rand()/double(RAND_MAX) * defaultPointXYScale;
    }
    return nodeEntityItem;
}



void json2nodelist(vector<nodeEntity *> &nodeList,Value &nodesJson,bool useJsonXY){
    if (nodesJson.IsArray()){
        size_t size = nodesJson.Size();
        srand(time(NULL));
        nodeList.reserve(size);
        for (SizeType i = 0; i < size; i++) {
            Value &node_json = nodesJson[i];
            nodeEntity *nodeEntityItem = json2node(node_json,useJsonXY);
            nodeList.push_back(nodeEntityItem);
        }
    }
}


//对象转json
inline Value node2json(nodeEntity *nodeItem,Document::AllocatorType &allocator,bool yifanhu_3d){
    Value node(kObjectType);
    node.AddMember(StringRef(node_json_id), StringRef(nodeItem->id.c_str()), allocator);
    node.AddMember(StringRef(node_json_x), nodeItem->mCenterX, allocator);
    node.AddMember(StringRef(node_json_y), nodeItem->mCenterY, allocator);
    if(yifanhu_3d){
        node.AddMember(StringRef(node_json_z), nodeItem->mCenterZ, allocator);
    }
    return node;
}

inline Value nodelist2json(vector<nodeEntity *> &nodeList,Document::AllocatorType &allocator,bool yifanhu_3d){
    Value nodesValue(kArrayType);
    for(nodeEntity *nodeItem:nodeList){
        Value valueJson = node2json(nodeItem,allocator,yifanhu_3d);
        nodesValue.PushBack(valueJson, allocator);
    }
    return nodesValue;
}


