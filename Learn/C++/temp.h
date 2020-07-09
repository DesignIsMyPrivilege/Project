//
// Created by q7 on 2019/12/9.
//

#ifndef C__TEST_TEMP_H
#define C__TEST_TEMP_H

//模板是一种对类型进行参数化的工具；通常有两种形式：函数模板和类模板；
//函数模板针对仅参数类型不同的函数；
//类模板针对仅数据成员和成员函数类型不同的类。
//
//使用模板的目的就是能够让程序员编写与类型无关的代码。
//比如编写了一个交换两个整型int 类型的swap函数，这个函数就只能实现int 型，对double，字符这些类型无法实现，要实现这些类型的交换就要重新编写另一个swap函数。
//使用模板的目的就是要让这程序的实现与类型无关，比如一个swap模板函数，即可以实现int 型，又可以实现double型的交换。
//https://www.runoob.com/w3cnote/c-templates-detail.html
//模板的声明或定义只能在全局、命名空间或类范围内进行，不能在局部范围、函数内进行，比如不能在main函数中声明或定义一个模板
template <class T>//这里的class关键字和typename可以互换，两者没有区别，都是表明T是一个类型
inline T Max(T a, T b)
{
    return a > b ? a : b;
}

void temp_test()
{
    int i = 50, j = 20;
    cout << "Max(i,j):" << Max(i,j) << endl;
    float a = 10.0, b = 60.1;
    cout << "Max(a,b):" << Max(a,b) << endl;
}
//类模板
template <class T1> class A{
    public:
        T1 fun1(T1 a, T1 b);
        A();
    };
template <class T1> A<T1>::A() {
    cout << "调用构造函数" << endl;
}
template <class T1> T1 A<T1>::fun1(T1 a, T1 b) {
    return a + b;
}

void temp_class(int a, int b)
{
    A<int> a1;
    cout << a1.fun1(a ,b) << endl;
}
//含有非类型形参的类模板
//非类型形参在模板定义的内部是常量值，也就是说非类型形参在模板的内部是常量。
//非类型模板的形参只能是整型，指针和引用
//注意：任何局部对象，局部变量，局部对象的地址，局部变量的地址都不是一个常量表达式，都不能用作非类型模板形参的实参。
//全局指针类型，全局变量，全局对象也不是一个常量表达式，不能用作非类型模板形参的实参。
//
//全局变量的地址或引用，全局对象的地址或引用const类型变量是常量表达式，可以用作非类型模板形参的实参。
template <class T2, int *a> class A1{//int *a为非类型形参
public:
        void display();
    };

template <class T2, int *a> void A1<T2, a>::display() {
    cout << a[0] <<endl;
}
int a[1] ={10};//全局变量
void temp_class_2()
{
    A1<int, a> a2;//传递数组的第一个地址
    a2.display();
}
#endif //C__TEST_TEMP_H
