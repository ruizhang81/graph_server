#pragma once
#include "jsonHelpNodeEntity.cpp"

//edge
const char* edge_json_id = "id";
const char* edge_json_source = "source";
const char* edge_json_target = "target";

void getValue(int value,edge *edgeItem){
    stringstream ss;
    ss << value;
    edgeItem->id = ss.str();
    ss.clear();
}


//json转对象
edge *json2edge(Value &edge_json,unordered_map<string ,nodeEntity*> &nodesMap){
    edge *edgeItem = new edge();
    if(edge_json[edge_json_id].IsInt()){
        getValue(edge_json[edge_json_id].GetInt(),edgeItem);
    }else{
        edgeItem->id = edge_json[edge_json_id].GetString();
    }
    if(edge_json.HasMember(edge_json_source)){
        edgeItem->mSource = nodesMap[edge_json[edge_json_source].GetString()];
    }
    if(edge_json.HasMember(edge_json_target)){
        edgeItem->mTarget = nodesMap[edge_json[edge_json_target].GetString()];
    }
    return edgeItem;
}



void json2edgelist(vector<edge *> &edgeList,Value &edgesValue,unordered_map<string,nodeEntity*> &nodesMap){
    if (edgesValue.IsArray()){
        size_t size = edgesValue.Size();
        edgeList.reserve(size);
        for (SizeType i = 0; i < size; i++) {
            Value &edge_json = edgesValue[i];
            edge *edgeItem = json2edge(edge_json,nodesMap);
            edgeList.push_back(edgeItem);
        }
    }
}


//对象转json
inline Value edge2json(edge *edgeItem,Document::AllocatorType &allocator){
    Value edgeJson(kObjectType);
    edgeJson.AddMember(StringRef(edge_json_id),StringRef(edgeItem->id.c_str()), allocator);
    edgeJson.AddMember(StringRef(edge_json_source), StringRef(edgeItem->mSource->id.c_str()), allocator);
    edgeJson.AddMember(StringRef(edge_json_target), StringRef(edgeItem->mTarget->id.c_str()), allocator);
    return edgeJson;
}

inline Value edgelist2json(vector<edge *> &edgeList,Document::AllocatorType &allocator){
    Value edgesValue(kArrayType);
    for(edge *edgeItem:edgeList){
        Value valueJson = edge2json(edgeItem,allocator);
        edgesValue.PushBack(valueJson, allocator);
    }
    return edgesValue;
}
