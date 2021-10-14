#pragma once
#include "graph.cpp"

class requestParam :public refCounter{
public:
    //网络请求参数
    int threadPollType = 0;//0不使用线程池,1使用线程池,2使用pthread线程池
    bool isCreateForestFun = true;//是否先创建森林
    bool isSortDegree = true;//按度排序,如果不按度排序，簇布局会有问题 中间去掉了按度排序
    bool isLayoutHierarchy = false;//先分层
//    int taskNum = 5000;//多线程计算时,多少个边或者节点放进一个线程计算
    
    bool isYifanhu = true;//yifanhu计算
    //yifanhu参数
    bool yifanhu_3d = false;//是否计算yifanhu 3d
    float stepRatio = 0.95;//步长更新率的倒数
    float relativeStrength = 0.2;//斥力相对于引力的比例系数
    float repulsionRatioWithCluster = 1;//力的放大系数
    float optimalDistance = 100;//布局中距离的优化目标
    bool layoutBalanced = true;//空间分配
    float initialStep = 0;//初始步长是optimalDistance/5
    int quadTreeMaxLevel = 8;//四叉树最大深度
    float barnesHutTheta = 1.2;//BarnesHut算法搜寻重心和计算的阈值
    float convergenceThreshold = 1e-4;//能量小于多少 通知算法收敛
    int maxIterations = 50;//最大迭代次数
    //float spaceSize = 500;//森林布局单位，layoutBalanced开启时使用   非常影响计算性能
    float gridXSize = 500;
    float gridYSize = 500;
    
    bool clusterAllLeaves = true;//是否使用多簇
    //簇布局参数
    float layoutStarThreshold = 16;//少于这么多，使用星形布局
    float layoutStarRadius = 100;//星形布局半径
    float radiusStep = 50;//簇布局偏移角度
    float nodeOccupiedSpace = 2 * M_PI * radiusStep * 2 / 15;//所有节点所占空间
    float firstLevelMinAngleSpacing = M_PI / 180 * 5;//
    bool angleWellDistributed = true;//决定放射边之间的相对位置
    bool radiusWellDistributed = true;//决定半径序列
    float angleAdjustmentRatio = 0.3;//
    float angleSpacingRatio = 0.5;//
    bool randomRadius = false;
    bool isDraw3d = false;
    float layoutBalancedSpace = 300;
    int sortMethod = 0;//0 angle-level 1 level-angle
    
    //层布局
    float unit = 500;
    
    string url;
    
    graph graphObj;
    
    void print(){
        if(innerDebugPrint){
            cout << "参数:\n"
            << "isCreateForestFun:  " << isCreateForestFun << "\n"
            << "isSortDegree:  " << isSortDegree << "\n"
            << "isLayoutHierarchy:  " << isLayoutHierarchy << "\n"
            << "isYifanhu:  " << isYifanhu << "\n"
            << "clusterAllLeaves:  " << clusterAllLeaves << "\n"
            << "yifanhu_3d:  " << yifanhu_3d << "\n"
            << "stepRatio:  " << stepRatio << "\n"
            << "relativeStrength:  " << relativeStrength << "\n"
            << "repulsionRatioWithCluster:  " << repulsionRatioWithCluster << "\n"
            << "optimalDistance:  " << optimalDistance << "\n"
            << "layoutBalanced:  " << layoutBalanced << "\n"
            << "initialStep:  " << initialStep << "\n"
            << "quadTreeMaxLevel:  " << quadTreeMaxLevel << "\n"
            << "barnesHutTheta:  " << barnesHutTheta << "\n"
            << "convergenceThreshold:  " << convergenceThreshold << "\n"
            << "maxIterations:  " << maxIterations << "\n"
            << "layoutStarThreshold:  " << layoutStarThreshold << "\n"
            << "layoutStarRadius:  " << layoutStarRadius << "\n"
            << "radiusStep:  " << radiusStep << "\n"
            << "nodeOccupiedSpace:  " << nodeOccupiedSpace << "\n"
            << "firstLevelMinAngleSpacing:  " << firstLevelMinAngleSpacing << "\n"
            << "angleWellDistributed:  " << angleWellDistributed << "\n"
            << "radiusWellDistributed:  " << radiusWellDistributed << "\n"
            << "angleAdjustmentRatio:  " << angleAdjustmentRatio << "\n"
            << "angleSpacingRatio:  " << angleSpacingRatio << "\n"
            << "sortMethod:  " << sortMethod << "\n"
            << "url:  " << url
            << endl;
        }
    }
    
    explicit requestParam():refCounter("requestParam"){
        
    }
    
    explicit requestParam(const requestParam&):refCounter("requestParam"){
        cout << "requestParam&" << endl;
    }
    
    explicit requestParam(const requestParam&&):refCounter("requestParam"){
       cout << "requestParam&&" << endl;
    }
    
    ~requestParam(){
        
    }
    
};


