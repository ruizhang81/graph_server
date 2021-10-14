#include "../graph/zrun.cpp"
#include "../json/jsonHelpRequestParam.cpp"
#include "../util/createTestdata.cpp"



//error输出
void error(StringBuffer &buffer,
           string msg){
    Document document;
    Document::AllocatorType &allocator = document.GetAllocator();
    allocator.Clear();
    document.SetObject();
    document.AddMember(StringRef(codeKey.c_str()), responseErr, allocator);
    document.AddMember(StringRef(msgKey.c_str()), StringRef(msg.c_str()), allocator);
//    PrettyWriter<StringBuffer> writer(buffer);
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);
    allocator.Clear();
}


inline string sortGraph(const char* graphString,
                 StringBuffer &buffer,
                 requestParam &requestParamObj){
    Document document;
    //json转图谱数据
    string result = convert2Obj(graphString,document,requestParamObj.graphObj);
    if(!result.empty()){
        return result;
    }
    mySort(requestParamObj);
    modifyJson(requestParamObj.graphObj,document);
    convert2String(buffer,document);
    return "";
}

string runCmd(const char* str){
    StringBuffer buffer;
    requestParam requestParamObj;
    string result = sortGraph(str,buffer,requestParamObj);
    if(!result.empty()){
        error(buffer,result);
    }
    return buffer.GetString();
}


#ifndef WEBASSEMBLY

#include "../../lib/boostServer/server_http.hpp"
#include "../../lib/requests/liburlHelp.cpp"


using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;


void getHeader(SimpleWeb::CaseInsensitiveMultimap &header){
    header.emplace("Content-Type", "application/json");
    header.emplace("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, DELETE");
    header.emplace("Access-Control-Max-Age", "2678400");
    header.emplace("Access-Control-Allow-Headers", "authorization,content-encoding,content-type,sophon-request-id,sophon-auth-token");
    header.emplace("Access-Control-Allow-Origin", "*");
}


void handle_send_binary(string path,
                        shared_ptr<HttpServer::Response> &response,
                        string type){
    SimpleWeb::CaseInsensitiveMultimap header;
    auto ifs = make_shared<ifstream>();
    ifs->open(path, ifstream::in | ios::binary | ios::ate);
    
    if(*ifs) {
        auto length = ifs->tellg();
        ifs->seekg(0, ios::beg);
        
        header.emplace("Content-Length", to_string(length));
        header.emplace("Content-type", type);
        response->write(header);
        
        // Trick to define a recursive function within this scope (for example purposes)
        class FileServer {
        public:
            static void read_and_send(const shared_ptr<HttpServer::Response> &response, const shared_ptr<ifstream> &ifs) {
                // Read and send 128 KB at a time
                static vector<char> buffer(131072); // Safe when server is running on one thread
                streamsize read_length;
                if((read_length = ifs->read(&buffer[0], static_cast<streamsize>(buffer.size())).gcount()) > 0) {
                    response->write(&buffer[0], read_length);
                    if(read_length == static_cast<streamsize>(buffer.size())) {
                        response->send([response, ifs](const SimpleWeb::error_code &ec) {
                            if(!ec)
                                read_and_send(response, ifs);
                            else
                                cerr << "Connection interrupted" << endl;
                        });
                    }
                }
            }
        };
        FileServer::read_and_send(response, ifs);
    }
}

//获取文件内容
string getFileContent(string path){
    string dir = GetProgramDir();
    path = dir + "/static" + path;
    ifstream t(path);
    stringstream buffer;
    buffer << t.rdbuf();
    string contents(buffer.str());
    buffer.clear();
    t.close( );
    return contents;
}

//请求图谱数据
string requestGraph(string &content,
                    shared_ptr<HttpServer::Request> &request,
                    requestParam &requestParamObj,
                    string &graphString){
    vector<string> list;
    
    //post参数 字符串转对象
    string result = string2RequestParam(requestParamObj,content.c_str(),true);
    if(!result.empty()){
        return result;
    }
    //取header
    for(auto &field : request->header){
        string value = field.first + ":" + field.second;
        list.push_back(move(value));
    }
    //请求图谱数据
    bool getSuccess = getData(requestParamObj.url,list,graphString);
    if(!getSuccess){
        return result + graphString;
    }
    return "";
}


