#include <stdio.h>

/*//C命令行参数
//argv[0] 存储程序的名称，argv[1] 是一个指向第一个命令行参数的指针
//没有提供任何参数，argc 将为 1，否则，如果传递了一个参数，argc 将被设置为 2
int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        printf("The argument supplied is %s\n", argv[1]);
    }
    else if(argc > 2)
    {
        printf("There are too many arguments.\n");
    } else
    {
        printf("There are only one argument:%s\n", argv[0]);
    }

    return 0;
}*/

/*//C内存管理
//动态分配内存
#include <stdlib.h>
#include <string.h>

int main()
{
    char name[100];
    char *description;

    //scanf("%c", name);

    //动态分配内存
    description = (char *) malloc(200 * sizeof(char));

    if (description == NULL)
    {
        fprintf(stderr, "Error!\n");
    }
    else
    {
        strcpy(description, "This is a test.");
    }

    printf("Name=%s\n", name);
    printf("Description:%s\n", description);
    printf("The length of the Description:%lu\n", strlen(description));
    printf("The size of the Description:%lu\n", sizeof(description));// sizeof 计算的是变量的大小，而 strlen 计算的是字符串的长度，前者不受字符 \0 影响，后者以 \0 作为长度判定依据
    free(description);//使用free（）函数释放内存

    return 0;
}*/

/*//C可变参数
#include <stdarg.h>
//va:variable-argument(可变参数) 的意思
double average(int num, ...)
{
    va_list vaList;//va_list: 保存宏va_start、va_arg和va_end所需信息的一种类型
    int i;
    double sum = 0.0;

    va_start(vaList, num);//为num个参数初始化vaList

    for(i = 0 ;i < num; i++)//访问所有赋给vaList的参数
    {
        sum += va_arg(vaList, int);//va_arg:展开成一个表达式的宏，该表达式具有变长参数列表中下一个参数的值和类型
    }
    va_end(vaList);//清理为vaList保留的内存

    return sum / num;
}

int main()
{
    double result;

    result = average(5, 1,1,1,8,7);
    printf("The average is:%f\n", result);
}*/

/*//C递归
double result(int a)
{
    if (a <= 1)
    {
        return 1;
    }

    return a * result(a -1);
}//计算一个数的阶乘

int main()
{
    int a;
    double num;

    printf("Please input the number:\n");
    scanf("%d", &a);
    num = result(a);
    printf("The result of the %d is %f\n", a, num);

    return 0;
}*/

/*//C错误处理
#include <errno.h>
#include <string.h>

int main()
{
    FILE *fp;

    int errornum;
    fp = fopen("noexist.txt", "r");

    if(fp == NULL)
    {
        errornum = errno;//在发生错误时，大多数的 C 或 UNIX 函数调用返回 1 或 NULL，同时会设置一个错误代码 errno，
        // 该错误代码是全局变量，表示在函数调用期间发生了错误。您可以在 errno.h 头文件中找到各种各样的错误代码。
        fprintf(stderr,"The errno number is: %d\n", errornum);//使用stderr文件流来输出所有的错误
        perror("The error out by perror");//perror显示传递给它的字符串，后跟一个冒号、一个空格和当前errno值的文本表现形式
        fprintf(stderr, "The error is (perror):%s\n", strerror(errornum));//strerror函数，返回一个指针，指针指向当前errno值的文本表现形式
    }

    return 0;
}*/

/*//C预处理器
//字符串常量化运算符（#），标记粘贴运算符（##）
#define message_for(a) printf("num" #a ":%d\n", num##a)
//类似于：printf("num1:%d\n", num1);
int main(void)
{
    int num1 = 100;

    message_for(1);

    return 0;
}*/
/*
//defined()运算符
#if !defined(MESSAGE)
#define MESSAGE "test"
#endif
//判断MESSAGE是否被定义了
int main()
{
    printf("The massage is %s!\n", MESSAGE);

    return 0;
}
*/
/*//参数化的宏:使用参数化的宏来模拟函数
#define MAX(a,b) ((a)>(b)?a:b)//宏名称和左圆括号之间不允许有空格

int main()
{
    printf("The max of the number:%d\n", MAX(5,6));

    return 0;
}*/

