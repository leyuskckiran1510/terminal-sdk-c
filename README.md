# Terminal Api C SDK


## How to use
 [ ps this is the motivation, work in progress ]
```c
#define TERM_SDK_IMPL
#include "term_sdk.h"

int main(){
    Terminal client = {
            .bearer_token=get_envrion("TOKEN"),
            .environment="dev",
        };
    Product product[100] = client.product.list();
    print(Product,product);
}
```


### notes
- [ApiDoc](https://www.terminal.shop/api)
- [ApiRendredSwagger](./endpoint_docs/api_docs.html)
- ```c
/*
list -> get
get -> get

update -> put
add -> put
set -> put

create -> post
collect -> post
convert -> post

delete -> delete
cancel -> delete
*/
```