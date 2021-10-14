#!/bin/sh

install(){
    package=$1
    result=`rpm -qa|grep $package`
    if [ $# == 2 ] ; then
        result=`$2`
    fi
    install=$(echo $result | grep "${result}")
    if [[ "$install" != "" ]];
    then
        echo $package" 已经安装"
    else
        while true;do
            for i in '-' "\\" '|' '/';do
                printf $package"正在安装... %s\r" $i
                sleep 0.2
            done
            trap 'exit 0' HUP
        done &
        BG_PID=$!
        sudo yum -y install $package >> /dev/null
        (kill -HUP ${BG_PID})
        echo $package" 已经安装      "
    fi
}

#检查安装git
install git "git --version"

install supervisor
install boost
install boost-devel
install boost-doc

#curl
install libcurl-dev
install libcurl-devel

install vim

#配置epel源
install epel-release
install gcc
install gcc-c++
install scl-utils
install centos-release-scl
sudo yum-config-manager --enable rhel-server-rhscl-7-rpms

result=`rpm -qa|grep devtoolset-7`
result=$(echo $result | grep "${result}")
if [[ "$result" != "" ]];
then
    echo "已经加入环境变量！"
else
    install devtoolset-7
    # scl会重新创建一个shell 所以开个子线程
    sudo scl enable devtoolset-7 bash &
    sleep 0.5
    sudo echo "source scl_source enable devtoolset-7" >> ~/.bash_profile
    source ~/.bash_profile
fi


#下载代码
path=`pwd`
if [ ! -d ${path}"/graph_server" ];then
    #无这个文件夹
    echo "获取图谱服务器源码"
    git clone http://172.22.1.88/zhangrui2/graph_server.git
    cd graph_server
else
    #有这个文件夹
    echo "更新"
    cd graph_server && git pull
fi
cd graph_server
make clean
make
cd ..

#sudo supervisorctl shutdown  >> /dev/null
sudo kill -9 `lsof -t -i:8000` >> /dev/null
sudo kill -9 `ps -ef | grep myservice | grep -v grep | awk '{print $2}'` >> /dev/null


#配置服务自动重启
myserviceIni=/etc/supervisord.d/myservice.ini
sudo sh -c "echo -e '[program:myservice]
command=sh run
directory=$path/graph_server
environment=
user=root
stopsignal=INT
autostart=true
autorestart=true
startsecs=3
stderr_logfile=/var/log/ossoffical.err.log
stdout_logfile=/var/log/ossoffical.out.log' > $myserviceIni"
sudo supervisord -c /etc/supervisord.conf
sudo supervisorctl -c /etc/supervisord.conf reload

#开机启动
supervisord_service=/lib/systemd/system/supervisord.service
sudo sh -c "echo -e '[Unit]
Description=Process Monitoring and Control Daemon
After=rc-local.service nss-user-lookup.target

[Service]
Type=forking
ExecStart=/usr/bin/supervisord -c /etc/supervisord.conf
ExecStop=/usr/bin/supervisord shutdown
ExecReload=/usr/bin/supervisord reload
killMode=process
Restart=on-failure
RestartSec=42s

[Install]
WantedBy=multi-user.target' > $supervisord_service"
sudo systemctl enable supervisord

cd $path/graph_server
./run &
echo "图谱服务器已经启动！已加入开机启动！"

sudo supervisorctl restart myservice
