#include "types.h"
#include "basic_requests.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

size_t output(void *data,size_t size,size_t nmemb,void* prod_addr){
    size_t total_size = size * nmemb;
    Product *products = calloc(100, sizeof(Product));
    (void) size;
    (void) nmemb;
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        free(products);
        return -1;
    }
    cJSON *data_obj = cJSON_GetObjectItem(json, "data");
    if (!cJSON_IsArray(data_obj)) {
        cJSON_Delete(json);
        free(products);
        return -1;
    }
    int count = cJSON_GetArraySize(data_obj);
    for (int i = 0; i < count ; i++) {
        Product *prod = &products[i];
        cJSON *item = cJSON_GetArrayItem(data_obj, i);
        if (!item) continue;

        cJSON *id = cJSON_GetObjectItem(item, "id");
        cJSON *name = cJSON_GetObjectItem(item, "name");
        cJSON *desc = cJSON_GetObjectItem(item, "description");

        cJSON *variants = cJSON_GetObjectItem(item, "variants");
        cJSON *first_variant = (variants && cJSON_IsArray(variants)) ? cJSON_GetArrayItem(variants, 0) : NULL;
        cJSON *price = first_variant ? cJSON_GetObjectItem(first_variant, "price") : NULL;

        cJSON *tags = cJSON_GetObjectItem(item, "tags");
        cJSON *featured = tags ? cJSON_GetObjectItem(tags, "featured") : NULL;

        if (id && name && desc) {
            strncpy(prod->id, id->valuestring, sizeof(prod->id) - 1);
            strncpy(prod->name, name->valuestring, sizeof(prod->name) - 1);
            prod->description = calloc(strlen(desc->valuestring),sizeof(char));
            strncpy(prod->description, desc->valuestring, sizeof(prod->description) - 1);
        }
    }
    strncpy((products)[count].id,"__end__",10);
    cJSON_Delete(json);
    prod_addr = &products;
    return total_size;
}



Product * product_list(){
    Product *products;
    build_url(sdk_terminal.environment,url_product());
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_WRITEDATA, &products);
    requests_get(sdk_terminal.curl, URL_BUILD_BUFFER, output);
    return products;
}
