#include <stdio.h>
#include <string.h>

#define BIG_INT_IMPLEMENTATION
#include "../BigInt.h"

BigInt fibo_dp(unsigned long long n) {
    if (n < 1)
        return bigint_alloc();
    BigInt prev2 = bigint_alloc();
    BigInt prev = bigint_alloc();
    BigInt curr = bigint_alloc();

    bigint_set(&prev2, "0");
    bigint_set(&prev, "1");
    bigint_set(&curr, "1");
    for (unsigned long long i = 2; i < n; i++) {
        // bigint_mem_dump(prev2);
        // bigint_mem_dump(prev);
        bigint_deep_copy(&prev2, &prev);
        bigint_deep_copy(&prev, &curr);
        bigint_add(&curr, &prev, &prev2);
        // if (i > 22) {
        //     bigint_mem_dump(curr);
        //     printf("--------------------\n");
        // }
    }
    return curr;
}

int main(int argc, char **argv) {
    // int n = atoi(argv[1]);
    // int n = 500;
    char buf[255] = "";
    // for (size_t i = 1; i <= n; i++) {
        memset(buf, '\0', 255);
        printf("fibo(%zu)\n", 500);
        BigInt res = fibo_dp(500);
        bigint_to_dec_str(res, buf, 255);
        printf("buf: %25s\n", buf);
    // }
    return 0;
}
