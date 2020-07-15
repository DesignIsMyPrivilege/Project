//
// Created by q7 on 2019/12/3.
//

#ifndef C__TEST_DOCUMENT_H
#define C__TEST_DOCUMENT_H

#endif //C__TEST_DOCUMENT_H

void doc_op()
{
    //char str1[10], str2[10], str3[20];
    string stg1, stg2, stg3;
    cout << "请输入你的名字："<<endl;
    //cin.getline(str1,10);//str1最多存入9个字符，第10个字符用来存储换行符，超出9个字符程序运行会出错
    //cin >> str1;
    getline(cin, stg1);//这种形式可以实现对字符串的操作
    //cin.ignore(1024,'\n');//从输入流中提取字符，提取的字符被忽略 (ignore)，不被使用，当忽略个数达到1024个或者遇到被抛弃的字符是'\n'时，则执行终止
    cout << "请输入你的年龄：" << endl;
    //cin >> str2;
    cin >> stg2;
    //cin.ignore(1024,'\n');
    cout << "请输入你的籍贯：" << endl;
    cin >> stg3;
    cout << endl;
    cout << sizeof(string) << endl;//只有C++才有string类型
    cout << "你的名字：" << stg1 << endl;
    cout << "你的年龄：" << stg2 << endl;
    cout << "你的籍贯：" << stg3 << endl;
}

void doc_save()
{
    char a1[100];
    fstream file1;
    file1.open("test.txt", ios::out | ios::trunc);
    cout << "向文件写入信息" << endl;
    cout << "写入的信息：" << endl;
    cin.getline(a1, 100,'\n');
    file1 << a1 << endl;
    cout << "写入信息2：" << endl;
    cin >> a1;
    file1 << a1 << endl;
    file1.close();

    cout << "从文件读取信息" << endl;
    file1.open("test.txt", ios::in);
    while(file1 >> a1)//判断是否还能读取文件数据
    {
        cout << a1 << endl;
    }

    file1.close();
}