/*
//文件读写
int main()
{
    FILE *fp = NULL;//声明文件指针

    fp = fopen("text.txt","w+");//以写入模式打开文件（允许读写文件）
    fprintf(fp, "This is a test file! For fprintf \n");
    fputs("This is a test file! For fputs \n", fp);

    fseek(fp, 10, SEEK_SET);
    if(fputs("This is insert test.", fp) == EOF)
    {
        printf("fputs Error!\n");
    }
    fclose(fp);//关闭文件

    return 0;
}
*/

/*
//C共用体：节约内存
//共用体是一种特殊的数据类型，允许您在相同的内存位置存储不同的数据类型。您可以定义一个带有多成员的共用体，但是任何时候只能有一个成员带有值。
#include <string.h>

union NUM
{
    int a;
    float b;
    char c[20];
};

int main()
{
    union NUM num;

    printf("Memory size of the union.a:%lu\n", sizeof(num.a));//共用体占用的内存应足够存储共用体中最大的成员
    printf("Memory size of the union.b:%lu\n", sizeof(num.b));//共用体占用的内存大小应该为所有共用体成员大小的最小公约数
    printf("Memory size of the union.c:%lu\n", sizeof(num.c));
    printf("Memory size of the union:%lu\n", sizeof(num));

    return 0;
}*/

/*//位域
struct bc
{
    unsigned a:1;//为什么使用int a:1 后面printf输出的时候是-1
    unsigned b:2;
    int  :1;
    int c:8;
};

int main()
{
    struct bc num_1;
    struct bc *num_2;

    num_1.a = 1;
    num_1.b = 2;
    num_1.c = 100;

    printf("%d,%d,%d\n", num_1.a, num_1.b, num_1.c);

    num_2 = &num_1;
    num_2->a = 0;
    num_2->b|=1;
    num_2->c&=15;

    printf("%d,%d,%d\n", num_2->a, num_2->b, num_2->c);

    return 0;
}*/

//结构体(数组只能存储相同类型的变量，结构体可以存储不同类型的变量)
//结构体中成员变量分配的空间是按照成员变量中占用空间最大的来作为分配单位,同样成员变量的存储空间也是不能跨分配单位的,如果当前的空间不足,则会存储到下一个分配单位中。
//结构体的总大小为结构体最大数据类型成员大小的整数倍
/*struct
{
    unsigned int a;
    unsigned char b;
    char c;
}store_1;

typedef struct SAMPLE
{
    unsigned char a;
    short b;
    int c;
}store_2;

struct
{
    short b;
    int a;
}store_3;

int main()
{
    store_2 a2[2];
    printf("The size:%lu\n", sizeof(a2));
    printf("The size:%lu\n", sizeof(store_3));
    a2[0].c = 0;
    a2[1].c = 1;
    a2[2].c = 7;
    a2[3].c = 5;
    printf("The vaule of a2[0].c=%d\n", a2[0].c);
    printf("The vaule of a2[1].c=%d\n", a2[1].c);
    printf("The vaule of a2[2].c=%d\n", a2[2].c);

    printf("The vaule of a2[3].c=%d\n", a2[3].c);
    //printf("the length of the store_1:%lu\n", sizeof(store_1));
    //printf("the length of the store_1.a:%lu\n", sizeof(store_1.a));
    //printf("the length of the store_1.b:%lu\n", sizeof(store_1.b));
    printf("the address of the store_1.b:%p\n", &store_1.b);
    printf("the length of the store_1.c:%lu\n", sizeof(store_1.c));
    printf("the address of the store_1.c:%p\n", &store_1.c);

    printf("the length of the store_2:%lu\n", sizeof(store_2));
    //printf("the length of the store_2.a:%lu\n", sizeof(store_2.a));
    //printf("the address of the store_2.a:%p\n", &store_2.a);
    //printf("the length of the store_2.b:%lu\n", sizeof(store_2.b));
    //printf("the address of the store_2.b:%p\n", &store_2.b);

//    printf("the length of the store_2:%lu\n", sizeof(a2[0]));
//    printf("the length of the store_2.a:%lu\n", sizeof(a2[2].a));
    //printf("the address of the store_2.a:%p\n", &store_2.a);
//    printf("the length of the store_2.b:%lu\n", sizeof(a2[2].b));
    //printf("the address of the store_2.b:%p\n", &store_2.b);

    return 0;
}*/

/*字符串
#include <string.h>
//在 C 语言中，字符串实际上是使用 null 字符 '\0' 终止的一维字符数组。因此，一个以 null 结尾的字符串，包含了组成字符串的字符。
int main()
{
    char a[6] = {'1','2','3','4','5','\0'};
    int len = sizeof(a);
    char *hi = "hello";//字符指针

    printf("Message: %s\n", a);
    printf("The length of the a:%d\n", len);

    printf("The address of the hi:%p\n", &hi[2]);
    printf("The value of the hi:%s\n", hi);

    return 0;
}*/

