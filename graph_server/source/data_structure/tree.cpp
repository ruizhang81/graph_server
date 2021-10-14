#pragma once
#include "refCounter.cpp"
#include "nodeEntity.cpp"
#include "edge.cpp"
#include "rectClass.cpp"

class tree : public refCounter{
public:
    vector< nodeEntity * > nodes;
    vector< edge * > edges;
    rectClass rect;
    
    explicit tree():refCounter("tree"){

    }
    
    explicit tree(const tree&):refCounter("tree"){
        cout << "tree&" << endl;
    }
    
    explicit tree(const tree&&):refCounter("tree"){
        cout << "tree&&" << endl;
    }
    
    ~tree(){
        clearVectorR(nodes);
        clearVectorR(edges);
    }
};
