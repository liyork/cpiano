#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len){
  printf("len:%lu\n", len);
  size_t i;
  for (i = 0; i< len; i++)
    printf(" %.2x", start[i]);
  printf("\n");
}

void show_int(int x){
  show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x){
  show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x){
  show_bytes((byte_pointer) &x, sizeof(void *));
}

void show_char(char x){
  show_bytes((byte_pointer) &x, sizeof(char));
}

void test_show_bytes(int val){
  int ival = val;
  float fval = (float)val;
  int *pval = &ival;
  int cval = (char)&ival;
  show_int(ival);
  show_float(fval);
  show_pointer(pval);
  show_char(cval);

  show_bytes((byte_pointer) &"12345", 6);
  
  const char *s = "abcdef";
  show_bytes((byte_pointer) s, strlen(s));
}

void test_fixint(){
  int32_t a = 15;
  uint64_t b = 15;
  printf("x = %" PRId32 ", y = %" PRIu64 "\n", a, b);
}

void test_complement(){
  short x = 12345;
  short mx = -x;
  show_bytes((byte_pointer) &x, sizeof(short));
  show_bytes((byte_pointer) &mx, sizeof(short));
}

void test_convert(){
  short int v = -12345;
  unsigned short uv = (unsigned short) v;
  printf("v = %d, uv = %u\n", v, uv);

  unsigned u = 4294967295u;
  int tu = (int) u;
  printf("u = %u, tu = %d\n", u, tu);

  int x = -1;
  unsigned u2 = 2147483648;
  printf("x = %u = %d\n", x, x);
  printf("u2 = %u = %d\n", u2, u2);
}

void test_extension(){
  short sx = -12345;
  printf("xs = %d:\t", sx);
  show_bytes((byte_pointer) &sx, sizeof(short));

  unsigned short usx = sx;
  printf("usx = %u:\t", usx);
  show_bytes((byte_pointer) &usx, sizeof(unsigned short));

  int x = sx;
  printf("x = %d:\t", x);
  show_bytes((byte_pointer) &x, sizeof(int));

  unsigned ux = usx;
  printf("ux = %d:\t", ux);
  show_bytes((byte_pointer) &ux, sizeof(unsigned));
}

int main(){
  //test_show_bytes(12345);
  //test_fixint();
  //test_complement();
  //test_convert();
  test_extension();
  return 0;
}

// gcc -o show-bytes.o show-bytes.c 
