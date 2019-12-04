//
// Created by q7 on 2019/12/3.
//

#ifndef C__TEST_DOCUMENT_H
#define C__TEST_DOCUMENT_H

#endif //C__TEST_DOCUMENT_H

void doc()
{
    /*char   str1[30],str2[30],str3[30];
    cout   <<   "请输入你的姓名：";
    cin>>str1;
    cout<<"请输入你的住址：";
    cin.ignore();
    cin.getline(str2,30,'a');
    cout   <<   "请输入你的籍贯：";
    cin.ignore();
    cin.getline(str3,30);
    cout<<str3;*/
    fstream iofile;
    iofile.open("t.txt", ios::out | ios::in | ios::trunc);
    string bookname;
    string bookwriter;
    cout << "input the bookname:" << endl;
    getline(cin, bookname);
    iofile << bookname << endl;
    cout << "input the bookwriter:" << endl;
    getline(cin, bookwriter);
    iofile << bookwriter << endl;
    iofile.close();
    cout << "read the input file:" << endl;
    iofile.open("t.txt");
    while (getline(iofile, bookname))
    {
        cout << bookname << endl;
    }
}