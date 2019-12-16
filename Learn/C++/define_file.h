//
// Created by q7 on 2019/12/9.
//

#ifndef C__TEST_DEFINE_FILE_H
#define C__TEST_DEFINE_FILE_H

void define_test()
{
    cout << "Value of __LINE__:" << __LINE__ <<endl;//这会在程序编译时包含当前行号。
    cout << "Value of __FILE__ : " << __FILE__ << endl;//这会在程序编译时包含当前文件名
    cout << "Value of __DATE__ : " << __DATE__ << endl;//这会包含一个形式为 month/day/year 的字符串，它表示把源文件转换为目标代码的日期。
    cout << "Value of __TIME__ : " << __TIME__ << endl;//这会包含一个形式为 hour:minute:second 的字符串，它表示程序被编译的时间
#if 0
    cout << "这是注释部分" << endl;
    #符号:#字符串化的意思,出现在宏定义中的#是把跟在后面的参数转换成一个字符串.
    ##符号:连接符号,把参数连在一起.
#endif
}

#endif //C__TEST_DEFINE_FILE_H