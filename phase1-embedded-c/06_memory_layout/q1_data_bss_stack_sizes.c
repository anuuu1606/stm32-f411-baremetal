// Q. Write a file with one initialized global, one uninitialized global,
// and a local variable in main. Compile it, run `size` on the binary,
// and note the .text/.data/.bss sizes.

int x = 5;   // initialized global -> .data
int y;       // uninitialized global -> .bss

int main() {
    int local = 10;   // local variable -> lives on the stack
    return local;
}

// output in terminal:
//    text	   data	    bss	    dec	    hex	filename
//  1233	    548	     12	   1793	    701	q1_data_bss_stack_sizes.out
