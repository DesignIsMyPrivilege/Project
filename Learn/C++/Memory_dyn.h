//
// Created by q7 on 2019/12/7.
//

#ifndef C__TEST_MEMORY_DYN_H
#define C__TEST_MEMORY_DYN_H

#endif //C__TEST_MEMORY_DYN_H

//C++ 程序中的内存分为两个部分：
//栈：在函数内部声明的所有变量都将占用栈内存。（编译时自动创建，为一级缓存）
//堆：这是程序中未使用的内存，在程序运行时可用于动态分配内存。（通过new创建，为二级缓存）
//用new函数创建动态内存，返回的是所分配的空间地址，即返回的是地址

//new和C中malloc相比，主要优点是new不止分配了内存，还创建了对象,返回的是该对象的指针;malloc 只是机械的分配一块内存


//如果ptr代表一个用new申请的内存返回的内存空间地址，即所谓的指针，那么：
//
//delete ptr -- 代表用来释放内存，且只用来释放ptr指向的内存。
//delete[] rg -- 用来释放rg指向的内存，！！还逐一调用数组中每个对象的 destructor！！

void memory_test()
{
    int *prt = NULL;
    //new运算符为任意的数据类型动态分配内存
    if(!(prt = new int))//检查new运算符是否返回NULL指针，即自由存储区是否用完
    {
        cout << "Out of the memory." << endl;
        exit(1);
    }
    *prt = 112;//普通的new创建的动态内存，返回的是内存地址，需要使用指针的赋值方式
    cout << *prt << endl;
    delete prt;//delete运算符，删除之前由new运算符分配的内存，释放动态内存空间

}
//数组的动态内存分配，new返回的相当于内存的对象，可以直接赋值，不像指针的赋值方式
void memory_test_array1()
{
    char *prt= NULL;//初始化为null指针

    if (!(prt = new char [10]))
    {
        cout << "Out of the memory" << endl;
        exit(1);
    }
    for (int i = 0; i < 10; i++)
    {
        prt[i] = 'a';
    }

    for (int j = 0; j < 10; j++)
    {
        cout << prt[j] << endl;
    }

    delete [] prt;
}
void memory_test_array2()
{
    int **prt = NULL;

    prt = new int *[3];
    for(int i = 0; i < 3; i++)
    {
        prt[i] = new int [2];
    }
    for(int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            prt[i][j] = i + j;
        }
    }

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            cout << prt[i][j] << endl;
        }
    }
    for (int i = 0;i < 3; i++)
    {
        delete [] prt[i];
    }
    delete [] prt;
}