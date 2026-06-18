#include <stdint.h>
#include <stdio.h>

#define BIG_INT_IMPLEMENTATION
#include "../../BigInt.h"

void run_test_case(const char *test_name, char *input_str, uint32_t divisor, char *exp_q, uint32_t exp_r) {
    BigInt n = bigint_alloc();
    BigInt q = bigint_alloc();
    uint32_t r;

    // Set BigInt with the given input string
    bigint_set(&n, input_str);
    printf("%s - After set: ", test_name);
    bigint_mem_dump(n);
    printf("size: %zu\n", n.size);

    // Perform division
    naive_divide(&n, divisor, &q, &r);
    char buf[255] = "";
    bigint_to_dec_str(q, buf, 255);
    printf("%s / %d \n", input_str, divisor);
    printf("quotient: %s\n", buf);
    printf("remainder: %d\n", r);

    bool is_q_correct = strcmp(exp_q, buf) == 0;
    if (is_q_correct && r == exp_r) {
        printf("\033[32mpass \033[0m\n");
    } else if (is_q_correct && r != exp_r) {
        printf("\033[31mfail - wrong remainder \033[0m\n");
    } else if (!is_q_correct && r == exp_r) {
        printf("\033[31mfail - wrong quotient \033[0m\n");
    } else {
        printf("\033[31mfail - wrong quotient and remainder \033[0m\n");
    }
    printf("------------------------------\n");
    bigint_free(&n);
    bigint_free(&q);
}

int main() {
    // Test case 1: Simple Division by 10
    run_test_case("Test 1", "12345678900", 10, "1234567890", 0);

    // Test case 2: Large number with division by 10
    run_test_case("Test 2", "123456789012345678901234567890", 10, "12345678901234567890123456789", 0);

    // Test case 3: Non-zero remainder (division by 7)
    run_test_case("Test 3", "123456789012345678901234567890", 7, "17636684144620811271604938270", 0);

    // Test case 4: Division by 1 (should return the same number)
    run_test_case("Test 4", "123456789012345678901234567890", 1, "123456789012345678901234567890", 0);

    // Test case 5: Division by 2 (even number, should have no remainder)
    run_test_case("Test 5", "123456789012345678901234567890", 2, "61728394506172839450617283945", 0);

    // Test case 6: Division of a small number with non-zero remainder
    run_test_case("Test 6", "12345", 7, "1763", 4);

    run_test_case("Divide from extremly larger number",
           "429496729642949672964294967294294964294967296729664294967296"
           "429496729642949672964294967294294964294967296729664294967296"
           "429496729642949672964294967294294964294967296729664294967296",
           10,
           "429496729642949672964294967294294964294967296729664294967296"
           "429496729642949672964294967294294964294967296729664294967296"
           "42949672964294967296429496729429496429496729672966429496729",
           6);

    return 0;
}
