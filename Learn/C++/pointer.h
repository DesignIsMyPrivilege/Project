//
// Created by q7 on 2019/11/20.
//

#ifndef C__TEST_POINTER_H
#define C__TEST_POINTER_H

#endif //C__TEST_POINTER_H

/*****************************Pointer:NULL指针**********************************/
void Pointer()
{
    cout << setfill('*') << setw( 20) << "pointer" << setfill('*') << setw( 15) << "*"<< endl;
    int var = 20;
    int *p;
    int *prt = NULL;//赋为 NULL 值的指针被称为空指针，NULL 指针是一个定义在标准库中的值为零的常量;如果所有未使用的指针都被赋予空值，同时避免使用空指针，就可以防止误用一个未初始化的指针
    p = &var;
//    prt = &var;

    cout << "The value of the var:" << var << endl;
    cout << "The address of the var:" << &var << endl;
    cout << "The value of the p:" << *p << endl;
    cout << "The address of the p:" << p << endl;
//    cout << "The value of the prt:" << *prt << endl;
    cout << "The address of the prt:" << prt << endl;
}
/*****************************Array_pointer:指针数组**********************************/
void Array_pointer()
{
    cout << setfill('*') << setw( 20) << "Array pointer" << setfill('*') << setw( 15) << "*"<< endl;
    int nu[10] ={1,2,3,4,5,6,7,8,9,0};//nu指向整型数组的第一个数的地址，为常量，不可更改，即nu++（nu=nu+1）错误
    int *nup = nu;//nup为指针变量，可以更改，即nup++有效

    //nup = nu;
    for(int m = 0; m < 10; m++)
    {
        cout << "The value of the nup:" << *nup << endl;
        cout << "The address of the nup:" << nup << endl;
        nup++;
    }
}
/*****************************Function_pointer:函数指针**********************************/
void t1()
{
    cout << "Test1" << endl;
}
void t2()
{
    cout << "Test2" << endl;
}
void t3()
{
    cout << "Test3" << endl;
}
//函数存放在内存的代码区域内，它们同样有地址.如果我们有一个 int test(int a) 的函数，那么，它的地址就是函数的名字，这一点如同数组一样，数组的名字就是数组的起始地址。
void Fun_pointer()
{

    cout << setfill('*') << setw( 20) << "Array pointer" << setfill('*') << setw( 15) << "*"<< endl;

    typedef void (*pt)(void);
    pt b11[] = {t1,t2,t3};//函数指针数组，指向函数的指针数组
    b11[2]();
}
/*****************************Return_pointer:从函数返回指针**********************************/
int *pointer(int num)
{
    static int test[3];

    srand((unsigned)time(NULL));
    for(int j = 0; j < num; j++)
    {
        test[j] = rand();
        cout << test[j] << endl;
    }

    //cout << test << endl;
    //cout << &test[0] << endl;
    return test;
}

void Return_pointer()
{
    cout << setfill('*') << setw( 20) << "Return pointer" << setfill('*') << setw( 15) << "*"<< endl;
    int *pointer_n;
    int num1 = 3;
    pointer_n = pointer(num1);

    //cout << pointer_n << endl;
    for(int i = 0; i < num1; i++)
    {
        cout << *(pointer_n + i) << endl;
    }
}