// Q. Write two simple functions add(int,int) and sub(int,int) with matching signatures. 
// Declare a function pointer, point it at each in turn, and call through it.

#include <stdio.h>

int add(int a, int b) {
    return a+b;
}

int sub(int a, int b) {
    return a-b;
}

int compute(int (*fp)(int, int), int m, int n) {
    return fp(m, n);
}

int main() {
    int x = 3;
    int y = 4;

    printf("Sum: %d\n", compute(add, x, y));
    printf("Difference: %d", compute(sub, x, y));

    return 0;
}