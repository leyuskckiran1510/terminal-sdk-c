


#include <curl/curl.h>
#include "types.h"


CURLcode requests_get(CURL *curl, char *url,Writeable writefunction){
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunction);
    return curl_easy_perform(curl);
}

