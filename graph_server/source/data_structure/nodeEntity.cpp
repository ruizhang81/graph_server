#pragma once
#include "forceVector.cpp"
#include "refCounter.cpp"

class nodeEntity:public refCounter
{
public:
    string id;
    string tmp_id;
    float mCenterX;
    float mCenterY;
    float mCenterZ;
    float mCenterX_draw;
    float mCenterY_draw;
    float mCenterZ_draw;
//    float size;
    forceVector forceVectorItem;
    
    //广度遍历需要
    int wt_level;//该点的深度
    int wt_maxlevel;//最大深度
    vector<nodeEntity*> wt_children;//叶子节点
    int wt_leaves;//叶子节点数
    int degree;//节点的度
    int wt_height;//子树高度
    
    //节点多簇布局需要
    unordered_map<nodeEntity *, int> neighbors;//邻节点
    vector<nodeEntity *> nonLeaves;
    vector<nodeEntity *> leaves;
    
    explicit nodeEntity():refCounter("nodeEntity"){
        mCenterZ = 0;
        mCenterZ_draw = 0;
        degree = 0;
    }

    explicit nodeEntity(const nodeEntity& that):refCounter("nodeEntity"){
        cout << "nodeEntity&" << endl;
        mCenterX = that.mCenterX;
        mCenterY = that.mCenterY;
        mCenterZ = that.mCenterZ;
        mCenterX_draw = that.mCenterX_draw;
        mCenterY_draw = that.mCenterY_draw;
        mCenterZ_draw = that.mCenterZ_draw;
        degree = that.degree;
    }
    
    explicit nodeEntity(const nodeEntity&& that):refCounter("nodeEntity"){
        cout << "nodeEntity&&" << endl;
        mCenterX = that.mCenterX;
        mCenterY = that.mCenterY;
        mCenterZ = that.mCenterZ;
        mCenterX_draw = that.mCenterX_draw;
        mCenterY_draw = that.mCenterY_draw;
        mCenterZ_draw = that.mCenterZ_draw;
        degree = that.degree;
    }
    
    ~nodeEntity(){
        wt_children.clear();
        clearMap(neighbors);
        nonLeaves.clear();
        leaves.clear();
    }

    void deleteExtra(){
        wt_children.clear();
        clearMap(neighbors);
        nonLeaves.clear();
        leaves.clear();
    }
};

float distance(const nodeEntity *n1,const nodeEntity *n2,const bool yifanhu_3d) {
    if(yifanhu_3d){
        return std::sqrt(std::pow(n1->mCenterX - n2->mCenterX,2)
                         + std::pow(n1->mCenterY - n2->mCenterY,2)
                         + std::pow(n1->mCenterZ - n2->mCenterZ,2));
    }else{
        return hypot(n1->mCenterX - n2->mCenterX, n1->mCenterY - n2->mCenterY);
    }
}


