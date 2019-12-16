//
// Created by q7 on 2019/11/28.
//

#ifndef C__TEST_STRING_FUN_H
#define C__TEST_STRING_FUN_H

void string_length0()
{
    cout << setfill('*') << setw( 20) << "length" << setfill('*') << setw( 15) << "*"<< endl;
    unsigned int len1, len2;
    //    char t1[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
//    char t2[] = {"Hello"};
//    len1 = sizeof(t1);
//    len2 = sizeof(t2);
//
//    cout << len1 <<endl;
//    cout << len2 << endl;
//
//    int flag;
//    flag = strcmp(t1, t2);
//    cout << flag <<endl;

    string str1 = "Hello";
    string str2 = "Test";
    char str3[5] = {'0','1', '2','3'};

    int i1 = 0;
    i1 = strlen(str3);
    cout << i1 << endl;
    cout << str1 + str2 << endl;
    cout << str1.size() << endl;
}

void string_length1()
{
    cout << setfill('*') << setw( 20) << "number" << setfill('*') << setw( 15) << "*"<< endl;
    char *ss = "12345678";
    cout << sizeof(ss) << endl;//ss 是指向字符串常量的字符指针，sizeof 获得的是一个指针的之所占的空间,应该是长整型的，所以是 8。
    cout << sizeof(*ss) << endl;//*ss 是第一个字符 其实就是获得了字符串的第一位 '1' 所占的内存空间，是 char 类型的，占了 1 位
}

#endif //C__TEST_STRING_FUN_H