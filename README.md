# BigInt-C

`BigInt.h` is a stb style header file for handling arbitrarily large integers in c

## How to use?

- as header file

```c
#include "BigInt.h" // only includes declarations
```

- as source file

```c
#define BIG_INT_IMPLEMENTATION  // include implementation
#include "BigInt.h"
int main() {
    BigInt n = bigint_alloc();
    char decimal_str[] = "12345678900";
    bigint_set(&n, decimal_str);
}
```

## Feature Todo List

- support negative numbers
- support addition, subtraction operations
- support multiplication, division operations
- support mod operation
- support comparison operators (<, >, ==, !=, <=, >=)
- support conversion between different bases (Hex, Decimal, Octal, Binary)
