#ifndef ANSI_COLOR_MACROS_H

#define ANSI_RESET    "\033[0m"
#define ANSI_RED      "\033[31m"
#define ANSI_GREEN    "\033[32m"
#define ANSI_YELLOW   "\033[33m"
#define ANSI_BLUE     "\033[34m"
#define ANSI_MAGENTA  "\033[35m"
#define ANSI_CYAN     "\033[36m"
#define ANSI_WHITE    "\033[37m"

// Convenience macros for common patterns
#define PRINT_PASS()     printf("\n" ANSI_GREEN "pass" ANSI_RESET "\n")
#define PRINT_FAIL()     printf(ANSI_RED)
#define PRINT_END_FAIL() printf(ANSI_RESET "\n")


#define ANSI_COLOR_MACROS_H
#endif