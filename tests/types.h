
#ifndef __SDK_TYPES__
#define __SDK_TYPES__

#include <stdlib.h>
#include <curl/curl.h>


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


typedef struct _ProductTag{
    int featured;
    int market_eu;
    int market_na;
    char app[32];
    char color[8];
} ProductTag;

typedef struct _Product{
    char id[32];
    char subscription[32];
    char name[64];
    ProductTag tags;
    ProductVariant *variants;
    char *description;
    uint order;
} Product;


// RI -> requests interface
typedef struct _ProductRI{
    Product*(*list)(void);
    Product (*get)(char * id);
} ProductRI;

typedef struct _Terminal{
    char bearer_token[32];
    ENV_E environment;
    CURL *curl;
    ProductRI product;
}Terminal;


static Terminal sdk_terminal;


#endif
