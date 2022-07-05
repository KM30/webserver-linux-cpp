#include <stdio.h>

int main()
{
    int a = 10;
#ifdef DEBUG
    printf("我是一个程序猿, 我不会爬树...\n");
#endif
    for(int i=0; i<3; ++i)
    {
        printf("hello, GCC!!!\n");
    }

    int b, c, d, f;
    b = 10;
    c = b;
    d = c;
    f = d;

    /*
        int b,c,d,f;
        b = 10;
        c = 10;
        d = 10;
        f = 10;
    */


    return 0;
}
