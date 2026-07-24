// Q. Add a static global and a static local (inside a function)
// rerun size and nm, and note where each symbol shows 
// up (nm -S or nm with objdump -t) and what section it's in.

int x = 5;          // initialized global -> .data
int y;               // uninitialized global -> .bss

static int s_global = 7;   // static global initialized

void some_function(void) {
    static int s_local = 3;   // static local
    s_local++;
}

int main() {
    int local = 10;   // ordinary local, on the stack
    some_function();
    return local;
}

//    text	   data	    bss	    dec	    hex	filename
//    1308	    556	     12	   1876	    754	q2_static_symbols_nm.out

// 00000000000010e0 t __do_global_dtors_aux
// 0000000000003df8 d __do_global_dtors_aux_fini_array_entry
// 0000000000004014 d s_global
// 0000000000004018 d s_local.0
// 0000000000004010 D x
// 0000000000004020 B y
