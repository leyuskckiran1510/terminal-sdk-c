#ifndef __SDK_CONSTANTS__
#define __SDK_CONSTANTS__
#define BASE_URL "terminal.shop"

#define url_product() BASE_URL "/product"
#define url_profile() BASE_URL "/profile"


#define json_item(json,key)  cJSON *key =  cJSON_GetObjectItem(json,#key);

static char URL_BUILD_BUFFER[64];

#endif