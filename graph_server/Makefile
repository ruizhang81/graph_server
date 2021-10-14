
PROG = myservice

#目录认证 伪造锁？
#MODULE_CFLAGS=-DMG_DISABLE_DAV_AUTH -DMG_ENABLE_FAKE_DAVLOCK

CC = g++
SOURCES = source/service/$(PROG).cpp
CFLAGS = -std=c++17
#调试用
#CFLAGS += -g
#编译优化级别
CFLAGS += -O3
#允许发出GCC能够提供的所有有用的警告
CFLAGS += -W
#include目录
#CFLAGS += -I../..
#发现不使用的函数不警告
CFLAGS += -Wno-unused-function
#视警告为错误;出现任何警告即放弃编译.
#CFLAGS += -Werror

#优化浮点运算
CFLAGS += -ffast-math

CFLAGS += -DOPEN_THREAD
#boost需要
CFLAGS += -pthread -lboost_system -lboost_thread -lboost_filesystem -lboost_iostreams
#curl需要
CFLAGS += -lcurl
#make的时候加入服务器代码

#boost的路由解析库
ifeq ($(shell uname), Linux)
    CFLAGS += -DUSE_BOOST_REGEX
    CFLAGS += -rdynamic $(shell find / -name libboost_regex.so 2>/dev/null)
endif

ifeq ($(shell uname), Darwin)
    CFLAGS += -DMAC_OS
    #链接opencv.pc
    CFLAGS += `pkg-config --cflags --libs opencv`
endif



#编译目标
all: $(PROG)

#内存检测
ifdef ASAN
    CFLAGS += -fsanitize=address
endif

#编译
$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)
