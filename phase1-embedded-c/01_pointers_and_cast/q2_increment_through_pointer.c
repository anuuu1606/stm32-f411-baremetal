// Q. Write a function void increment(int *p) that increments the value it points to. Call it on a local variable and print before/after.


#include <stdio.h>


void increment (int *p) {
   (*p)++;
}


int main() {
   int num = 7;


   // before calling increment function
   printf("%d\n", num);


   increment(&num);


   // after calling increment function
   printf("%d\n", num);


   return 0;
}