/*
//函数指针
int max(int a, int b)
{
    return a > b ? a : b;
}

int main()
{
    int (*ip)(int m, int n) = max;

    int a, b, c, d;

    printf("Input three numbers:");
    scanf("%d %d %d", &a, &b, &c);

    d = ip(ip(a,b), c);
    printf("The biggest number is %d\n", d);

    return 0;
}*/

/*//回调函数
#include <stdlib.h>
//size_t 是一种数据类型，近似于无符号整型，但容量范围一般大于 int 和 unsigned。这里使用 size_t 是为了保证 arraysize 变量能够有足够大的容量来储存可能大的数组。
void populate_array(int *array, size_t arraysize, int(*getNextValue)(void))
{
    for(size_t i = 0; i < arraysize; i++)
    {
        array[i] = getNextValue();
    }
}

int getNextRandomValue(void)
{
    return rand();
}

int main()
{
    int array[10];

    populate_array(array, 10, getNextRandomValue);

    for(int i = 0; i < 10; i++)
    {
        printf("The number of the array[%d]:%d\n", i, array[i]);
    }

    return 0;
}*/

/*//C从函数返回指针，C 不支持在调用函数时返回局部变量的地址，除非定义局部变量为 static 变量。
#include <time.h>
#include <stdlib.h>

//要生成和返回随机数的函数
int * getRandom( )
{
    static int r[10];
    int i;

    //设置种子
    srand( (unsigned)time( NULL ) );
    for ( i = 0; i < 10; ++i)
    {
        r[i] = rand();
        printf("%d\n", r[i] );
    }

    return r;
}
//要调用上面定义函数的主函数

 int main ()
{
    //一个指向整数的指针
    int *p = NULL;
    int i;

    p = getRandom();
    for ( i = 0; i < 10; i++ )
    {
        printf("*(p + [%d]) : %d\n", i, *(p + i) );
    }

    return 0;
}*/

/*//传递指针给函数
#include <time.h>

void getSeconds(unsigned long *ip);

void main()
{
    unsigned long sec;

    getSeconds(&sec);

    printf("Number of the second:%ld\n", sec);

}

void getSeconds(unsigned long *ip)
{
    *ip = time(NULL);
}*/

/*指向指针的指针
 iip指向ip的地址，即*iip的值为ip的地址
int main()
{
    int var = 2000;
    int *ip;
    int **iip;

    ip = &var;
    iip = &ip;

    printf("the value of the var:%d\n", var);
    printf("the value of the ip:%d\n", *ip);
    printf("the address of the ip:%p\n", ip);
    printf("the value of the *iip:%p\n", *iip);
    printf("the value of the **iip:%d\n", **iip);
    printf("the address of the iip:%p\n", iip);

    return 0;
}*/

/*//指针数组
int main()
{
    const int Max = 3;
    int i;
    char *ip[Max];
    char num[] = {'8', '7', '6'};

    for(i = 0; i < Max; i++)
    {
        ip[i] = &num[i];
        printf("the address of the num[%d]:%p\n", i, ip[i]);
        printf("the address of the ip:%p\n", ip);
        printf("the value of the ip:%c\n", *ip[i]);
    }

    return 0;
}*/

//注意区别数组指针与指针数组
 //指针数组：表示的是一个由指针变量组成的数组，也就是说其中的元素都是指针变量
 //数组指针：表示的是这是个指向数组的指针，那么该指针变量存储的地址就必须是数组的首地址
/*#include <stdio.h>

const int MAX = 4;

int main ()
{
    const char *names[] = {
            "Zara Ali",
            "Hina Ali",
            "Nuha Ali",
            "Sara Ali",
    };
    int i = 0;

    for ( i = 0; i < MAX; i++)
    {
        printf("Value of names[%d] = %s\n", i, names[i] );
        printf("the address of the names[%d] = %p\n", i, names[i]);//指针数组的地址问题：names的地址和names[i]地址的区别
        printf("the address of the names = %p\n", names);
    }
    return 0;
}*/

int main()
{
    int num = 5;
    int number[] = {1,2,3,4,5};
    int *p;

    p = number;
    for(int i = 0; i < num; i++)
    {
        printf("The value of the p: %d\n", *(p + i));
    }

}