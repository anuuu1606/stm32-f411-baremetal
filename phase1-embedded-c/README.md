# Phase 1 — C Fundamentals for Embedded

Practice exercises for embedded-relevant C concepts.

## Subfolders
- `01_pointers_and_cast` — pointers, casting, pointer arithmetic
- `02_function_pointers` — function pointers, callbacks, dispatch tables, function pointers in structs
- `03_structs_unions_bitfields` — register-map structs, unions (endianness), bitfields
- `04_bitwise_ops` — set/clear/toggle/mask patterns, bit counting, bit diffing
- `05_type_qualifiers` — volatile (compiler optimization behavior), const, const+volatile combined
- `06_memory_layout` — .text/.data/.bss, static linkage, initialized vs uninitialized globals

## Build
Each folder has standalone `.c` files. Compile with:
```
gcc -Wall -o file.out file.c
./file.out
```

## Key takeaways
- **volatile**: without it, the compiler can (and will, at -O2) optimize away repeated memory reads, collapsing a polling loop into a bare infinite jump. Confirmed this directly via objdump:  a real, visible difference in generated assembly.
- **const + volatile together**: the standard pattern for real hardware status registers. Volatile because hardware can change the value independently, const because the program should never write to it.
- **Bitfields vs manual mask/shift**: bitfields are cleaner to read but layout isn't guaranteed by the standard across compilers. Manual mask/shift is more lengthy but very predictable, which matters for real register access.
- **.bss vs .data**: .bss reserves RAM and zeros it at load time; costs no space in the file itself, even for huge arrays. .data must store real values in the binary and copy them at startup. An explicit `= 0` initializer still lands in .bss, not .data. The compiler optimizes based on the actual value, not just presence of a `=`.
- **Endianness**: verified directly via a union that a 32-bit value's bytes are stored in a specific, observable order in memory. Most laptops and PCs are little endian, storing LSB first.
