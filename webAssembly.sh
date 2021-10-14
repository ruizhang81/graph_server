#!/bin/sh

quick="false"
js="false"
while getopts 'qj' OPT; do
    case $OPT in
        q) quick="true";;
        j) js="true";;
    esac
done

rm graph.wasm 2>/dev/null
rm graph.js 2>/dev/null
rm graph.js.mem 2>/dev/null
rm graph.worker.js 2>/dev/null
rm graph.wasm.mem 2>/dev/null
rm asm.js 2>/dev/null

nofindStr="command not found"
emsdkDir=/Users/zhangrui/Documents/code/emsdk
result=`emcc 2>&1`
install=$(echo $result | grep "${nofindStr}")

if [[ "$install" != "" ]];then
    emsdk activate latest
    source ${emsdkDir}/emsdk_env.sh
else
    echo "已经激活emcc"
fi
emcc --clear-cache

#编译config
openException=1;
memoryInitFile=1;
TOTAL_MEMORY=1GB;
TOTAL_MEMORY_MUTITHREAD=512MB;
#memoryInit=268435456;

print(){
params=""
for i in $@
do
params="${params} "$i
done
params="emcc ${params}"
echo "\033[32m$params\033[0m"
}

#-s ASSERTIONS=1 \
#--closure 1
#TOTAL_STACK

baseparam="graph_server/source/service/myservice.cpp \
-std=c++17 -O3 \
-ffast-math \
-o graph.js \
-s FILESYSTEM=0 \
-DWEBASSEMBLY \
-s DISABLE_EXCEPTION_CATCHING=${openException} \
-s EXTRA_EXPORTED_RUNTIME_METHODS=[\"allocateUTF8\",\"UTF8ToString\"] \
-s EXPORTED_FUNCTIONS=[\"_sort\",\"_testPrint\"] \
-s ABORTING_MALLOC=1 \
-s ASSERTIONS=1 \
-s EXIT_RUNTIME=1 \
-s MODULARIZE=1";


createASM(){
    echo "单线程js_asm"
param="${baseparam} \
-s WASM=0 \
-s TOTAL_MEMORY=${TOTAL_MEMORY} --memory-init-file ${memoryInitFile} \
-s ABORTING_MALLOC=1 \
-s EXPORT_NAME=\"ModuleASM\" \
-s ENVIRONMENT='web'";

    print $param
    emcc $param
}


createWASM(){
    echo "单线程js_wasm"
param="${baseparam} \
-s WASM=1 \
-s ALLOW_MEMORY_GROWTH=1 \
-s ENVIRONMENT='web'";

    print $param
    emcc $param
}

if [[ $js == "true" ]];then

    echo "模块打包"

    webpack=webpack
    asmPath=webpack/asm
    wasmPath=${webpack}/wasm
    rm -rf ${webpack} 2>/dev/null
    mkdir ${webpack} 2>/dev/null
    mkdir $asmPath 2>/dev/null
    mkdir $wasmPath 2>/dev/null


    createASM
    sed -i '' 's/graph.js.mem/..\/static\/webassembly\/asm\/graph.js.mem/'  'graph.js'
    mv graph.js $asmPath 2>/dev/null
    mv graph.js.mem $asmPath 2>/dev/null

    createWASM
    sed -i '' 's/graph.wasm/..\/static\/webassembly\/wasm\/graph.wasm/'  'graph.js'
    mv graph.js $wasmPath 2>/dev/null
    mv graph.wasm $wasmPath 2>/dev/null

#方便本地测试，自动把生成的文件copy到智子项目中
    sophon_project_Path="/Users/zhangrui/Documents/code/SophonWeb/src/static/webassembly"
    rm -rf ${sophon_project_Path}/wasm 2>/dev/null
    rm -rf ${sophon_project_Path}/asm 2>/dev/null
    mv ${webpack}/* ${sophon_project_Path} 2>/dev/null
    rm -rf ${webpack} 2>/dev/null

else

    echo "工程打包"

    jsPath=graph_server/static/js/
    rm ${jsPath}graph.wasm 2>/dev/null
    rm ${jsPath}graph.js 2>/dev/null
    rm ${jsPath}graph.js.mem 2>/dev/null
    rm ${jsPath}graph.worker.js 2>/dev/null
    rm ${jsPath}graph.wasm.mem 2>/dev/null


    #1打包多线程js 2打包单线程
    buildType=2
    if [[ $buildType == 1 ]];then

        echo "多线程js"
param="${baseparam} \
-s WASM=1 \
-DOPEN_THREAD \
-pthread \
-s PROXY_TO_PTHREAD=1 \
-s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=5 \
-s TOTAL_MEMORY=${TOTAL_MEMORY_MUTITHREAD} --memory-init-file ${memoryInitFile} \
-s ENVIRONMENT='web,worker'";

        print $param
        emcc $param

    else

        createWASM

    fi

    mv graph.js.mem ${jsPath} 2>/dev/null
    mv graph.worker.js ${jsPath} 2>/dev/null
    mv graph.wasm ${jsPath} 2>/dev/null
    mv graph.js ${jsPath} 2>/dev/null
    mv graph.wasm.mem ${jsPath} 2>/dev/null

    asmPath=${jsPath}asm
    rm -rf ${asmPath} 2>/dev/null
    mkdir ${asmPath} 2>/dev/null

    createASM

    mv graph.js asm.js
    mv asm.js ${asmPath} 2>/dev/null
    mv graph.js.mem ${asmPath} 2>/dev/null

    cd graph_server

    if [ $quick = "false" ]; then
        make clean
        make
    fi
    ./myservice


fi

