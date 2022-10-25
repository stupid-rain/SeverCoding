#include <stdio.h>
void byteorder()
{
    union
    {
        short value;
        char union_bytes[sizeof( short )]; // char类型数据占一个字节 一个字节=8位 0x是16进制 每2个16进制 = 2 * 2的4次 = 一个字节，也就是一个字节可以用2个16进制数字表示
        // short 占2个字节int long float 都占4个字节 double 占8个字节
        // short 占2个字节，就有高低位置之分，用short来表示一个整数 同时用0x来表示
    } test;
    test.value = 0x0102;
    printf( "sizeof(short):%d\n", (int)sizeof( short ) );
    if ( ( test.union_bytes[0] == 1 ) && ( test.union_bytes[1] == 2 ) )
    {
        printf( "big endian\n" );
    }
    else if ( ( test.union_bytes[0] == 2 ) && ( test.union_bytes[1] == 1 ) )
    {
        printf( "little endian\n" );
    }
    else
    {
        printf( "unknown...\n" );
    }
}

int main()
{
    byteorder();
    return 0;
}