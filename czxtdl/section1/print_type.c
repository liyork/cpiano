#include <stdio.h>
 
int main(void) {
    int a_int = -1;
    short a_short = -2;
    long a_long = -3;
    long long a_long_long = -4;
 
    unsigned int a_uint = 1;
    unsigned short a_ushort = 2;
    unsigned long int a_ulong = 3;
    unsigned long long int a_ulong_long = 4;
 
    char a_char = 'c';
 
    float a_float = 1.0;
    double a_double = 2.0;
    long double a_ldouble = 3.0;
 
    printf("size of int: %ld, value is: %d\n",
                    sizeof(int), a_int);
    printf("size of short: %ld, value is: %hd\n",
                    sizeof(short), a_short);
    printf("size of long: %ld, value is: %ld\n",
                    sizeof(long), a_long);
    printf("size of long long: %ld, value is: %lld\n",
                    sizeof(long long), a_long_long);
 
    printf("size of unsigned int: %ld, value is: %u\n",
                    sizeof(unsigned int), a_uint);
    printf("size of unsigned short: %ld, value is: %hu\n",
                    sizeof(unsigned short), a_ushort);
    printf("size of unsigned long: %ld, value is: %lu\n",
                    sizeof(unsigned long), a_ulong);
    printf("size of unsigned long long: %ld, value is: %llu\n",
                    sizeof(unsigned long long), a_ulong_long);
 
    printf("size of char: %ld, value is: %c\n",
                    sizeof(char), a_char);
 
    printf("size of float: %ld, value is: %f\n",
                    sizeof(float), a_float);
    printf("size of double: %ld, value is: %lf\n",
                    sizeof(double), a_double);
    printf("size of long double: %ld, value is: %Lf\n",
                    sizeof(long double), a_ldouble);
    
    return 0;
}

//C语言是一种强类型语言，也就是说，C语言的变量使用要严格符合定义，所有变量都必须先定义后使用
//有符号整型
//有符号整型可以用来定义正整数和负整数，有下面几种类型：
//int：最常使用的类型，是系统的基本类型，C语言标准规定，int类型至少16位长。printf中使用%d进行打印。
//short或short int：C语言规定，short的长度不应大于最大的int长度，也就是说，short至少16位长，printf中使用%hd打印。
//long或long int：长度不应小于int的最大长度，C语言要求至少有32位长。printf打印使用%ld.
//long long或long long int：长度不应小于最大的long长度，至少为64位长。printf使用%lld进行打印。
//
//无符号整型
//无符号整型可以用来定义0和正整数。使用unsigned关键字修饰上面的有符号整型即可，如unsigned int代表无符号int类型。无符号类型的长度与有符号类型的长度相同，但是由于无符号类型仅用来表示0和正整数，所以无符号整型可以表示更大的正值范围。printf打印只需要将上面有符号整型的d换为u即可，如unsigned long可以使用%lu.
//字符
//字符使用char关键字定义，实际C语言将char类型当做一个字节的存储空间，可以使用unsigned修饰char，printf使用%c打印。
//C语言设计之初char主要为了支持ASCII字符集，为了支持宽字符集，C语言加入了wchar_t类型，代表一个宽字符，为此还提供了一套字符串处理函数和文件io函数，作为扩充，大家可以去了解
//布尔值
//_Bool用来表示布尔值，这里使用非0代表true，0代表false，如果希望更直观，可以使用stdbool.h中定义的bool，true以及false。使用%d打印。实际C语言开发更喜欢直接使用int来判断布尔值，非0代表true，0代表false。
//实浮点数
//float：基本浮点类型，至少精确表示六位有效数字，使用%f打印；
//double：更大范围的浮点数，至少精确表示10位有效数字，使用%lf打印；
//long double：更大范围的浮点数，比double有更多的有效数字位数，使用%Lf打印。
//
//复数和虚浮点数
//这里不再介绍，感兴趣可以去了解，C语言提供了复数和虚数类型的数据类型，科学计算中使用更多，平时开发一般不会使用。
