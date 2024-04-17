//
// Created by felix on 4/15/24.
//

#ifndef VIDEOSCALE_UNIT_TEST_H
#define VIDEOSCALE_UNIT_TEST_H

#include <stdio.h>
#include <string.h>

#define RED "\e[1;31m"
#define GREEN "\e[1;32m"
#define RESET "\e[1;0m"
#define UNIT_START(name) printf("starting unit \""name"\"\n")
#define TEST_START(name) printf("running \""name"\": ")
#define TEST_PASSED() printf(GREEN"passed\n"RESET)
#define LOG_RESULTS(unit, succ, total) printf("%sunit \"%s\" %i/%i tests succeeded.\n"RESET, succ == total ? GREEN : RED, unit, succ, total)
#define CHECK_FAILED(a) printf(RED"check \"%s\" on line number %i failed.\n"RESET, #a, __LINE__)
#define CHECK(a) do {if (!(a)) {CHECK_FAILED(a); return -1;}}while(0)

#define TEST(name, routine) \
int name(void) {            \
TEST_START(#name);\
routine                     \
return 0;}                  \

#define UNIT(name, ...) \
int main(void){         \
UNIT_START(#name);\
const char *case_names_str = #__VA_ARGS__; \
int(*cases[])(void) = { \
__VA_ARGS__             \
};                      \
int succ = 0;           \
int num_cases = sizeof(cases) / sizeof(cases[0]);\
for (int i = 0; i < num_cases; ++i){ \
int err = cases[i]();   \
if (!err){              \
    succ++;             \
    TEST_PASSED();\
}                       \
}                       \
LOG_RESULTS(#name, succ, num_cases);\
return 0;}              \

#endif //VIDEOSCALE_UNIT_TEST_H
