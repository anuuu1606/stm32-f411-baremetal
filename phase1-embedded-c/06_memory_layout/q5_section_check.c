// Q. Write one program containing a large initialized global array, a large uninitialized
// global array, one global set to 0, and one global set to a nonzero value.
// Use size and nm to determine which section each one lands in.

int global_a = 5;              // initialized, nonzero -> .data
int global_b = 0;               // initialized to zero -> .bss
int global_c;                    // uninitialized -> .bss
int large_init[1000] = {1};      // large, initialized -> .data
int large_uninit[1000];          // large, uninitialized -> .bss

int main() {
    return 0;
}

//    text	   data	    bss	    dec	    hex	filename
//    1228	   4592	   4064	   9884	   269c	q5_section_check.out

// 0000000000004020 D global_a -> data
// 0000000000005000 B global_b -> .bss
// 0000000000005004 B global_c -> .bss
// 0000000000004040 D large_init -> .data
// 0000000000005020 B large_uninit -> .bss