void sortGraph(shared_ptr<HttpServer::Response> &response,
          shared_ptr<HttpServer::Request> &request,
          string &content){
    if(debugPrint){
        cout << "POST /api/sort" << endl;
    }
    try {
        //            debugPrint = true;
        auto start = getTimeStamp(debugPrint);
        StringBuffer buffer;//返回字符串
        requestParam requestParamObj;//参数
        string graphString;
        
        string result = requestGraph(content,request,requestParamObj,graphString);
        if(debugPrint){
            GetCostTime("----get server data finish---- 消耗",start);
        }
        if(result.empty()){
            result = sortGraph(graphString.c_str(),buffer,requestParamObj);
            if(!result.empty()){
                error(buffer,result);
                if(debugPrint){
                    cout << "----sort--"<< result << "----" << endl;
                }
            }else{
                if(httpTestSaveImg){
                    drawPic(requestParamObj);
                }
                if(debugPrint){
                    cout << "----sort--success----" << endl;
                }
            }
        }else{
            error(buffer,result);
            if(debugPrint){
                cout << "----sort--"<< result << "----" << endl;
            }
        }
        
        SimpleWeb::CaseInsensitiveMultimap header;
        getHeader(header);
        response->write(SimpleWeb::StatusCode::success_ok, buffer.GetString(), header);
        if(debugPrint){
            GetCostTime("----response---- 总共消耗",start);
        }
    }catch(const exception &e) {
        *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n"
        << e.what();
        if(debugPrint){
            cout << e.what() << endl;
        }
    }
}

void sort2(shared_ptr<HttpServer::Response> &response,
           const char *content){
    if(debugPrint){
        cout << "POST /api/sort2" << endl;
    }
    try {
        StringBuffer buffer;//返回字符串
        requestParam requestParamObj;//参数
        string result = string2RequestParam(requestParamObj,content,false);
        
        if(result.empty()){
            result = sortGraph(content,buffer,requestParamObj);
            if(!result.empty()){
                error(buffer,result);
                if(debugPrint){
                    cout << "----sort2--"<< result << "----" << endl;
                }
            }else{
                if(httpTestSaveImg){
                    drawPic(requestParamObj);
                }
                if(debugPrint){
                    cout << "----sort2--success----" << endl;
                }
            }
        }else{
            error(buffer,result);
            if(debugPrint){
                cout << "----sort2--"<< result << "----" << endl;
            }
        }
        
        SimpleWeb::CaseInsensitiveMultimap header;
        getHeader(header);
        response->write(SimpleWeb::StatusCode::success_ok, buffer.GetString(), header);
    }catch(const exception &e) {
        *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n"
        << e.what();
        if(debugPrint){
            cout << e.what() << endl;
        }
    }
}




//--------------最多可以接收5个用户请求，第6个让他滚蛋
std::mutex mutex_;
const int maxOperaterNum = 5;
vector<string> acceptUrlList;
bool addAcceptUrl(shared_ptr<HttpServer::Response> &response,
                  string requestId){
//    cout << acceptUrlList.size() << endl;
    std::scoped_lock lock(mutex_);
    if(acceptUrlList.size() < maxOperaterNum){
        acceptUrlList.push_back(requestId);
        return true;
    }
    StringBuffer buffer;
    error(buffer,"server is busy");
    
    SimpleWeb::CaseInsensitiveMultimap header;
    getHeader(header);
    response->write(SimpleWeb::StatusCode::success_ok, buffer.GetString(), header);
    return false;
}
void removeAcceptUrl(string requestId){
    std::scoped_lock lock(mutex_);
    vector<string>::iterator it;
    for(it=acceptUrlList.begin();it!=acceptUrlList.end();)
    {
        if(*it == requestId){
            it = acceptUrlList.erase(it);
        }else{
            ++it;
        }
    }
}
//--------------最多可以接收5个用户请求，第6个让他滚蛋


