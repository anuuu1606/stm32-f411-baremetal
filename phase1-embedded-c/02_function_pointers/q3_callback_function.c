// Q. Write a function void run_callback(void (*cb)(int), int value) that takes a callback and an int, and calls the callback with that value. 
// Pass in a couple different callback functions.

#include <stdio.h>

void square(int a) {
    printf("%d\n", a*a);
}

void plus_ten(int a) {
    printf("%d\n", a+10);
}

void double_num(int a) {
    printf("%d\n", 2*a);
}

void run_callback(void (*cb)(int), int value) {
    cb(value);
}

int main(){
    int x = 5;
    run_callback(square, x);
    run_callback(double_num, x);
    run_callback(plus_ten, x);

    return 0;
}