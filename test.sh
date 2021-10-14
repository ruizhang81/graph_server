#!/bin/sh

pwd=`pwd`
projectPath=${pwd}/graph_server
rm -rf $projectPath/test

g++ ${projectPath}/source/service_test/test.cpp \
-std=c++1z -W \
-lcurl -lpthread  -pthread \
-o ${projectPath}/test


$projectPath/test