void startServer(){
    
    HttpServer server;
    server.config.port = default_http_port;
    server.config.timeout_request = 50;
    server.config.timeout_content = 6000;//超时定个10分钟
    server.config.thread_pool_size = 10;
    server.config.reuse_address = true;
    
    //用repoId去取图谱，计算完之后返回给客户端
    server.resource["^/api/sort$"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                                shared_ptr<HttpServer::Request> request) {
        string content = request->content.string();
        if(addAcceptUrl(response,content)){
            auto start  = getTimeStamp(debugPrint);
            sortGraph(response,request,content);
            if(debugPrint){
                printAllCCRefCounter(start);
            }
            removeAcceptUrl(content);
        }
    };
    
    //客户端发来图谱，计算完之后返回给客户端
    server.resource["^/api/sort2$"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                                 shared_ptr<HttpServer::Request> request) {
        string content;
        if(request->content.size() > 1000){
            content = request->content.unZipString();
        }else{
            content = request->content.string();
        }
        
        string requestId;
        auto header_it = request->header.find("sophon-request-id");
        if(header_it != request->header.end()){
            requestId = header_it->second;
            cout << "requestId:" << requestId << endl;
        }
  
        if(addAcceptUrl(response,requestId)){
            auto start  = getTimeStamp(debugPrint);
            sort2(response,content.c_str());
            if(debugPrint){
                printAllCCRefCounter(start);
            }
            removeAcceptUrl(requestId);
        }
    };
    
    // Deals with CORS requests
    server.default_resource["OPTIONS"] = [](shared_ptr<HttpServer::Response> response,
                                            shared_ptr<HttpServer::Request> request) {
        UNUSED(request);
        if(debugPrint){
            cout << "OPTIONS " << request->path << endl;
        }
        try {
            // Set header fields
            SimpleWeb::CaseInsensitiveMultimap header;
            getHeader(header);
            response->write(SimpleWeb::StatusCode::success_ok, "", header);
        }
        catch(const exception &e) {
            response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
        }
    };
    
    
    server.default_resource["GET"] = [](shared_ptr<HttpServer::Response> response,
                                        shared_ptr<HttpServer::Request> request) {
        string path = request->path;
        if(path.compare("/favicon.ico") == 0){
            *response << "HTTP/1.1 200 OK\r\nContent-Length: 0 \r\n" <<
            "Content-Type: text/html" << "\r\n\r\n"
            << "";
        }else{
            if(path.find(".wasm") != path.npos){
                string dir = GetProgramDir();
                path = dir + "/static" + path;
                handle_send_binary(path,response,"application/wasm");
            }else if(path.find(".mem") != path.npos){
                string dir = GetProgramDir();
                path = dir + "/static" + path;
                handle_send_binary(path,response,"application/wasm");
            }
            else{
                if(path.compare("/") == 0){
                    path = "/index.html";
                }
                if(path.find("/index.html") != path.npos){
                    path = "/index.html";
                }
                string content = getFileContent(path);
                if(path.find(".js") != path.npos){
                    *response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.size() << "\r\n" <<
                    "Content-Type: application/x-javascript" << "\r\n\r\n"
                    << content.c_str();
                }else{
                    *response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.size() << "\r\n" <<
                    "Content-Type: text/html" << "\r\n\r\n"
                    << content.c_str();
                }
                
            }
        }
    };
    
    
    server.on_error = [](shared_ptr<HttpServer::Request> request,
                         const SimpleWeb::error_code & errcode/*ec*/) {
        if(debugPrint){
            cout << "on_error " << errcode.message() << endl;
        }else{
            UNUSED(errcode);
        }
        string content = request->content.string();
        removeAcceptUrl(content);
        // Handle errors here
        // Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled

    };
    
    thread server_thread([&server]() {
        server.start();
    });
    
    this_thread::sleep_for(chrono::seconds(1));
    
    cout << "server start! listen post:" << server.config.port << endl;
    server_thread.join();
}


