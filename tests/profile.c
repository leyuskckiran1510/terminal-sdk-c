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

size_t profile_put_callback(void *data, size_t size, size_t nmemb, void *status) {
    ResponseStatus *_status = (ResponseStatus *)status;
    size_t total_size = size * nmemb;
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        return -1;
    }
    // is sucessfull we recive data
    cJSON *item = cJSON_GetObjectItem(json, "data");
    if (item) {
        cJSON_Delete(json);
        strcpy(_status->message,"Sucessfully patched profile.");
        return total_size;
    }
    
    //  on error we recive message
    item = cJSON_GetObjectItem(json, "message");
    if (!item) {
        cJSON_Delete(json);
        return total_size;
    }
    strcpy(_status->message,item->valuestring);
    return total_size;
}

ResponseStatus profile_update(ProfileUpdate pu){
    char *data = calloc(sizeof(pu)*2,1);
    ResponseStatus status;
    sprintf(data,"{\"name\":\"%s\",\"email\":\"%s\"}",pu.name,pu.email);
    memset(URL_BUILD_BUFFER,0,sizeof(URL_BUILD_BUFFER));
    sprintf(URL_BUILD_BUFFER,"https://%s%s",env_map[sdk_terminal.environment],url_profile());
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_WRITEDATA, &status);
    status.status_code = requests_put(URL_BUILD_BUFFER,data,profile_put_callback);
    printf("%s a=%d\n",status.message,status.status_code);
    free(data);
    return status;
}