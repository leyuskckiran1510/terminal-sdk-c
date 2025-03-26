# Terminal Api C SDK


## How to use
 [ ps this is the motivation, work in progress ]
```c
#define TERM_SDK_IMPL
#include "term_sdk.h"
// sdk_terminal is already defined,
// it is a gloabl vairbale
int main() {
    init_terminal(getenv("TOKEN"),ENV_DEV);
    skd_terminal.product.list();
    return 0;
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