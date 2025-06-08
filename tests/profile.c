#include "constants.h"
#include "types.h"
#include "basic_requests.c"
#include <curl/curl.h>
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
    #define copy_if(X) if(X && X->valuestring != NULL) strncpy(__pp->X,X->valuestring, sizeof(__pp->X)-1);
    copy_if(id)
    copy_if(name)
    copy_if(email)
    copy_if(fingerprint)
    copy_if(stripeCustomerID)
    #undef copy_if
}


def_struct(__internal_profile_callback_parms, {
    Profile *profile;
    ResponseStatus *status;
});

size_t profile_get_callback(void *data,size_t size,size_t nmemb,void* parms){
    __internal_profile_callback_parms *_parms = (__internal_profile_callback_parms *)parms;
    size_t total_size = size * nmemb;
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        return -1;
    }
    cJSON *item = cJSON_GetObjectItem(json, "data");
    if (!item) {
        item = cJSON_GetObjectItem(json,"message");
        strcpy(_parms->status->message,item->valuestring);
        cJSON_Delete(json);
        return -1;
    }
    cJSON *user = cJSON_GetObjectItem(item,"user");
    if(!user){
        cJSON_Delete(user);
        return -1;
    }
    strcpy(_parms->status->message,"Sucessfully fetched profile.");
    parse_profile(user,_parms->profile);
    cJSON_Delete(json);
    return total_size;
}


ResponseStatus profile_get(Profile *output){
    ResponseStatus status;
    __internal_profile_callback_parms parms = {
        .profile = output,
        .status = &status,
    };
    memset(URL_BUILD_BUFFER,0,sizeof(URL_BUILD_BUFFER));
    sprintf(URL_BUILD_BUFFER,"https://%s%s",env_map[sdk_terminal.environment],url_profile());
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_WRITEDATA, &parms);
    status.status_code =  requests_get(URL_BUILD_BUFFER, profile_get_callback);
    return status;
}


ResponseStatus profile_update(ProfileUpdate pu,Profile *output){
    ResponseStatus status;
    __internal_profile_callback_parms parms = {
        .profile = output,
        .status = &status,
    };
    // 32 if for other json strings, name,email and quotes and commans
    char *data = calloc(1,sizeof(pu)+32);
    sprintf(data,"{\"name\":\"%s\",\"email\":\"%s\"}",pu.name,pu.email);
    memset(URL_BUILD_BUFFER,0,sizeof(URL_BUILD_BUFFER));
    sprintf(URL_BUILD_BUFFER,"https://%s%s",env_map[sdk_terminal.environment],url_profile());
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_WRITEDATA, &parms);
    status.status_code = requests_put(URL_BUILD_BUFFER,data,profile_get_callback);
    free(data);
    return status;
}