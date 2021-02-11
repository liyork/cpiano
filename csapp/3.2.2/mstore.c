

long mult2(long, long);

void multstore(long x, long y, long *dest) {
  long t = mult2(x, y);
  *dest = t;
}

// gcc -Og -S mstore.c
// gcc -Og -c mstore.c
// gdb mstore.o
// x/14xb multstore
// objdump -d mstore.o
