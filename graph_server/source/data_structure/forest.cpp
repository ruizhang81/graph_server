#pragma once
#include "tree.cpp"
#include "refCounter.cpp"

class forest : public refCounter{
public:
    vector<tree*> trees;
    
    explicit forest():refCounter("forest"){
        
    }
    
    explicit forest(const forest&):refCounter("forest"){
        cout << "forest&" << endl;
    }
    
    explicit forest(const forest&&):refCounter("forest"){
        cout << "forest&&" << endl;
    }
    
    
    ~forest(){
        clearVector(trees);
    }
};

