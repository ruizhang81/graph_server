#pragma once
#include "nodeEntity.cpp"
#include "refCounter.cpp"

class edge :public refCounter{
public:
    string id;
    string tmp_id;
    nodeEntity *mSource;
    nodeEntity *mTarget;

    explicit edge():refCounter("edge"){

    }
    
    explicit edge(const edge& that):refCounter("edge"){
        cout << "edge&" << endl;
        id = that.id;
        tmp_id = that.tmp_id;
        mSource = that.mSource;
        mTarget = that.mTarget;
    }
    
    explicit edge(const edge&& that):refCounter("edge"){
        cout << "edge&&" << endl;
        id = that.id;
        tmp_id = that.tmp_id;
        mSource = that.mSource;
        mTarget = that.mTarget;
    }
    
    
    ~edge(){
        if(nullptr != mSource){
            mSource = nullptr;
        }
        if(nullptr != mTarget){
            mTarget = nullptr;
        }
    }
};
