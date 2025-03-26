#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "product.c"



void clean_up(){
    if(client.curl)
        curl_easy_cleanup(client.curl);
}

void init_terminal(char *bearer_token,ENV_E environment){
    struct curl_slist *headers = NULL;
    char auth_header[256];
    
    memcpy(client.bearer_token,bearer_token,strlen(bearer_token));
    client.environment = environment;
    client.curl = curl_easy_init();
    if(client.curl==NULL){
        fprintf(stderr,"Error initializing curl...");
        exit(1);
    }
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", bearer_token);
    headers = curl_slist_append(headers, auth_header);
    curl_easy_setopt(client.curl, CURLOPT_HTTPHEADER, headers);
    client.product.list = product_list;
    atexit(clean_up);
}


int main() {
    init_terminal(getenv("TOKEN"),ENV_DEV);
    client.product.list();
    return 0;
}
