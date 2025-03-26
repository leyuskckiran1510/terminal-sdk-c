#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "product.c"



void clean_up(){
    if(sdk_terminal.curl)
        curl_easy_cleanup(sdk_terminal.curl);
}

void init_terminal(char *bearer_token,ENV_E environment){
    struct curl_slist *headers = NULL;
    char auth_header[256];
    
    memcpy(sdk_terminal.bearer_token,bearer_token,strlen(bearer_token));
    sdk_terminal.environment = environment;
    sdk_terminal.curl = curl_easy_init();
    if(sdk_terminal.curl==NULL){
        fprintf(stderr,"Error initializing curl...");
        exit(1);
    }
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", bearer_token);
    headers = curl_slist_append(headers, auth_header);
    curl_easy_setopt(sdk_terminal.curl, CURLOPT_HTTPHEADER, headers);
    sdk_terminal.product.list = product_list;
    sdk_terminal.product.get = product_get;
    atexit(clean_up);
}

void products_list_demo(){
    Product *products = sdk_terminal.product.list();
    int count =0;
    while(count<100){
        if(PRODUCT_IS_EOF(products[count])) break;
        printf("name:= %s\n",products[count].name);
        count++;
    }
}

int main() {
    init_terminal(getenv("TOKEN"),ENV_DEV);
    Product prod = sdk_terminal.product.get("prd_01JD0E7PD4H3XDZA5P5VXSDPQC");
    printf("name:= %s\n",prod.name);
    if(prod.variants){
        printf("id:=%s , name:= %s, price:= %lf \n",prod.variants[0].id,prod.variants[0].name,prod.variants[0].price);
    }
    printf("tag items:- %s %s %d",prod.tags.app,prod.tags.color,prod.tags.featured);
    
    return 0;
}
