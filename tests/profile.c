#include "constants.h"
#include "types.h"
#include "basic_requests.c"
#include <curl/easy.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"



void parse_profile(cJSON *user,void *profile){
    Profile *__pp = profile;
    json_item(user,id);
    json_item(user,name)
    json_item(user,email)
    json_item(user,fingerprint)
    json_item(user,stripeCustomerID)
    #define copy_if(X) if(X && X->valuestring != NULL) strncpy(__pp->X,X->valuestring, strlen(X->valuestring));
    copy_if(id)
    copy_if(name)
    copy_if(email)
    copy_if(fingerprint)
    copy_if(stripeCustomerID)
    #undef copy_if
}


size_t profile_get_callback(void *data,size_t size,size_t nmemb,void* void_prod){
    size_t total_size = size * nmemb;
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        return -1;
    }
    cJSON *item = cJSON_GetObjectItem(json, "data");
    if (!item) {
        cJSON_Delete(json);
        return -1;
    }
    cJSON *user = cJSON_GetObjectItem(item,"user");
    if(!user){
        cJSON_Delete(user);
        return -1;
    }
    parse_profile(user,void_prod);
    cJSON_Delete(json);
    return total_size;
}


Profile profile_get(){
    Profile profile = {0};
    memset(URL_BUILD_BUFFER,0,sizeof(URL_BUILD_BUFFER));
    sprintf(URL_BUILD_BUFFER,"https://%s%s",env_map[sdk_terminal.environment],url_profile());
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_WRITEDATA, &profile);
    requests_get(URL_BUILD_BUFFER, profile_get_callback);
    return profile;
}

size_t __write_callback(void *data, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        return -1;
    }
    // is sucessfull we recive data
    cJSON *item = cJSON_GetObjectItem(json, "data");
    if (item) {
        cJSON_Delete(json);
        return -1;
    }
    //  on error we recive message
    item = cJSON_GetObjectItem(json, "message");
    if (!item) {
        cJSON_Delete(json);
        return -1;
    }
    *(int *)userp=0;
    return total_size;
}

int profile_update(ProfileUpdate pu){
    char *data = calloc(sizeof(pu)*2,1);
    int success=1;
    sprintf(data,"{\"name\":\"%s\",\"email\":\"%s\"}",pu.name,pu.email);
    memset(URL_BUILD_BUFFER,0,sizeof(URL_BUILD_BUFFER));
    sprintf(URL_BUILD_BUFFER,"https://%s%s",env_map[sdk_terminal.environment],url_profile());
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_WRITEDATA, &success);
    requests_put(URL_BUILD_BUFFER,data,__write_callback);
    free(data);
    return success;
}