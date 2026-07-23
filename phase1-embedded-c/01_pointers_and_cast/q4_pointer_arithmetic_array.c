// Given an array of 4 uint32_t, use pointer arithmetic (not indexing with []) to print each element and its address, showing the address increments by 4 each time.


#include <stdio.h>
#include <stdint.h>


int main() {
   uint32_t arr[4]={2, 4, 6, 8};
   uint32_t *ptr=arr;


   // first element
   printf("%d\n", *ptr);
   printf("%p\n\n", (void*)ptr);


   // second element
   printf("%d\n", *(ptr+1));
   printf("%p\n\n", (void*)(ptr+1));


   // third element
   printf("%d\n", *(ptr+2));
   printf("%p\n\n", (void*)(ptr+2));


   // fourth element
   printf("%d\n", *(ptr+3));
   printf("%p\n\n", (void*)(ptr+3));


   return 0;
}
