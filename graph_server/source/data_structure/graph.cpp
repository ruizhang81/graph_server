#pragma once
#include "refCounter.cpp"

class graph :public refCounter{
public:
    
    vector<nodeEntity*> nodes;
    vector<edge*> edges;
    forest forestResult;
    
    explicit graph():refCounter("graph"){
        
    }
    
    explicit graph(const graph&):refCounter("graph"){
        cout << " graph&" << endl;
    }
    
    explicit graph(const graph&&):refCounter("graph"){
        cout << " graph&&" << endl;
    }
    
    ~graph(){
        clearVector(edges);
        clearVector(nodes);
    }
    
};


