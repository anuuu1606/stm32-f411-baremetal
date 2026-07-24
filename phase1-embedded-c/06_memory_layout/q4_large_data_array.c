// Q. Add a large initialized array instead. Rerun size/check file size, compare against Q3:
// explain the difference between .bss and .data in terms of what's actually stored
// in the flash/binary image.

int x = 5;
int y;

static int s_global = 7;

void some_function(void) {
    static int s_local = 3;
    s_local++;
}

int big[10000] = {1};   // large initialized array -> .data instead of .bss

int main() {
    int local = 10;
    some_function();
    return local;
}

//    text	   data	    bss	    dec	    hex	filename
//    1308	  40596	     12	  41916	   a3bc	q4_large_data_array.out

// this time the actual file size gre by a large amount, instead of .bss like previous question
// since .data's contents  need to be stored in the binary as the loader has to copy real, 
// specific values into RAM at startup