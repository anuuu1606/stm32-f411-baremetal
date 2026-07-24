// Q. Declare a variable as const int x = 5;. Try to modify it directly (x = 10;) and observe the compiler error.
// Then take its address with a non-const int *p = &x; and modify it through the pointer —
// observe what actually happens (undefined behavior) vs. what the compiler warns about.

#include <stdio.h>

int main() {
    const int x = 5;

    printf("Before: x = %d\n", x);

    // uncommenting the next line will result in error as const can not be directly changed.
    // x = 10;

    // the compiler warns about the next lines (discarding the 'const' qualifier),
    // but it will still compile and succeed
    int *p = &x;
    *p = 10;

    printf("After:  x = %d\n", x);

    return 0;
}