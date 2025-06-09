// note: to pass these tests bigint-decimal-str conversion needs to be working

#include <stdint.h>
#include <stdio.h>

#define BIG_INT_IMPLEMENTATION
#include "../../BigInt.h"

void test_case(const char *test_name, char *input, uint32_t shift_by, char* output, size_t expected_size) {
    BigInt n = bigint_alloc();
    char buf[255] = "";

    bigint_set(&n, input);
    printf("%s\n", test_name);
    printf("Initialized with value: %s\n", input);
    printf("Shift by: %d\n", shift_by);

    printf("Size: %zu\n", n.size);
    printf("Memory dump: ");
    bigint_mem_dump(n);
    printf("\n");
    
    bigint_right_shift(&n, shift_by);
    
    printf("Size: %zu\n", n.size);
    printf("Memory dump: ");
    bigint_mem_dump(n);

    bigint_to_dec_str(n, buf, 255);
    printf("Converted to string: %s\n", buf);
    if (strcmp(buf, output) == 0 && n.size == expected_size) {
        printf("\033[32mpass\033[0m\n");
    } else {
        printf("\033[31m");
        if (strcmp(buf, output) != 0) {
            printf("Error: Output mismatch.\n");
            printf("Expected: \"%s\"\n", output);
            printf("Actual:   \"%s\"\n", buf);
        }
        if (n.size != expected_size) {
            printf("Error: Size mismatch.\n");
            printf("Expected size: %zu\n", expected_size);
            printf("Actual size:   %zu\n", n.size);
        }        printf("\033[0m\n");
    }
    printf("------------------------------\n");
}

int main() {
    // Simple shifts
    test_case("Right Shift by 1", "4", 1, "2", 2);
    test_case("Right Shift by 2", "16", 2, "4", 2);

    // No-op shift
    test_case("Right Shift by 0", "12345", 0, "12345", 2);

    // Shifting 1 bit from small number
    test_case("Right Shift small value", "1", 1, "0", 2);

    // Shifting zero
    test_case("Right Shift Zero", "0", 5, "0", 2);

    // Large value right shift
    test_case("Large Value Right Shift", "123456789123456789", 1, "61728394561728394", 3);

    // Large value right shift
    test_case("Large Value Right Shift", "123456789123456789", 26, "1839649515", 2);
    
    // Right shift over full 32-bit word boundary
    test_case("Right Shift by 32 bits (should fail, 2)", "4294967296", 32, "1", 2);

    return 0;
}