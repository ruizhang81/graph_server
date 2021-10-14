#  图谱布局服务器

### 一、使用的第三方框架  
1、服务器使用boost搭建   <https://www.boost.org/>  
2、json解析使用  <http://rapidjson.org/zh-cn/>  
3、网络请求使用libcurl  
4、线程池使用 thread + mutex  

### 二、安装  

目前支持centos和mac部署  

##### centos  
把install.sh拷贝到部署目录，然后赋权限后使用sudo执行  

##### mac  
需要源码编译vtk+opencv  (参见)
需要安装boost  
支持graph_server下使用make编译，或者用xcode打开graph_server.xcodeproj  

### 三、使用  

目前有两种方法使用：  

##### 1、网络请求  [api参见](http://172.22.1.88/zhangrui2/graph_server/blob/master/graph_server/static/api.md)
  
##### 2、命令行使用
./myservice 2d 100  随机生成100个节点和边，使用2d绘制图片  
./myservice 3d 100  随机生成100个节点和边，使用3d绘制图片，可旋转   

### 四、使用webassembly  
安装emsdk  
1、git clone https://github.com/emscripten-core/emsdk.git  
2、cd emsdk  
3、./emsdk install latest  
4、./emsdk activate latest  
5、source ./emsdk_env.sh  

二、执行webAssembly.sh  
进入localhost:8000查看  
1、左上是未布局的  
2、右上是js布局的  
3、左下是webAssembly布局的  
4、右下是c++服务器布局的  

###  五、缺陷  
没有使用智能指针  
线程池有轻微泄露  
当使用webAssembly的时候，关闭线程池，因为目前chrome需要手动去设置开启thread
