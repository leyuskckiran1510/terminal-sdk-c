


#include <curl/curl.h>
#include "types.h"
#include "constants.h"
#include <string.h>

void build_url(ENV_E environment,const char* api){
    memset(URL_BUILD_BUFFER,0,sizeof(URL_BUILD_BUFFER));
    sprintf(URL_BUILD_BUFFER,"https://%s%s",env_map[environment],api);
}


CURLcode requests_get(CURL *curl, char *url,Writeable writefunction){
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunction);
    return curl_easy_perform(curl);
}

