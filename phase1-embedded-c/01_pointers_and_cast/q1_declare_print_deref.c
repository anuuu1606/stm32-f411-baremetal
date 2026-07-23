// Q. Declare an int, then a pointer to it. Print the variable's value, its address, and the value read back through the pointer.


#include <stdio.h>


int main() {
   int num = 5;
   int *ptr = &num;


   // print the value of num
   printf("%d\n", num);


   // print the address of num
   printf("%p\n", (void*)&num);
  
   // print the value read back through the pointer
   printf("%d\n", *ptr);


   return 0;
}


