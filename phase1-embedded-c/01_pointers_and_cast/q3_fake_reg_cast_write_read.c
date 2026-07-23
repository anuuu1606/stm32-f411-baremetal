// Q. Create a #define FAKE_REG_ADDR pointing at some byte array's address, cast it to volatile uint32_t *, and write/read a value through it.


#include <stdio.h>
#include <stdint.h>


uint8_t array[6];
uint8_t *ptr = array;


#define FAKE_REG_ADDR ptr


int main(){
   volatile uint32_t *reg = (volatile uint32_t*)FAKE_REG_ADDR;
   reg[0]=3;
   printf("%d", reg[0]);
   return 0;
}


