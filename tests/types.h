
#ifndef __SDK_TYPES__
#define __SDK_TYPES__

#include <stdlib.h>
#include <curl/curl.h>

#define def_struct(x,y) typedef struct _##x y x;
#define def_enum(x,y) typedef enum _##x y x;



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


def_struct(ProductVariant,{
    char id[32];
    char name[32];
    double price;
})


def_struct(ProductTag,{
    int featured;
    int market_eu;
    int market_na;
    char app[32];
    char color[8];
})

def_struct(Product,{
    char id[32];
    char subscription[32];
    char name[64];
    ProductTag tags;
    ProductVariant *variants;
    char *description;
    uint order;
});


// RI -> requests interface
def_struct(ProductRI,{
    Product*(*list)(void);
    Product (*get)(char * id);
});

/*

{'data': 
    {'user': 
        {
            'id': 'usr_01JQ87AAF2VXNJC6288E7XSX12',
            'name': None,
            'email': None,
            'fingerprint': None,
            'stripeCustomerID': 'cus_S0m1XJdaP7oxIw'
        }
    }
}

*/


def_struct(Profile ,{
    char id[32];
    char email[32];
    char name[64];
    char fingerprint[128];
    char stripeCustomerID[32];
});



def_struct(ProfileUpdate,{
    char name[64];
    char email[32];
});

def_struct(ProfileRI,{
    Profile (*get)();
    int (*update)(ProfileUpdate);
});

def_struct(Terminal,{
    char bearer_token[32];
    ENV_E environment;
    CURL *curl;
    ProductRI product;
    ProfileRI profile;
});


static Terminal sdk_terminal;


#endif
