#include <stdio.h>

#include "./tests.h"

typedef struct TestEntry TestEntry;
struct TestEntry {
    int (*function)(void);
    const char* name;
};

#define TEST_DEFINE(func) int func(void);
#define TEST_ENTRY(func) {func, #func},

TEST_LIST(TEST_DEFINE)
#define TESTS { \
    TEST_LIST(TEST_ENTRY) \
}

TestEntry tests[] = TESTS;
long testCount =  sizeof(tests) / sizeof(tests[0]);

int main()
{
    int failCount = 0;
    printf("\n");
    printf("Running tests...\n");
    for(int i = 0; i < testCount; i++) {
        int result = tests[i].function();
        const char* passfail = result ? "FAILED" : "passed";
        failCount += result ? 1 : 0;
        printf("%d/%ld %s -> %s\n", i + 1, testCount, tests[i].name, passfail);
    }
    printf("\n");
    if(failCount > 0)
        fprintf(stderr, "%d/%ld TESTS FAILED!\n", failCount, testCount);
    else
        printf("All tests passed! ^_^\n");
    printf("\n");
    return 0;
}
