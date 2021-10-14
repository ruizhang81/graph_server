#pragma once
#include "../config_include_base.h"


template<typename T>
void clearVector(vector<T *> &list) {
    for (T *item:list) {
        if (nullptr != item) {
            delete item;
            item = nullptr;
        }
    }
    list.clear();
}

template<typename T>
void clearVectorR(vector<T> &list) {
    if(list.size()>0){
        list.erase(list.begin(),list.end());
        list.clear();
    }
}



template<class K,class V>
void clearMap(unordered_map<K,V> &map) {
    map.erase(map.begin(),map.end());
    map.clear();
}











