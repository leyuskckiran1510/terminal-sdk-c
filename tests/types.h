
#ifndef __SDK_TYPES__
#define __SDK_TYPES__

#include <stdlib.h>
#include <curl/curl.h>



typedef enum{
    SUB_NONE=0,
    SUB_ALLOWED,
    SUB_REQUIRED,
    SUB_COUNT,
}Subscription_E;

static char subscription_map[SUB_COUNT][32] = {
    [SUB_NONE] = "none",
    [SUB_ALLOWED]="allowed",
    [SUB_REQUIRED]="required"
};



typedef enum _ENV_E{
    ENV_DEV=0,
    ENV_PROD,
    ENV_COUNT,
}ENV_E;

static char env_map[ENV_COUNT][32] = {
    [ENV_DEV]="api.dev.",
    [ENV_PROD]="api."
};

typedef size_t (*Writeable)(void *ptr, size_t size, size_t nmemb, void *userdata); 

typedef struct _ProductVariant{
    char id[32];
    char name[32];
    double price;
}ProductVariant;

typedef struct _Product{
    char id[32];
    char name[64];
    char *description;
    uint order;
    Subscription_E subscription;
    ProductVariant *variants;
    char **tags;
} Product;


// RI -> requests interface
typedef struct _ProductRI{
    Product*(*list)(void);
    Product (*get)(int id);
} ProductRI;

typedef struct _Terminal{
    char bearer_token[32];
    ENV_E environment;
    CURL *curl;
    ProductRI product;
}Terminal;


static Terminal sdk_terminal;


#endif
