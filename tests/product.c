#include "constants.h"
#include "types.h"
#include "basic_requests.c"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

typedef struct{
    Product *products;
    size_t *size;
    size_t *used;
    ResponseStatus *status;
} __internal_product_list_callback_parmas;


void parse_varients(cJSON *_item,ProductVariant *vars,int count){
    for (int i = 0; i < count ; i++) {
        ProductVariant *var = &vars[i];
        cJSON *item = cJSON_GetArrayItem(_item, i);
        if (!item) continue;
        cJSON *id = cJSON_GetObjectItem(item, "id");
        cJSON *name = cJSON_GetObjectItem(item, "name");
        cJSON *price = cJSON_GetObjectItem(item, "price");
        if(id){
            strncpy(var->id,id->valuestring,sizeof(var->id));
        }
        if(name){
            strncpy(var->name,name->valuestring, sizeof(var->name));
        }
        if(price){
            var->price = price->valuedouble;
        }
    }

}


void parse_tags(cJSON *tags,ProductTag *tag){
    cJSON *app = cJSON_GetObjectItem(tags, "app");
    if(app)
        strncpy(tag->app,app->valuestring,sizeof(tag->app)-1);

    cJSON *color = cJSON_GetObjectItem(tags, "color");
    if(color)
        strncpy(tag->color,color->valuestring,sizeof(tag->color)-1);
    

    cJSON *featured = cJSON_GetObjectItem(tags, "featured");
    if(featured)
        tag->featured = featured->valueint;
    cJSON *market_eu = cJSON_GetObjectItem(tags, "market_eu");
    if(market_eu)
        tag->market_eu = market_eu->valueint;
    cJSON *market_na = cJSON_GetObjectItem(tags, "market_na");
    if(market_na)
        tag->market_na = market_na->valueint;
}   
void parse_product(cJSON *item,Product *prod){
    cJSON *id = cJSON_GetObjectItem(item, "id");
    cJSON *name = cJSON_GetObjectItem(item, "name");
    cJSON *desc = cJSON_GetObjectItem(item, "description");
    cJSON *order = cJSON_GetObjectItem(item, "order");
    cJSON *subscription = cJSON_GetObjectItem(item, "subscription");
    // variants logic
    cJSON *variants = cJSON_GetObjectItem(item, "variants");
    if (cJSON_IsArray(variants)) {
        int count = cJSON_GetArraySize(variants);
        prod->variants = calloc(count,sizeof(ProductVariant));
        parse_varients(variants,prod->variants,count);
    }
    
    // tags logic
    cJSON *tags = cJSON_GetObjectItem(item, "tags");
    if(tags){
        cJSON *item = NULL;
        int count=0;
        cJSON_ArrayForEach(item, tags) {
                count++;
        }
        parse_tags(tags,&prod->tags);
    }
    
    if (id && name && desc ) {
        strncpy(prod->id, id->valuestring, sizeof(prod->id) - 1);
        strncpy(prod->name, name->valuestring, sizeof(prod->name) - 1);
        prod->description = calloc(strlen(desc->valuestring),sizeof(char));
        strncpy(prod->description, desc->valuestring, sizeof(prod->description) - 1);
        if(subscription)
            strncpy(prod->subscription, subscription->valuestring, sizeof(prod->subscription) - 1);
        if(order)
            prod->order = order->valueint;
    }
}

size_t product_list_callback(void *data,size_t size,size_t nmemb,void* params){
    __internal_product_list_callback_parmas *_parms = (__internal_product_list_callback_parmas*)params;
    size_t total_size = size * nmemb;
    Product *products = _parms->products;
    
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        free(products);
        return -1;
    }
    cJSON *data_obj = cJSON_GetObjectItem(json, "data");
    if (!cJSON_IsArray(data_obj)) {
        data_obj = cJSON_GetObjectItem(json,"message");
        strcpy(_parms->status->message,data_obj->valuestring);
        cJSON_Delete(json);
        return -1;
    }
    size_t count = (size_t)cJSON_GetArraySize(data_obj);
    sprintf(_parms->status->message, "Sucessfully fetched %lu products.", count);
    if(count >*_parms->size){
        count = *_parms->size;
    }

    for (size_t i = 0; i < count ; i++) {
        cJSON *item = cJSON_GetArrayItem(data_obj, i);
        if (!item) continue;
        parse_product(item,&products[i]);
    }
    *_parms->used = count;
    cJSON_Delete(json);
    return total_size;
}


ResponseStatus product_list(Product* *output,size_t in_size,size_t* out_size){
    ResponseStatus status;
    Product *products= *output;
    __internal_product_list_callback_parmas prd_ls_c_params = {
        .products = products,
        .size = &in_size,
        .used = out_size,
        .status = &status,
    };
    memset(URL_BUILD_BUFFER,0,sizeof(URL_BUILD_BUFFER));
    sprintf(URL_BUILD_BUFFER,"https://%s%s",env_map[sdk_terminal.environment],url_product());
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_WRITEDATA, &prd_ls_c_params);
    status.status_code = requests_get(URL_BUILD_BUFFER, product_list_callback);
    return status;
}

size_t product_get_callback(void *data,size_t size,size_t nmemb,void* void_prod){
    size_t total_size = size * nmemb;
    __internal_product_list_callback_parmas *_parms = (__internal_product_list_callback_parmas*)void_prod;
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
    strcpy(_parms->status->message,"Sucessfully fetched product.");
    parse_product(item,_parms->products);
    cJSON_Delete(json);
    return total_size;
}


ResponseStatus product_get(char *id,Product *output){
    size_t _size_1 = 1;
    ResponseStatus status;
    __internal_product_list_callback_parmas prd_ls_c_params = {
        .products= output,
        .size = &_size_1,
        .used = &_size_1,
        .status = &status,
    };
    memset(URL_BUILD_BUFFER,0,sizeof(URL_BUILD_BUFFER));
    sprintf(URL_BUILD_BUFFER,"https://%s%s/%s",env_map[sdk_terminal.environment],url_product(),id);
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_WRITEDATA, &prd_ls_c_params);
    status.status_code = requests_get(URL_BUILD_BUFFER, product_get_callback);
    return status;
}