#define __DEBUG__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "product.c"
#include "profile.c"


void clean_up(){
    if(sdk_terminal.curl)
        curl_easy_cleanup(sdk_terminal.curl);
}

void init_terminal(char *bearer_token,ENV_E environment){
    memcpy(sdk_terminal.bearer_token,bearer_token,strlen(bearer_token));
    sdk_terminal.environment = environment;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    sdk_terminal.curl = curl_easy_init();
    if(sdk_terminal.curl==NULL){
        fprintf(stderr,"Error initializing curl...");
        exit(1);
    }
    
    sdk_terminal.product.list = product_list;
    sdk_terminal.product.get = product_get;
    sdk_terminal.profile.get = profile_get;
    sdk_terminal.profile.update = profile_update;
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

void single_product_demo(){
    Product prod = sdk_terminal.product.get("prd_01JD0E7PD4H3XDZA5P5VXSDPQC");
    printf("name:= %s\n",prod.name);
    if(prod.variants){
        printf("id:=%s , name:= %s, price:= %lf \n",prod.variants[0].id,prod.variants[0].name,prod.variants[0].price);
    }
    printf("tag items:- %s %s %d",prod.tags.app,prod.tags.color,prod.tags.featured);
}

void profile_get_demo(){
    Profile profile = sdk_terminal.profile.get();
    printf("id:=%s\n",profile.id);
    printf("name:=%s\n",profile.name);
    
}

void profile_put_demo(){
    ProfileUpdate pu = {
        .name="kiran raj dhakal",
        .email="tester@teser.com",
    };
    ResponseStatus status = sdk_terminal.profile.update(pu); 
    printf("msg: %s; code: %u\n",status.message,status.status_code);
    if(status.status_code!=200){
        printf("Fix your email\n");
        return;
    }
    profile_get_demo();
}

int main() {
    char *token = getenv("TOKEN");
    if(token==NULL){
        fprintf(stderr,"Must Provide TOKEN of term-cofee\n"
                "\tTOKEN=your_token ./<executable>\n");
        exit(1);
    }
    init_terminal(token,ENV_DEV);
    
}
