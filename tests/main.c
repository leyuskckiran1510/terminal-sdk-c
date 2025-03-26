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
    atexit(clean_up);
}


int main() {
    init_terminal(getenv("TOKEN"),ENV_DEV);
    Product *products = sdk_terminal.product.list();
    int count =0;
    while(count<100){
        printf("name:= %s\n",products[count].name);
        count++;
    }
    return 0;
}
