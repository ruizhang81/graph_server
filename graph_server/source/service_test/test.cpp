#include "../../lib/requests/liburlHelp.cpp"
#include "../json/jsonHelpRequestParam.cpp"
#include "../util/createTestdata.cpp"

//const string url = "http://172.22.5.13:8000/api/sort2";
const string url = "http://localhost:8000/api/sort2";
const int times = 100;
const unsigned long num_size = 1000;
int finishNum = 0;

void createData(StringBuffer &buffer,const unsigned long  num){
    requestParam requestParamObj;
    requestParamObj.threadPollType = 1;
    fullNodesAndEdges(requestParamObj,num,num);
    obj2String(buffer,requestParamObj);
}



void runRequest(const string *testData){
    string resultResponse;
    
//    cout << testData->c_str() << endl;
    bool result =  postData(url,testData->c_str(),resultResponse);

    if(result){
        Document document;
        const char *responseChar = resultResponse.c_str();
        size_t size = std::strlen(responseChar);
        document.Parse(responseChar,size);
//        cout << responseChar << endl;
        
        if(document.IsObject()){
            const char* codeKey_char = codeKey.c_str();
            if(document.HasMember(codeKey_char)){
                int code = document[codeKey_char].GetInt();
                if(code == 0){
                    finishNum++;
                    printf("\r[%d%%] ", finishNum*100/times);
                    fflush(stdout);
                }else{
                    string message = document[msgKey.c_str()].GetString();
                    cout << "失败2:" << message << endl;
                }
                
            }else{
                cout << "失败2:" << endl;
            }
        }else{
            cout << "失败3:" << endl;
        }
    }
    
    

}


/// 新线程入口
void *demo(void *param)
{
    const string *testData = (string*)param;
    runRequest(testData);
    return NULL;
}

void run(){
    cout << "start test"<< endl;
    finishNum = 0;
    StringBuffer buffer;
    createData(buffer,num_size);
    const string testData = buffer.GetString();
    cout << "create test data finish"<< endl;
    
    auto start  = system_clock::now();
    
    pthread_t my_thread[num_size];
    
    for(int i = 1; i <= times; i++) {
        int ret =  pthread_create(&my_thread[i], NULL, demo,(void*)&testData);
        if(ret != 0) {
            cout << "error" << endl;
            exit(EXIT_FAILURE);
        }
    }
    
    for(int i = 1; i <= times; i++) {
        pthread_join(my_thread[i],NULL);
    }
    
    if(finishNum == times){
        cout << "test all success num_size:"<< num_size << " times:"<< times << endl;
        GetCostTime("总共费时",start);
    }else{
        cout << "test finish has error:" << (times - finishNum) << endl;
    }
}

int main(int argc, char* argv[]) {
    UNUSED(argc);
    UNUSED(argv);
    
//    while(true){
//        run();
//    }
    run();
    return 0;
}
