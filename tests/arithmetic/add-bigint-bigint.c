#include <stdio.h>

#define BIG_INT_IMPLEMENTATION
#include "../../BigInt.h"

int main() {
    BigInt n1 = bigint_alloc();
    BigInt n2 = bigint_alloc();
    BigInt res = bigint_alloc();

    // char decimal_str1[] = "1152921504606846976";
    char decimal_str1[] = "75025";
    char decimal_str2[] = "46368";
    // char decimal_str[] = "69696";
    bigint_set(&n1, decimal_str1);
    bigint_set(&n2, decimal_str2);

    bigint_add(&res, &n1, &n2);
    char buf[255] = "";
    bigint_to_dec_str(res, buf, 255);
    printf("result\n%s\n", buf);

    return 0;
}
