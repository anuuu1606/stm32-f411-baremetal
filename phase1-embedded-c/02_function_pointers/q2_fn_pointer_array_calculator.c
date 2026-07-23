// Q. Build an array of function pointers (int (*ops[])(int,int)) indexed 0=add, 1=sub, 2=mul, 3=div. Write a small "calculator" that picks the op by index.

#include <stdio.h>

int add(int a, int b) {
    return a+b;
}

int sub(int a, int b) {
    return a-b;
}

int mul(int a, int b) {
    return a*b;
}

int div(int a, int b) {
    return a/b;
}

int (*fp[4])(int, int) = {add, sub, mul, div};

int compute(int index, int m, int n) {
    return fp[index](m, n);
}

int main() {
    int x = 6;
    int y = 3;

    printf("Sum: %d\n", compute(0, x, y));
    printf("Difference: %d\n", compute(1, x, y));
    printf("Product: %d\n", compute(2, x, y));
    printf("Quotient: %d\n", compute(3, x, y));

    return 0;
}