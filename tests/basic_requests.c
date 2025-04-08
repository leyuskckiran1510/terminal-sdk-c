
#ifndef __B_REQ__
#define __B_REQ__
#include <curl/curl.h>
#include "types.h"
#include "curl_code.h"



#define authorize() char auth_header[256];\
    struct curl_slist *headers = NULL;\
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", sdk_terminal.bearer_token);\
    headers = curl_slist_append(headers, auth_header);\
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_HTTPHEADER, headers);\

CURLcode requests_get(char *url,Writeable writefunction){
    #ifdef __DEBUG__
        curl_easy_setopt(sdk_terminal.curl, CURLOPT_VERBOSE, 1L);
    #endif

    authorize();
    
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_URL, url);
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_WRITEFUNCTION, writefunction);
    CURLcode code = curl_easy_perform(sdk_terminal.curl);
    curl_slist_free_all(headers);
    return code;
}

CURLcode requests_put(char *url,void *json_data,Writeable writefunction){
    (void) writefunction;
    #ifdef __DEBUG__
        curl_easy_setopt(sdk_terminal.curl, CURLOPT_VERBOSE, 1L);
    #endif
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_URL, url);
    
     authorize();

    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_POSTFIELDS, json_data);
    CURLcode code = curl_easy_perform(sdk_terminal.curl);
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_CUSTOMREQUEST, NULL);
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_POSTFIELDS, NULL);
    curl_slist_free_all(headers);
    return code;
}
#endif