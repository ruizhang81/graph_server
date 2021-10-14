#pragma once
#include "stepDisplacement.cpp"
#include "quadTree_2d.cpp"
#include "quadTree_3d.cpp"
#include "barnesHut.cpp"


 quadTree *buildTree(vector<nodeEntity *> *mnodeEntitys, int maxLevel,bool yifanhu_3d) {
    if(yifanhu_3d){
        float minX = __FLT_MAX__;
        float maxX = __FLT_MIN__;
        float minY = __FLT_MAX__;
        float maxY = __FLT_MIN__;
        float minZ = __FLT_MAX__;
        float maxZ = __FLT_MIN__;
        for(nodeEntity *node : *mnodeEntitys){
            if(node->mCenterX < minX){
                minX = node->mCenterX;
            }
            if(node->mCenterX > maxX){
                maxX = node->mCenterX;
            }
            if(node->mCenterY < minY){
                minY = node->mCenterY;
            }
            if(node->mCenterY > maxY){
                maxY = node->mCenterY;
            }
            if(node->mCenterZ < minZ){
                minZ = node->mCenterZ;
            }
            if(node->mCenterZ > maxZ){
                maxZ = node->mCenterZ;
            }
        }
        
        float disY = maxY - minY;
        float disX = maxX - minX;
        float disZ = maxZ - minZ;
        float size = disY > disX ? disY : disX;
        size = size > disZ ? size : disZ;
        
        quadTree *tree = new quadTree_3d(minX, minY, minZ, size, maxLevel);
        for (nodeEntity *nodeEntityItem : *mnodeEntitys) {
            tree->addnodeEntity(nodeEntityItem);
        }
        return tree;
    }else{
        float minX = __FLT_MAX__;
        float maxX = __FLT_MIN__;
        float minY = __FLT_MAX__;
        float maxY = __FLT_MIN__;
        for(nodeEntity *node: *mnodeEntitys){
            if(node->mCenterX < minX){
                minX = node->mCenterX;
            }
            if(node->mCenterX > maxX){
                maxX = node->mCenterX;
            }
            if(node->mCenterY < minY){
                minY = node->mCenterY;
            }
            if(node->mCenterY > maxY){
                maxY = node->mCenterY;
            }
        }
        float disY = maxY - minY;
        float disX = maxX - minX;
        float size = disY > disX ? disY : disX;
        
        quadTree *tree = new quadTree_2d(minX, minY, size, maxLevel);
        for (nodeEntity *nodeEntityItem : *mnodeEntitys) {
            tree->addnodeEntity(nodeEntityItem);
        }
        return tree;
    }
}


class yifanHuAlgorithm : public refCounter{
public:
    
    float step;// (力矢量)坐标更新的步长
    int progress;// 可以理解为正反馈次数，如果当前更新的方向是正确的，且正反馈的次数大于5，则加大更新步长 step = step / StepRatio（小于1）
    float convergenceThreshold;//收敛判断
    bool adaptiveCooling;//自适应退火算法的开启，与5想对应
    double energyPre;//初始布局的能量
    double energy;//当前布局的能量
    double energyMin;//达到的最小能量
    bool converged;//是否已经达到均衡
    vector<nodeEntity *> *mNodes;
    vector<edge *> *mEdges;
    requestParam *mRequestParamObj;
    
    //力计算
    springForce force;
    barnesHut barnes;
    
    long nodeSize;
    long edgeSize;
    bool isPrint;
    
    explicit yifanHuAlgorithm(const yifanHuAlgorithm&):refCounter("yifanHuAlgorithm"){
        cout << "yifanHuAlgorithm&" << endl;
    }
    
    explicit yifanHuAlgorithm(const yifanHuAlgorithm&&):refCounter("yifanHuAlgorithm"){
        cout << "yifanHuAlgorithm&&" << endl;
    }
    
    explicit yifanHuAlgorithm():refCounter("yifanHuAlgorithm"){}
    
    explicit yifanHuAlgorithm(vector<nodeEntity *> *nodes,
                     vector<edge *> *edges,
                     requestParam *requestParamObj):refCounter("yifanHuAlgorithm") {
        init(nodes,edges,requestParamObj);
    }
    
    ~yifanHuAlgorithm()
    {
        reset();
    }
    
    void init(vector<nodeEntity *> *nodes,
                     vector<edge *> *edges,
                     requestParam *requestParamObj){
        mRequestParamObj = requestParamObj;
        mNodes = nodes;
        mEdges = edges;
        if(mRequestParamObj->optimalDistance == 0){
            float edgeLength = 0;//边的总长度
            for (edge *edge_obj : *mEdges) {
                edgeLength += distance(edge_obj->mSource, edge_obj->mTarget);
            }
            mRequestParamObj->optimalDistance = edgeLength / (mEdges->size() + 1);//边的平均长度
        }
        
        step = (mRequestParamObj->initialStep != 0) ? mRequestParamObj->initialStep != 0: mRequestParamObj->optimalDistance / 5;
        adaptiveCooling = true;
        convergenceThreshold = mRequestParamObj->convergenceThreshold;
        energy = DBL_MAX;//能量初始设置为无限大
        energyMin = DBL_MAX;
        progress = 0;
        converged = false;
        isPrint = mNodes->size() > printNum;
        force.setValue(mRequestParamObj);
        barnes.setValue(mRequestParamObj);
        
        if(mRequestParamObj->threadPollType > 0) {
            nodeSize = mNodes->size();
            edgeSize = mEdges->size();
        }
    }
    
