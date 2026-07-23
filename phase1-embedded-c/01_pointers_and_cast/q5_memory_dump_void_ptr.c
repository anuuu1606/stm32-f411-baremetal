// Write a function that takes void * and a size, and prints out the raw bytes at that address (a mini "memory dump" — this is genuinely useful later for debugging).


#include <stdio.h>
#include <stdint.h>


void memory_dump(void *addr, size_t size) {
   uint8_t *ptr = (uint8_t *)addr;
   for (size_t i=0; i < size; i++) {
       printf("data in %p: %d\n", (void*)(ptr+i), *(ptr+i));
   }
}


int main() {
   uint8_t arr[6]={1, 2, 3, 4, 5, 6};


   memory_dump(arr, sizeof(arr));


   return 0;
}
