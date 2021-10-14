#pragma once
#include "jsonHelpEdge.cpp"


//对象塞入原来的json(只需要塞入node的坐标即可)
void modifyJson(graph &graphObj,Document &document){
    Document::AllocatorType &allocator = document.GetAllocator();
    Value &nodesJson = document["nodes"];

    for (SizeType i = 0; i < nodesJson.Size(); i++) {
        Value &node_json = nodesJson[i];
        nodeEntity *node = graphObj.nodes[i];
        
//        node_json.RemoveMember(node_json_x);
//        node_json.AddMember(StringRef(node_json_x),node->mCenterX,allocator);
  
//        node_json.RemoveMember(node_json_y);
//        node_json.AddMember(StringRef(node_json_y),node->mCenterY,allocator);
        
        node_json.AddMember(StringRef(node_json_x_yfh),node->mCenterX,allocator);
        node_json.AddMember(StringRef(node_json_y_yfh),node->mCenterY,allocator);
    }
    
    document.AddMember(StringRef(codeKey.c_str()), responseSuccess, allocator);
}


//json转对象
inline string convert2Obj(const char *content,Document &document,graph &graphData){
    size_t size = strlen(content);
    document.Parse(content,size);
  
    if (!document.IsObject()){
        return "json解析失败";
    }
    if(!document.HasMember("nodes")){
        return "必须包含nodes字段";
    }
    if(!document.HasMember("edges")){
        return "必须包含edges字段";
    }
    
    Value &nodesJson = document["nodes"];
    json2nodelist(graphData.nodes, nodesJson,false);
    unordered_map<string,nodeEntity*> nodesMap;
    long nodeSize = graphData.nodes.size();
    nodesMap.reserve(nodeSize);
    nodesMap.rehash(nodeSize);
    for (nodeEntity *yifanhuNodePtr: graphData.nodes) {
        string key(yifanhuNodePtr->id);
        nodesMap[key] = yifanhuNodePtr;
    }
    
    
    Value &edgesJson = document["edges"];
    json2edgelist(graphData.edges,edgesJson,nodesMap);
    clearMap(nodesMap);
    return "";
}


//json转string
void convert2String(StringBuffer &buffer,Document &document){
//    PrettyWriter<StringBuffer> writer(buffer);
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);
    document.GetAllocator().Clear();
}


//test:对象转json
void obj2String(StringBuffer &buffer,
                   requestParam &requestParamObj){
    Document document;
    Document::AllocatorType &allocator = document.GetAllocator();
    
    Value nodes_value = nodelist2json(requestParamObj.graphObj.nodes,allocator,false);
    Value edges_value = edgelist2json(requestParamObj.graphObj.edges,allocator);
    
    document.SetObject();
    document.AddMember(StringRef("nodes"), nodes_value,allocator);
    document.AddMember(StringRef("edges"), edges_value,allocator);
    document.AddMember(StringRef("threadPollType"), requestParamObj.threadPollType,allocator);
    
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);
    allocator.Clear();
}
