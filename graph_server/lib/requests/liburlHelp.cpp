#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

size_t writeToString(void *ptr, size_t size, size_t count, void *stream)
{
    ((std::string*)stream)->append((char*)ptr, 0, size* count);
    return size* count;
}

bool getData(string url,vector<string> &headerList,std::string& response)
{
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        for(string field : headerList){
            if(field.find("SOPHON") != field.npos){
                headers = curl_slist_append(headers, field.c_str());
                continue;
            }
            if(field.find("sophon") != field.npos){
                headers = curl_slist_append(headers, field.c_str());
                continue;
            }
            if(field.find("Sophon") != field.npos){
                headers = curl_slist_append(headers, field.c_str());
                continue;
            }
        }
        headers = curl_slist_append(headers, "Accept: */*");
        headers = curl_slist_append(headers, "User-Agent: PostmanRuntime/7.6.1");
        headers = curl_slist_append(headers, "Postman-Token: 4f52a0c9-a771-4e0e-989c-b0cdc08b737a");
        headers = curl_slist_append(headers, "cache-control: no-cache");
        headers = curl_slist_append(headers, "accept-Encoding: gzip, deflate");
        headers = curl_slist_append(headers, "Connection: keep-alive");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        // data now holds response
        
        // write curl response to string variable..
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            return false;
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        return true;
    }else{
        return false;
    }
    
}





bool postData(string url,
              const char* postParams,
              std::string& response)
{
    // init curl
    CURL *curl = curl_easy_init();
    // res code
    CURLcode res;
    if (curl)
    {
        // set params
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams); // params
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
//        curl_easy_setopt(curl, CURLOPT_HEADER, 1); //打开会打印返回头
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 300000);
        // start req
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return false;
        }
    }
    // release curl
    curl_easy_cleanup(curl);
    return true;
}
