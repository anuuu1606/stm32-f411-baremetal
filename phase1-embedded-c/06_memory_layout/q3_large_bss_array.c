// Q. Add a large uninitialized array as a global. Rerun size, and explain why the
// binary file size on disk barely changed even though .bss grew a lot.

int x = 5;
int y;

static int s_global = 7;

void some_function(void) {
    static int s_local = 3;
    s_local++;
}

int big[10000];   // large uninitialized global array -> .bss

int main() {
    int local = 10;
    some_function();
    return local;
}

//   text	   data	    bss	    dec	    hex	filename
//   1308	    556	  40064	  41928	   a3c8	q3_large_bss_array.out

// .bss's size represents how much RAM will be reserved and zeroed at program startup
// but none of those 40,000 bytes are actually in the file itself, thus file size
// on disk has barely changed. It only reserves the bytes, not the actual zero bytes. 
