#pragma once
#include "../../data_structure/z_include.h"
#include "interpolatesAngle.cpp"

class position :public refCounter
{
public:
    float x;
    float y;
    float angle;
    float level;
    
    explicit position():refCounter("position"){
        x = 0;
        y = 0;
        angle = 0;
        level = 0;
    }
    
    explicit position(const position& that):refCounter("position"){
        cout << "position&" << endl;
        x = that.x;
        y = that.y;
        angle = that.angle;
        level = that.level;
    }

    //Copy assignment operator is implicitly deleted because 'position' has a user-declared move constructor
    explicit position(const position&& that):refCounter("position"){
        cout << "position&&" << endl;
        x = that.x;
        y = that.y;
        angle = that.angle;
        level = that.level;
    }
    
    ~position(){
        
    }
};

float getRadius(int radius, requestParam &requestParamObj) {
    return radius + (requestParamObj.randomRadius ? 0.5 * requestParamObj.radiusStep * random() : 0);
}

bool myfunction(position i, position j) { return (i.level < j.level); }


void clustersNodesNew(requestParam &requestParamObj,
                      vector<float> &angleInputs,
                      nodeEntity *root,
                      vector<nodeEntity *> &nodes){
    if(innerDebugPrint){
        cout << "clustersNodesNew" << endl;
    }
    int len = (int)nodes.size();
    if(len == 0){
        return;
    }
    vector<vector<float>> angles;
    
    interpolatesAngleNew(requestParamObj,len,angles,angleInputs);

//    cout << "1--------" << endl;
//    for(float angleInput : angleInputs){
//        cout << angleInput << endl;
//    }
//    cout << "2--------" << endl;
//
//    cout << "3--------" << endl;
//    for(vector<float> angle : angles){
//        for(float angleItem : angle){
//            cout << angleItem << " ";
//        }
//        cout << endl;
//    }
//    cout << "4--------" << endl;
    
    if(angles.size() == 0){
        return;
    }
    
    vector<shared_ptr<position>> positions;
    positions.reserve(len);
    int radius = requestParamObj.radiusStep;
    int k = 0;
    float r;
    float rx = root->mCenterX;
    float ry = root->mCenterY;
    
    // 2r, 3r, 5r, 7r, ...
    for (int i = 0; i < ((int)angles.size()) && k < len; i++) {
        if (i <= 1 || requestParamObj.radiusWellDistributed) {
            radius += requestParamObj.radiusStep;
        } else {
            radius += 2 * requestParamObj.radiusStep;
        }
        
        int alen = (int)angles[i].size();
//        cout << alen << endl;
        int level = i;
        for (int j = 0; j < alen && k < len; j++, k++) {
            r = getRadius(radius, requestParamObj);
            float angle = angles[i][j];
            shared_ptr<position> position_item = make_shared<position>(position());
            position_item.get()->x = rx + r * cos(angle);
            position_item.get()->y = ry + r * sin(angle);
            position_item.get()->angle = angle;
            position_item.get()->level = level;
            positions.push_back(position_item);
        }
    }
    
    if(requestParamObj.sortMethod == 0){
//        cout << "level" << endl;
        sort(positions.begin(),positions.end(),[&](shared_ptr<position> positionA,shared_ptr<position> positionB ){
            return positionB.get()->level < positionA.get()->level;
        });
        sort(positions.begin(),positions.end(),[&](shared_ptr<position> positionA,shared_ptr<position> positionB ){
            return positionB.get()->angle < positionA.get()->angle;
        });
    }else{
//        cout << "angle" << endl;
        sort(positions.begin(),positions.end(),[&](shared_ptr<position> positionA,shared_ptr<position> positionB ){
            return positionB.get()->angle < positionA.get()->angle;
        });
        sort(positions.begin(),positions.end(),[&](shared_ptr<position> positionA,shared_ptr<position> positionB ){
            return positionB.get()->level < positionA.get()->level;
        });
    }
    
    
    
    
    for (int i = 0; i<len; i++) {
        nodeEntity *node = nodes[i];
        position* position_item = positions[i].get();
        node->mCenterX =  position_item->x;
        node->mCenterY =  position_item->y;
    }
    
    for(vector<float> &angle:angles){
        clearVectorR(angle);
    }
    clearVectorR(angles);
    positions.clear();
}