    void reset(){
        mRequestParamObj = nullptr;
        mNodes = nullptr;
        mEdges = nullptr;
    }
    
    //单线程使用
    void repulsionFun(quadTree *treePtr,
                      long p_startIndex,
                      long p_endIndex){
        forceVector forceVectorPtr;
        for(long i = p_startIndex;i < p_endIndex;i++){
            forceVectorPtr.reset();
            nodeEntity *nodeEntity = (*mNodes)[i];
            barnes.bCalculateForce(forceVectorPtr,nodeEntity, treePtr);
            nodeEntity->forceVectorItem.add(forceVectorPtr);
        }
    }
    
    void gravityFun(long p_startIndex,
                    long p_endIndex){
        forceVector forceVectorPtr;
        for(long i = p_startIndex;i < p_endIndex;i++){
            forceVectorPtr.reset();
            edge *e = (*mEdges)[i];
            nodeEntity *n1 = e->mSource;
            nodeEntity *n2 = e->mTarget;
            if (n1 == n2) {
            } else {
                force.calculateForce(forceVectorPtr,n1, n2);
                n1->forceVectorItem.add(forceVectorPtr);
                n2->forceVectorItem.subtract(forceVectorPtr);
            }
        }
    }
    

    void
    goAlgo(int index) {
        auto start  = getTimeStamp(innerDebugPrint);
        
        // 生成四叉树
        quadTree *treePtr = buildTree(mNodes, mRequestParamObj->quadTreeMaxLevel,mRequestParamObj->yifanhu_3d);
        
        const long p_node_startIndex = 0;
        const long p_node_endIndex = (long)mNodes->size();
        repulsionFun(treePtr,
                     p_node_startIndex,
                     p_node_endIndex);
        
        const long p_edge_startIndex = 0;
        const long p_edge_endIndex = (long)mEdges->size();
        gravityFun(p_edge_startIndex,
                   p_edge_endIndex);
        
        // 计算总和的能量，求出最大力
        energyPre = energy;
        energy = 0;
        double maxForce = 1;
        for (nodeEntity *nodeEntityItem : *mNodes) {
            float norm = nodeEntityItem->forceVectorItem.getNorm();
            energy += norm;
            maxForce = maxForce > norm ? maxForce : norm;
        }
        
        if(std::isnan(energy)){
            converged = true;
            print2(index,start,0,"1");
        }else{
            
            /**
             * 规格化移动距离
             * **/
            float multiplier = 1.0 / maxForce;
            
            // 根据向量移动每个节点
            for (nodeEntity *nodeEntityItem : *mNodes) {
                nodeEntityItem->forceVectorItem.multiply(multiplier,mRequestParamObj->repulsionRatioWithCluster);//更新力向量步长
                movenodeEntity(nodeEntityItem,step,mRequestParamObj);//更新移动步长
            }
            
            
            //退火算法，如果达到预期，就改变步长
            if (adaptiveCooling) {
                if (energy < energyPre) {
                    progress++;
                    if (progress >= 5) {
                        progress = 0;
                        step /= mRequestParamObj->stepRatio;
                    }
                } else {
                    progress = 0;
                    step *= mRequestParamObj->stepRatio;
                }
            } else {
                step *= mRequestParamObj->stepRatio;
            }
            
            //如果能量小于预期，通知算法收敛
            double nowConvergenceThreshold = abs((energy - energyPre) / energy);
            if(energyMin > nowConvergenceThreshold){
                energyMin = nowConvergenceThreshold;
            }
            if(std::isnan(nowConvergenceThreshold)){
                converged = true;
                print2(index,start,nowConvergenceThreshold,"2");
            }else{
                if (nowConvergenceThreshold < convergenceThreshold) {
                    converged = true;
                    print2(index,start,nowConvergenceThreshold,"3");
                }else{
                    print2(index,start,nowConvergenceThreshold,"4");
                }
            }
            
            
            //把力向量初始化
            for (nodeEntity *nodeEntityItem : *mNodes) {
                nodeEntityItem->forceVectorItem.reset();
            }
        }
        
        if(yifanhuAlawyRun){
            converged = false;
        }
        delete treePtr;
        treePtr = nullptr;
    }
    
    
    void print(int index,double start){
        printf("cost %d cost:%f \n",index,clock()-start);
    }
    
    void print2(int index,system_clock::time_point start,float nowConvergenceThreshold,const char* flag){
        if(isPrint && innerDebugPrint){
            cout << "第" << index << "次迭代 " <<" 图谱能量:" << nowConvergenceThreshold << " 达到的最小能量:"<< energyMin << " step:" << step << " flag:"<< flag <<endl;
            
            GetCostTime("费时",start);
        }
    }
};