void shellDebug(int argc, char* argv[]){
    
    const char *arg1 = argv[1];
    if(strcmp(arg1, "f") == 0){
        string path = argv[2];
        
        ifstream t(path);
        stringstream stringstreamBuffer;
        stringstreamBuffer << t.rdbuf();
        string contents(stringstreamBuffer.str());
        stringstreamBuffer.clear();
        t.close( );
        
        //        cout << contents << endl;
        
        StringBuffer buffer;
        requestParam requestParamObj;
        sortGraph(contents.c_str(),buffer,requestParamObj);
        drawPic(requestParamObj);
        //        cout << buffer.GetString() << endl;
        
    }else if(strcmp(arg1, "draw") == 0){
        //绘制输入json的图谱 x y
        string path = argv[2];
        ifstream t(path);
        stringstream stringstreamBuffer;
        stringstreamBuffer << t.rdbuf();
        string contents(stringstreamBuffer.str());
        stringstreamBuffer.clear();
        t.close( );
        
        Document document;
        StringBuffer buffer;
        requestParam requestParamObj;
        string result = convert2Obj(contents.c_str(),document,requestParamObj.graphObj);
        if(result.empty()){
            drawPic(requestParamObj);
        }else{
            cout << "----draw--error----" << endl;
        }

    }else if(strcmp(arg1, "c") == 0){
        //生成图谱并测试，然后输出
        requestParam requestParamObj;
        fullNodesAndEdges(requestParamObj,24304,2);
        StringBuffer buffer;
        obj2String(buffer,requestParamObj);
        
        cout << buffer.GetString() << endl;
    }else{
        requestParam requestParamObj;
        requestParamObj.isDraw3d = false;
        unsigned long node_count = 0;
        unsigned long edge_count = 0;
        if(argc >= 3){
            node_count = (int)strtoul(argv[2], NULL, 10);
            edge_count = node_count;
        }
        if(argc >= 4){
            node_count = (int)strtoul(argv[2], NULL, 10);
            edge_count = (int)strtoul(argv[3], NULL, 10);
        }
        if(strcmp(arg1, "3d") == 0){
            requestParamObj.yifanhu_3d = true;
            requestParamObj.isDraw3d = true;
        }
        if(node_count == 0){
            node_count = 50;
            edge_count = 50;
        }
        fullNodesAndEdges(requestParamObj,node_count,edge_count);
        mySort(requestParamObj);
        drawPic(requestParamObj);
        
        //    StringBuffer buffer;
        //    obj2String(buffer,requestParamObj);
        //    cout << buffer.GetString() << endl;
    }
}

int main(int argc, char* argv[]) {
    if(argc > 1){
        if(argc < 5){
            //测试模式
            
            debugPrint = true;
#ifdef MAC_OS
            auto start  = getTimeStamp(debugPrint);
            shellDebug(argc, argv);
            printAllCCRefCounter(start);
#else
            std::cout << "centos上不支持该命令" << std::endl;
#endif
        }else{
            //命令行输入模式
            debugPrint = false;
            string str;
            for(int i = 1; i < argc; i++){
                str.append(argv[i]);
            }
            string result = runCmd(str.c_str());
            cout << result << endl;
        }
    }else{
        debugPrint = true;
        startServer();
    }
    return 0;
}
#else

inline const char* sortInner(const char* content){
    debugPrint = false;
    StringBuffer buffer;
    requestParam requestParamObj;
    
    string result = string2RequestParam(requestParamObj,content,false);
    
    if(result.empty()){
        result = sortGraph(content,buffer,requestParamObj);
        if(!result.empty()){
            error(buffer,result);
            if(debugPrint){
                cout << "----sort--"<< result << "----" << endl;
            }
        }
    }else{
        error(buffer,result);
        if(debugPrint){
            cout << "----sort--"<< result << "----" << endl;
        }
    }
    return buffer.GetString();
}


#include <emscripten.h>
extern "C"  {
    
    EMSCRIPTEN_KEEPALIVE
    const char* sort(const char* content) {
        const char* result = sortInner(content);
        return result;
    }
    
    
//    EMSCRIPTEN_KEEPALIVE
//    int main(int argc, char* argv[]) {
//        cout << "c engine is ready" << endl;
//        //        emscripten_run_script("runSort();");
//        emscripten_exit_with_live_runtime();
//        return 0;
//    }
    
    
    EMSCRIPTEN_KEEPALIVE
    int testPrint() {
        cout << "webAssembly compiled at " << __DATE__  << ", " << __TIME__ << endl;
        return 0;
    }
};
#endif





