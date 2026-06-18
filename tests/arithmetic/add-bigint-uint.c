#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BIG_INT_IMPLEMENTATION
#include "../../BigInt.h"

void test_add(const char *test_name, char *initial_value, uint32_t operand, const char *expected_result, bool expected_sign) {
    BigInt n = bigint_alloc();
    char buf[255] = "";

    bigint_set(&n, initial_value);
    printf("%s - Initial value: %s, Operand: %u\n", test_name, initial_value, operand);

    // Perform naive_add
    naive_add(&n, operand);

    printf("Memory dump: ");
    bigint_mem_dump(n);

    bigint_to_dec_str(n, buf, 255);
    printf("After: %s\n", buf);

    if (strcmp(expected_result, buf) == 0 && expected_sign == n.is_negative) {
        printf("\033[32mpass\033[0m\n");
    } else {
        printf("\033[31m");
        if (strcmp(expected_result, buf) != 0) {
            printf("Error: Output mismatch.\n");
            printf("Expected: \"%s\"\n", expected_result);
            printf("Actual:   \"%s\"\n", buf);
        }
        if (expected_sign != n.is_negative) {
            printf("Error: Sign mismatch\n");
            printf("Expected is_negative: %u\n", expected_sign);
            printf("Actual is_negative:   %u\n", n.is_negative);
        }
        printf("\033[0m\n");
    }

    printf("------------------------------\n\n");
    bigint_free(&n);
}

int main() {
    // Basic addition
    test_add("Add to small number", "123", 10, "133", false);

    // Addition resulting in carry
    test_add("Add causing carry", "4294967295", 1, "4294967296", false);

    // Add zero to non-zero
    test_add("Add zero", "987654321", 0, "987654321", false);

    // Add to large number
    test_add("Add to large number", "123456789012345678901234567890", 99, "123456789012345678901234567989", false);

    // Add to zero
    test_add("Add to zero", "0", 123, "123", false);

    // Add to negative number
    test_add("Add to negative number", "-500", 100, "-400", true);

    return 0;
}
