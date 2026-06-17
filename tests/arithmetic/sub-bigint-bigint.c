#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define BIG_INT_IMPLEMENTATION
#include "../../BigInt.h"
#include "../ANSI-color-macros.h"

void test_sub(const char *test_name, char *_n1, char *_n2,
              const char *expected_result, bool is_negative) {
  BigInt n1 = bigint_alloc();
  BigInt n2 = bigint_alloc();
  BigInt res = bigint_alloc();
  char buf[1024] = "";

  bigint_set(&n1, _n1);
  bigint_set(&n2, _n2);
  printf("%s: %s - %s\n", test_name, _n1, _n2);

  bigint_sub(&res, &n1, &n2);

  printf("Memory dump: ");
  bigint_mem_dump(res);

  bigint_to_dec_str(res, buf, 255);
  printf("Result: %s\n", buf);

  if (strcmp(expected_result, buf) == 0 && is_negative == res.is_negative) {
    printf_green("pass");
  } else {
    if (strcmp(expected_result, buf) != 0) {
      printf_red("Error: Output mismatch.");
      printf_red("Expected: \"%s\"", expected_result);
      printf_red("Actual:   \"%s\"", buf);
    }
    if (is_negative != res.is_negative) {
      printf_red("Error: Sign mismatch");
      printf_red("Expected is_negative: %u", is_negative);
      printf_red("Actual is_negative:   %u", res.is_negative);
    }
  }

  printf("------------------------------\n\n");
  bigint_free(&n1);
  bigint_free(&n2);
  // bigint_free(&res);
}

int main() {
  test_sub("Sub small number from larger number", "123", "23", "100", false);
  test_sub("Sub small number from larger number", "4294967296", "96",
           "4294967200", false);
  test_sub("Sub small number from extremly larger number",
           "429496729642949672964294967294294964294967296729664294967296"
           "429496729642949672964294967294294964294967296729664294967296"
           "429496729642949672964294967294294964294967296729664294967296",
           "96",
           "429496729642949672964294967294294964294967296729664294967296"
           "429496729642949672964294967294294964294967296729664294967296"
           "429496729642949672964294967294294964294967296729664294967200",
           false);
  test_sub("cascading borrow", "18446744073709551616", "8888888888888888888",
           "9557855184820662728", false);

  test_sub(
      "Extreme large - extreme large subtraction with cascading borrow",
      "100000000000000000000000000000000000000000000000000000000000000000000",
      "99999999999999999999999999999999999988888888888888888888888888888888",
      "11111111111111111111111111111112", false);

  //
  // test_add("Sub -ve from +ve number", "123", "-23", "100", false);
  //
  // test_add("Sub +ve from -ve number", "-123", "23", "100", false);
  //
  // test_add("Sub -ve from -ve number", "-123", "-23", "100", true);

  return 0;
}
