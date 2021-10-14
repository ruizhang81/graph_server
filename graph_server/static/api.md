图谱布局计算接口

联系人：张瑞  

###  WebAssembly js调用接口

引入graph.js  
```

Module.onRuntimeInitialized = function() {  

    var inputStr = JSON.stringify(jsObj);//js对象转字符串
    var inputPtr = allocateUTF8(inputStr);//字符串转指针
    var outputPtr = Module._sort(inputPtr);//sort排序
    var outputStr = UTF8ToString(outputPtr);//排序结果指针转字符串
    _free(inputStr);//释放申请的指针
    jsObj = JSON.parse(outputStr);

}  

```

1、提交的json如下，`url字段必须填写`   [请求字段详细](#request_params)  
```json
{
    "url":"http://api.sophon.com/graph/show/10165",
    "isCreateForestFun":true,    
    "isSortDegree":true,         
    "isLayoutHierarchy":true,    
    "isYifanhu":true,            
    "taskNum":50000,             
    "optimalDistance":100,       
    "yifanhu_3d":true,           
    "maxIterations":100
}
```

2、[返回的json格式字段详细](#reponse_params)   

  
###  提供一个请求url，返回布局完成的图谱  

接口地址 : /api/sort  
请求方式 ：post    

1、提交的json如下，`url字段必须填写`   [请求字段详细](#request_params)  
```json
{
    "url":"http://api.sophon.com/graph/show/10165",
    "isCreateForestFun":true,    
    "isSortDegree":true,         
    "isLayoutHierarchy":true,    
    "isYifanhu":true,            
    "taskNum":50000,             
    "optimalDistance":100,       
    "yifanhu_3d":true,           
    "maxIterations":100
}
```

2、[返回的json格式字段详细](#reponse_params)   

  
###  提供一个图谱，返回计算好的图谱  

接口地址 : /api/sort2
请求方式 ：post    

1、提交的json如下：  [请求字段详细](#request_params)     

```json
{
    "isCreateForestFun":true,    
    "isSortDegree":true,         
    "isLayoutHierarchy":true,    
    "isYifanhu":true,            
    "taskNum":50000,             
    "optimalDistance":100,       
    "yifanhu_3d":true,           
    "maxIterations":100,         
    "nodes": [
        {
            "id":"0",                
            "x":55.55,               
            "y":66.66                    
        },
        {
            "id":"1",                
            "x":55.55,               
            "y":66.66                     
        }
    ],
    "edges":[
        {
            "id":"0",               
            "source":"0",
            "target":"1"
        },
        {
            "id":"1",                
            "source":"1",
            "target":"0"
        }
    ]
}

```  
2、[返回的json格式字段详细](#reponse_params)   




 <div id="request_params"></div>   
   
###  请求参数详细 
  
```
url;//请求服务器地址
isCreateForestFun = true;//是否先创建森林
isSortDegree = false;//按度排序
isLayoutHierarchy = false;//先分层

isYifanhu = true;//是否使用yifanhu计算
//yifanhu参数
yifanhu_3d = false;//是否计算yifanhu 3d
stepRatio = 0.95;//步长更新率的倒数
relativeStrength = 0.2;//斥力相对于引力的比例系数
repulsionRatioWithCluster = 1;//力的放大系数
optimalDistance = 100;//布局中距离的优化目标
layoutBalanced = true;//未使用
initialStep = 0;//初始步长是optimalDistance/5
quadTreeMaxLevel = 8;//四叉树最大深度
barnesHutTheta = 1.2;//BarnesHut算法搜寻重心和计算的阈值
convergenceThreshold = 1e-4;//能量小于多少 通知算法收敛
maxIterations = 50;//最大迭代次数
spaceSize = 50;//最大迭代次数
taskNum = 5000;//多线程计算时,多少个边或者节点放进一个线程计算


clusterAllLeaves = true;//是否使用多簇
//簇布局参数
layoutStarThreshold = 0;//少于这么多，使用星形布局
layoutStarRadius = 100;//星形布局半径
radiusStep = 50;//簇布局偏移角度
nodeOccupiedSpace = 2 * Math.PI * radiusStep * 2 / 15;//所有节点所占空间
firstLevelMinAngleSpacing = Math.PI / 180 * 10;//
angleWellDistributed = true;//决定放射边之间的相对位置
radiusWellDistributed = true;//决定半径序列
angleAdjustmentRatio = 0.4;//
angleSpacingRatio = 0.5;//
```

<div id="reponse_params"></div>  
  
###   返回参数详细  
  
```json
{
    "nodes": [
        {
            "id":"0",                
            "x":55.55,               
            "y":66.66                   
        },
        {
            "id":"1",                
            "x":55.55,               
            "y":66.66                    
        }
    ],
    "edges":[
        {
            "id":"0",               
            "source":"0",
            "target":"1"
        },
        {
            "id":"1",                
            "source":"1",
            "target":"0"
        }
    ]
}

```

