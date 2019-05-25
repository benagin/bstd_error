# bstd::error

Throw more useful errors.

This is a header only library.

## Usage

### Clone

1. ```git clone git@github.com:benagin/bstd_error.git```
2. ```cd bstd_error```

##### Available build targets
1. Build everything: ```make``` or ```make all```
2. Build examples: ```make examples```

### Include public headers

Assuming the path to `bstd_error` is included with ```-I<path-to-bstd_error>``` or something similar:

* All public headers: ```#include "bstd_error/include/bstd_error.hpp"```

Alternatively, include ```bstd_error/include```. In gcc or equivalent:
* ```-Ibstd_error/include```

Then use (depending on which headers you need):
* ```#include <bstd_error.hpp>```
