#include "types.h"
#include "basic_requests.c"
#include <stdio.h>

size_t output(void *data,size_t size,size_t nmemb,void* _){
    (void)_;
    fwrite(data,size, nmemb,stdout);
    printf("\n");
    return 1;
}

Product * product_list(){
    build_url(client.environment,url_product());
    requests_get(client.curl, URL_BUILD_BUFFER, output);
    return NULL;
}
