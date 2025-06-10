#include <stdio.h>

#define BIG_INT_IMPLEMENTATION
#include "../../BigInt.h"
#include "../ANSI-color-macros.h"

void test_case(const char *test_name, char *input, bool is_negative, size_t expected_size) {
    BigInt n = bigint_alloc();
    char buf[255] = "";

    // Initialize BigInt with the given input string
    bigint_set(&n, input);
    printf("%s - Initialized with value: %s\n", test_name, input);

    // Dump memory representation
    printf("Memory dump: ");
    bigint_mem_dump(n);

    // Convert BigInt to decimal string and print it
    bigint_to_dec_str(n, buf, 255);
    printf("Converted to string: %s\n", buf);
    bool output_match = (strcmp(input, buf) == 0);
    bool sign_match = (is_negative == n.is_negative);
    bool size_match = (n.size == expected_size);

    if (output_match && sign_match && size_match) {
        PRINT_PASS();
    } else {
        printf(ANSI_RED);
        if (!output_match) {
            printf("Error: Output mismatch.\n");
            printf("Expected: \"%s\"\n", input);
            printf("Actual:   \"%s\"\n", buf);
        }
        if (!sign_match) {
            printf("Error: Sign mismatch\n");
            printf("Expected is_negative: %u\n", is_negative);
            printf("Actual is_negative:   %u\n", n.is_negative);
        }
        if (!size_match) {
            printf("Error: Size mismatch.\n");
            printf("Expected size: %zu\n", expected_size);
            printf("Actual size:   %zu\n", n.size);
        }
        if (BIGINT_GUARD(&n) != 0) {
            printf("Error: Guard bit overwritten\n");
        }
        printf(ANSI_RESET);
    }
    printf("------------------------------\n");
}

int main() {
    // case: Small integer
    test_case("Small integer", "12345", false, 2);

    // case: Large integer
    test_case("Large integer", "123456789012345678901234567890", false, 5);

    // case: Small integer with value "1"
    test_case("Small integer with value 1", "1", false, 2);

    // case: Zero initialization
    test_case("Zero initialization", "0", false, 2);

    // case: Max buffer size
    test_case("Max buffer size",
              "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234"
              "567890123456789012345678901234567890",
              false, 15);

    // case: Small integer
    test_case("Negative small integer", "-12345", true, 2);

    // case: Negative large number
    test_case("Negative large number", "-123456789012345678901234567890", true, 5);

    return 0;
}
