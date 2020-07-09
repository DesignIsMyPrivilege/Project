//
// Created by q7 on 2020/6/20.
//

#ifndef C__TEST_TEST_H
#define C__TEST_TEST_H

/*void test(){
    enum days{one, two, three}day;
    day = one;
    switch(day){
        case one:
            cout << "one" << endl;
            break;
        case two:
            cout << "two" << endl;
            break;
        default:
            cout << "three" << endl;
            break;
    }
    int a,b;
    cin >> a >> b;
    cout << a+b << endl;
}*/
/*void test(){
    string str;
    getline(cin,str);
    for(int i = str.size()-1;i>=0;i--)
        cout << str[i];
}*/

/*
void example1()
{
    //可以在new后面直接赋值
    int *p = new int(3);
    //也可以单独赋值
    //*p = 3;
    cout <<"p: "<< *p << endl;
    delete p;
}

void example2()
{
    //当new一个数组时，同样用一个指针接住数组的首地址
    int *q = new int[3];
    for(int i=0; i<3; i++)
        q[i] = i;
    for(int i=0; i<3; i++)
        cout << q[i] << " ";
    cout << endl;
    delete [] q;
}

struct student1
{
    string name;
    int score;
};


void example3()
{
    //这里是用一个结构体指针接住结构体数组的首地址
    //对于结构体指针，个人认为目前这种赋值方法比较方便
    student1 *stlist = new student1[3]{{"abc", 90}, {"bac", 78}, {"ccd", 93}};
    for(int i=0; i<3; i++)
        cout << stlist[i].name << " " << stlist[i].score << endl;
    delete [] stlist;
}



void test()
{
    example1();

    example2();

    example3();
}*/


#endif //C__TEST_TEST_H