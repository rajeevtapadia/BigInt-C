// note: to pass these tests bigint-decimal-str conversion needs to be working

#include <stdint.h>
#include <stdio.h>

#define BIG_INT_IMPLEMENTATION
#include "../../BigInt.h"

void test_case(const char *test_name, char *input, uint32_t shift_by, char* output) {
    BigInt n = bigint_alloc();
    char buf[255] = "";

    bigint_set(&n, input);
    printf("%s\n", test_name);
    printf("Initialized with value: %s\n", input);
    printf("Shift by: %d\n", shift_by);

    printf("Memory dump: ");
    bigint_mem_dump(n);
    
    bigint_left_shift(&n, shift_by);
    
    printf("Memory dump: ");
    bigint_mem_dump(n);

    bigint_to_dec_str(n, buf, 255);
    printf("Converted to string: %s\n", buf);
    if (strcmp(buf, output) == 0) {
        printf("\n");
        printf("\033[32mpass\033[0m\n");
    } else {
        printf("\033[31mfail\033[0m\n");
    }
    printf("------------------------------\n");
}

int main() {
    // Simple shift
    test_case("Simple Shift", "1", 1, "2");
    test_case("Shift by 2", "3", 2, "12");
    test_case("Shift by 0", "12345", 0, "12345");

    // Edge: zero input
    test_case("Zero Shift", "0", 5, "0");

    // Edge: large number shift
    test_case("Large Number Shift", "999999999", 4, "15999999984");

    // Edge: shift by 1 on large input
    test_case("Shift Large Value by 1", "123456789123456789", 1, "246913578246913578");

    // Edge: shift resulting in carry across words
    test_case("Word Boundary Shift", "4294967296", 1, "8589934592");

    // Edge: shift by 31 bits (near word width for 32-bit words)
    test_case("Shift by 31 bits", "1", 31, "2147483648");

    // Edge: shift by 32 bits (exact word width) 
    test_case("Shift by 32 bits (should crash)", "1", 32, "4294967296");

    return 0;
}
