//
// Created by q7 on 2019/12/5.
//

#ifndef C__TEST_ERROR_H
#define C__TEST_ERROR_H

//异常处理
//throw: 当问题出现时，程序会抛出一个异常。
//catch: 在您想要处理问题的地方，通过异常处理程序捕获异常
//try中是函数的主体
class MyExpection : public exception
{
public:
    const char * what() const throw()//const throw() 不是函数，这个东西叫异常规格说明，表示 what 函数可以抛出异常的类型
    {
        return "C++ Expection";
    }
};
//1. const char * 表示返回值类型
//2. what 是函数名称
//3. () 是参数列表
//4. const 表示该成员函数不能修改成员变量
//5. throw() 是异常规格说明符。括号内写该函数可抛出的异常类型
void error_test()
{
    try {
        throw MyExpection();
    }
    catch (MyExpection & a)
    {
        cout << "MyException caught" << endl;
        cout << a.what() << endl;
    }

}

#endif //C__TEST_ERROR_H