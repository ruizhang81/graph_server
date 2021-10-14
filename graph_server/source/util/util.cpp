using namespace std;
using namespace chrono;

//取小值
int min(int a,int b){
    if(a < b){
        return a;
    }else{
        return b;
    }
}

//取当前时间并格式化
string getTime()
{
    time_t timep;
    time (&timep);
    size_t size = 18;
    char timeStr[size];
    strftime(timeStr, size, "%Y-%m-%d_%H%M%S",localtime(&timep));
    return timeStr;
}

//取程序目录
string GetProgramDir()
{
    char buff[1000];
    getcwd(buff, 1000);
    return buff;
}

//取当前时间戳
auto getTimeStamp(bool get){
    if(get){
        return system_clock::now();
    }
    return chrono::time_point<system_clock>();
}

//打印消耗时间
auto GetCostTime(string message,system_clock::time_point start)
{
    auto end  = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << message << " 花费了"
    << double(duration.count()) * microseconds::period::num / microseconds::period::den
    << "秒" << endl;
}
