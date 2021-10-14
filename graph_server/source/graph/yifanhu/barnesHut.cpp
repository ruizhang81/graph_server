#pragma once
#include "force/electricalForce.cpp"

class barnesHut : public refCounter{

private:
    requestParam *mRequestParamObj;
    electricalForce force;
public:

    explicit barnesHut():refCounter("barnesHut") {
    }
    
    explicit barnesHut(const barnesHut&):refCounter("barnesHut") {
        cout << "barnesHut&" << endl;
    }
    
    explicit barnesHut(const barnesHut&&):refCounter("barnesHut") {
        cout << "barnesHut&&" << endl;
    }
    
    void setValue(requestParam *requestParamObj) {
        mRequestParamObj = requestParamObj;
        force.setValue(mRequestParamObj);
    }

    ~barnesHut() {
        mRequestParamObj = nullptr;
    }


    void bCalculateForce(forceVector &forceVectorPtr,nodeEntity* nodeEntity, quadTree* tree) {
        if (tree->mass <= 0) {
            return;
        }

        double distanceValue = distance(nodeEntity, tree,mRequestParamObj->yifanhu_3d);

        if (tree->isLeaf || tree->mass == 1) {
            if (distanceValue < 1e-8) {
                return;
            }
            force.calculateForce(forceVectorPtr,nodeEntity, tree);
            return;
        }

        if ((distanceValue * mRequestParamObj->barnesHutTheta) > tree->size) {
            force.calculateForce(forceVectorPtr,nodeEntity, tree, distanceValue);
            forceVectorPtr.multiply(tree->mass,mRequestParamObj->repulsionRatioWithCluster);
            return;
        }


        forceVector forceVectorAddPtr;
        for (quadTree *child : tree->children) {
            forceVectorAddPtr.reset();
            if(child != nullptr){
                bCalculateForce(forceVectorAddPtr,nodeEntity, child);
                forceVectorPtr.add(forceVectorAddPtr);
            }
        }
    }
};

