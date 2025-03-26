#include "constants.h"
#include "types.h"
#include "basic_requests.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#define PRODUCT_IS_EOF(product) strcmp(product.id,"__end__")==0

void parse_varients(cJSON *item,ProductVariant *vars,int count){
    for (int i = 0; i < count ; i++) {
        ProductVariant *var = &vars[i];
        cJSON *item = cJSON_GetArrayItem(item, i);
        if (!item) continue;
        cJSON *id = cJSON_GetObjectItem(item, "id");
        cJSON *name = cJSON_GetObjectItem(item, "name");
        cJSON *price = cJSON_GetObjectItem(item, "price");
        if(id){
            strncpy(var->id,id->valuestring, strlen(id->valuestring));
        }
        if(name){
            strncpy(var->name,name->valuestring, strlen(name->valuestring));
        }
        if(price){
            var->price = price->valuedouble;
        }
    }

}


void parse_tags(cJSON *tags,ProductTag *tag){
    cJSON *app = cJSON_GetObjectItem(tags, "app");
    if(app)
        strncpy(tag->app,app->valuestring,strlen(tag->app)-1);
    cJSON *color = cJSON_GetObjectItem(tags, "color");
    if(color)
        strncpy(tag->color,color->valuestring,strlen(tag->color)-1);
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

size_t product_list_callback(void *data,size_t size,size_t nmemb,void* void_prod){
    size_t total_size = size * nmemb;
    Product *products = (Product *)void_prod;
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
        parse_product(item,prod);
    }
    strncpy((products)[count].id,"__end__",10);
    cJSON_Delete(json);
    return total_size;
}


Product * product_list(){
    Product *products=calloc(100, sizeof(Product));
    memset(URL_BUILD_BUFFER,0,sizeof(URL_BUILD_BUFFER));
    sprintf(URL_BUILD_BUFFER,"https://%s%s",env_map[sdk_terminal.environment],url_product());
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_WRITEDATA, products);
    requests_get(sdk_terminal.curl, URL_BUILD_BUFFER, product_list_callback);
    return products;
}

size_t product_get_callback(void *data,size_t size,size_t nmemb,void* void_prod){
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
    parse_product(item,void_prod);
    cJSON_Delete(json);
    return total_size;
}


Product product_get(char *id){
    Product product={0};
    memset(URL_BUILD_BUFFER,0,sizeof(URL_BUILD_BUFFER));
    sprintf(URL_BUILD_BUFFER,"https://%s%s/%s",env_map[sdk_terminal.environment],url_product(),id);
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_WRITEDATA, &product);
    requests_get(sdk_terminal.curl, URL_BUILD_BUFFER, product_get_callback);
    return product;
}