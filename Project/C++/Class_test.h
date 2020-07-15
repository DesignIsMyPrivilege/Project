//
// Created by q7 on 2019/11/27.
//

#ifndef C__TEST_CLASS_TEST_H
#define C__TEST_CLASS_TEST_H

/*****************类成员函数************************/
class Box
{
    public:
        int height;
        int length;
        int width;
        int  getVolume(void)
        {
            return height * length * width;
        }
        int getArea(void);//类成员函数声明
};

inline int Box::getArea(void)
{
    return (height*length*2) + (height*width*2) + (length*width*2);
}//类成员函数

void classTest1() {
    cout << setfill('*') << setw( 20) << "class" << setfill('*') << setw( 15) << "*"<< endl;
    Box Box1, Box2;
    Box1.height = 5;
    Box1.width = 6;
    Box1.length = 8;
    cout << Box1.getVolume() << endl;

    Box2.height = 6;
    Box2.width = 8;
    Box2.length = 10;
    cout << Box2.getArea() << endl;
}

class student
{
public:
    char name[10];
    char sex[10];
    float chinese;
    float math;
    float english;
    void input(class student *stu);
    void input_name();
    void input_sex();
    void input_chinese();
    void input_math();
    void input_english();
    void show_message(class student *stu);
};

void student::input_name()
{
    cout << "请输入姓名："<< endl;
    cin.getline(name, sizeof(name));
}
inline void student::input_sex() {
    cout << "请输入性别：" << endl;
    cin.getline(sex, sizeof(sex));
}
inline void student::input_chinese() {
    cout << "请输入语文成绩：" << endl;
    cin >> chinese;
}
inline void student::input_math() {
    cout << "请出入数学成绩：" << endl;
    cin >> math;
}
inline void student::input_english() {
    cout << "请输入英语成绩：" << endl;
    cin >> english;
}
inline void student::input(class student *stu) {
    stu->input_name();
    stu->input_sex();
    stu->input_chinese();
    stu->input_math();
    stu->input_english();
}

inline void student::show_message(class student *stu) {
    cout << "学生的姓名：" << stu->name << endl;
    cout << "学生的性别：" << stu->sex << endl;
    cout << "学生的语文成绩：" << stu->chinese << endl;
    cout << "学生的数学成绩:" << stu->math << endl;
    cout << "学生的英语成绩:" << stu->english << endl;
}

void classTest2()
{
    student xiaoming;
    student *xiaoming_p = &xiaoming;
    xiaoming.input(xiaoming_p);
    xiaoming.show_message(xiaoming_p);
}
/**************************类访问修饰符*********************************/
//public:公有成员在程序中类的外部是可访问的。您可以不使用任何成员函数来设置和获取公有变量的值
//private:私有成员变量或函数在类的外部是不可访问的，甚至是不可查看的。只有类和友元函数可以访问私有成员,默认情况下，类的所有成员都是私有的
//protected:保护成员变量或函数与私有成员十分相似，但有一点不同，保护成员在派生类（即子类）中是可访问的。
class Big_0
{
public:
    //Big();
    int length_smallbig;
    int width_smallbig;
    int height_smallbig;
    void set_width();
    void set_length();
    void set_height();
    void set();
    int display();//类成员函数可以访问private类

private:
    int width;
    int length;
    int height;
};

inline void Big_0::set_width(){
    width = 10;
}
inline void Big_0::set_height(){
    height = 10;
}
inline void Big_0::set_length() {
    length = 10;
}
void Big_0::set(){
    set_width();
    set_height();
    set_length();
}
int Big_0::display(){
    int num;
    num = height*width*length;
    return num;
}

class Smallbig_0:public Big_0{
    int Vol;//默认为private类
public:
    int set_vol();
    Smallbig_0();//构造函数声明
};//pubilc继承

Smallbig_0::Smallbig_0() {
    height_smallbig = 8;
    width_smallbig = 8;
    length_smallbig = 8;
}//构造函数

int Smallbig_0::set_vol(){
    Vol = height_smallbig*width_smallbig*length_smallbig;
    //继承的height_smallbig等参数是Big_0的随机初始化的参数，Big_0对象化后的big1对height_smallbig等参数进行了赋值，但赋值后的height_smallbig等值属于big1对象
    //因而，Smallbig_0继承得到的height_smallbig等参数仍然是Big_0的随机初始化的参数。
    return Vol;
}

void Attributes0(){

    Big_0 big1;
    big1.set();
    Smallbig_0 big_1;
    cout << big_1.set_vol() << endl;
    cout << big1.display() << endl;
}
/************************构造函数&析构函数*******************************/
//类的构造函数是类的一种特殊的成员函数，它会在每次创建类的新对象时执行。
//构造函数的名称与类的名称是完全相同的，并且不会返回任何类型，也不会返回 void。构造函数可用于为某些成员变量设置初始值。
//
//类的析构函数是类的一种特殊的成员函数，它会在每次删除所创建的对象时执行。
//析构函数的名称与类的名称是完全相同的，只是在前面加了个波浪号（~）作为前缀，它不会返回任何值，也不能带有任何参数。析构函数有助于在跳出程序（比如关闭文件、释放内存等）前释放资源。
/**************************拷贝构造函数*********************************/
//拷贝构造函数通常用于：1.通过使用另一个同类型的对象来初始化新创建的对象(一个对象需要通过另外一个对象进行初始化)。
// 2.复制对象把它作为参数传递给函数(一个对象以值传递的方式传入函数体 )。
// 3.复制对象，并从函数返回这个对象(一个对象以值传递的方式从函数返回)。
//当定义一个新对象并用一个同类型的对象对它进行初始化时，将显示使用拷贝构造函数(方法1)。当该类型的对象传递给函数或从函数返回该类型的对象时，将隐式调用拷贝构造函数。
//
//必须定义拷贝构造函数的情况：
//只包含类类型成员或内置类型（但不是指针类型）成员的类，无须显式地定义拷贝构造函数也可以拷贝；
//有的类有一个数据成员是指针，或者是有成员表示在构造函数中分配的其他资源，这两种情况下都必须定义拷贝构造函数。
class Line
{
public:
    int getLength( void );
    void display(void);
    Line( int len );             // 简单的构造函数
    Line( const Line &obj);      // 拷贝构造函数
    ~Line();                     // 析构函数

private:
    int *ptr;
};

// 成员函数定义，包括构造函数
Line::Line(int len)
{
    cout << "调用构造函数" << endl;
    // 为指针分配内存
    ptr = new int;
    *ptr = len;
}//构造函数
Line::Line(const Line &obj)
{
    cout << "调用拷贝构造函数并为指针 ptr 分配内存" << endl;
    ptr = new int;
    *ptr = *obj.ptr; // 拷贝值
}//拷贝构造函数
Line::~Line(void)
{
    cout << "释放内存" << endl;
    delete ptr;
}//析构函数

int Line::getLength( void )
{
    return *ptr;
}
void Line::display(void)
{
    cout << "Line 大小：" << *ptr << endl;
}

void display(Line obj)
{
    cout << "line 大小 : " << obj.getLength() <<endl;
}//外部函数

void copy_fun( )
{
    Line line1(10);
    //line.display();
    Line line2 = line1;//方法1.通过使用另一个同类型的对象来初始化新创建的对象

    display(line1);//方法2.复制对象把它作为参数传递给函数
    //display(line2);
}
/**************************友元函数*********************************/
//类的友元函数是定义在类外部，但有权访问类的所有私有（private）成员和保护（protected）成员。
/**************************内联函数*********************************/
//内联函数inline：引入内联函数的目的是为了解决程序中函数调用的效率问题，这么说吧，
// 程序在编译器编译的时候，编译器将程序中出现的内联函数的调用表达式用内联函数的函数体进行替换，而对于其他的函数，都是在运行时候才被替代。
// 这其实就是个空间代价换时间的i节省。所以内联函数一般都是1-5行的小函数。
// 在使用内联函数时要留神：
//1.在内联函数内不允许使用循环语句和开关语句；
//2.内联函数的定义必须出现在内联函数第一次调用之前；
//结论: 一个较为合理的经验准则是, 不要内联超过 10 行的函数. 谨慎对待析构函数, 析构函数往往比其表面看起来要更长, 因为有隐含的成员和基类析构函数被调用!
/*************************C++中的this指针****************************/
//每一个对象都能通过 this 指针来访问自己的地址。this 指针是所有成员函数的隐含参数。因此，在成员函数内部，它可以用来指向调用对象。
//友元函数没有 this 指针，因为友元不是类的成员。只有成员函数才有 this 指针。
class Big_1
{
public:
    //Big();
    void set_width();
    void set_length();
    void set_height();
    void set();
    int display();

private:
    int width;
    int length;
    int height;
};

inline void Big_1::set_width()
{
    this->width = 8;
}
inline void Big_1::set_height()
{
    this->height = 8;
}
inline void Big_1::set_length() {
    this->length = 8;
}
void Big_1::set(){
    this->set_length();
    this->set_height();
    this->set_width();
}
int Big_1::display(){
    int static num;
    num = this->height*this->width*this->length;
    return num;
}

void Attributes1()
{

    Big_1 big1;
    big1.set();
    cout << big1.display() << endl;
}
/************************C++中指向类的指针****************************/
//指向类的指针方式如同指向结构的指针。实际上，类可以看成是一个带有函数的结构。
/************************C++类的静态成员******************************/
//当我们声明类的成员为静态时，这意味着无论创建多少个类的对象，静态成员都只有一个副本。
//
//静态成员在类的所有对象中是共享的。如果不存在其他的初始化语句，在创建第一个对象时，所有的静态数据都会被初始化为零。
//我们不能把静态成员的初始化放置在类的定义中，但是可以在类的外部通过使用范围解析运算符 :: 来重新声明静态变量从而对它进行初始化。
//
//如果把函数成员声明为静态的，就可以把函数与类的任何特定对象独立开来。
//静态成员函数即使在类对象不存在的情况下也能被调用，静态函数只要使用类名加范围解析运算符 :: 就可以访问。
//静态成员函数只能访问静态成员数据、其他静态成员函数和类外部的其他函数。
//静态成员函数有一个类范围，他们不能访问类的 this 指针。您可以使用静态成员函数来判断类的某些对象是否已被创建。

/**************************继承**************************************/
//派生类可以访问基类中所有的非私有成员。因此基类成员如果不想被派生类的成员函数访问，则应在基类中声明为 private。
//一个派生类继承了所有的基类方法，但下列情况除外：
//基类的构造函数、析构函数和拷贝构造函数。
//基类的重载运算符。
//基类的友元函数。
//C++ 类可以从多个类继承成员，语法如下：
//
// class <派生类名>:<继承方式1><基类名1>,<继承方式2><基类名2>,…   {
//<派生类类体>
//};
//虚拟继承:
//为了解决从不同途径继承来的同名的数据成员在内存中有不同的拷贝造成数据不一致问题，将共同基类设置为虚基类。
//这时从不同的路径继承过来的同名数据成员在内存中就只有一个拷贝，同一个函数名也只有一个映射。这样不仅就解决了二义性问题，也节省了内存，避免了数据不一致的问题。
/************************重载运算符和重载函数****************************/
//C++ 允许在同一作用域中的某个函数和运算符指定多个定义，分别称为函数重载和运算符重载。
//重载声明是指一个与之前已经在该作用域内声明过的函数或方法具有相同名称的声明，但是它们的参数列表和定义（实现）不相同
//当您调用一个重载函数或重载运算符时，编译器通过把您所使用的参数类型与定义中的参数类型进行比较，决定选用最合适的定义。
//选择最合适的重载函数或重载运算符的过程，称为重载决策。
//
//一元运算符通常出现在它们所操作的对象的左边。
//递增运算符（ ++ ）和递减运算符（ -- ）是 C++ 语言中两个重要的一元运算符。
//
//二元元算符需要两个参数，使用的加运算符（ + ）、减运算符（ - ）、乘运算符（ * ）和除运算符（ / ）都属于二元运算符。
class Plus
{
private:
    int width;
    int length;
    int heigth;
public:
    Plus();//默认构造函数声明
    Plus(int a, int b, int c);//带参数的构造函数声明
    int get_V();//类内函数声明
    Plus operator+(const int a);//重载+运算符声明
    Plus operator+(const Plus & obj);//重载+运算符声明
    friend Plus operator+(const int a, Plus obj);//友元函数重载+运算符声明
};

Plus::Plus() {
    width = 1;
    length = 1;
    heigth = 1;
}//默认构造函数定义
Plus::Plus(int a, int b, int c) {
    width = a;
    length = b;
    heigth = c;
}//带参数的构造函数定义
int Plus::get_V(){
    return width * length * heigth;
}//类内函数定义
Plus Plus::operator+(const int d) {
    return Plus(width + d, this->length, this->heigth);
}//重载+运算符定义（用于对象与数相加）
Plus Plus::operator+(const Plus & obj) {
    Plus flag;
    flag.width = this->width + obj.width;
    return flag;
}//重载+运算符定义（用于对象相加）
Plus operator+(const int a, Plus obj)
{
    return Plus(obj.width+a, obj.length, obj.heigth);
}//友元函数重载+运算符定义（用于使得加法的数字加对象的运算顺序识别）

void plus_1()
{
    Plus m1(2,2,2);
    Plus m2, m3, m4, m5;
    int m = 1;

    cout << "m1:" << m1.get_V() << endl;
    cout << "m2:" << m2.get_V() << endl;
    m3 = m1 + m2;
    cout << "m3:" << m3.get_V() << endl;
    m4 = m1 + m;//当 2 个对象相加时是没有顺序要求的，但要重载 + 让其与一个数字相加则有顺序要求
    cout << "m4:" << m4.get_V() << endl;
    m5 = m + m1;//对应友元函数的功能
    cout << "m5:" << m5.get_V() << endl;
}
//C++ 语言支持各种关系运算符（ < 、 > 、 <= 、 >= 、 == 等等），它们可用于比较 C++ 内置的数据类型。
//您可以重载任何一个关系运算符，重载后的关系运算符可用于比较类的对象。
//
//习惯上人们是使用 cin>> 和 cout<< 的，得使用友元函数来重载运算符，如果使用成员函数来重载会出现 d1<<cout; 这种不自然的代码。
//
//函数调用运算符 () 可以被重载用于类的对象。当重载 () 时，您不是创造了一种新的调用函数的方式，相反地，这是创建一个可以传递任意数目参数的运算符函数。
//
//
/*****************************C++多态********************************/
//多态按字面的意思就是多种形态。当类之间存在层次结构，并且类之间是通过继承关联时，就会用到多态
//C++ 多态意味着调用成员函数时，会根据调用函数的对象的类型来执行不同的函数
//虚函数 是在基类中使用关键字 virtual 声明的函数。在派生类中重新定义基类中定义的虚函数时，会告诉编译器不要静态链接到该函数。
//我们想要的是在程序中任意点可以根据所调用的对象类型来选择调用的函数，这种操作被称为动态链接，或后期绑定。
//
//形成多态必须具备三个条件：
//1、必须存在继承关系；
//2、继承关系必须有同名虚函数（其中虚函数是在基类中使用关键字Virtual声明的函数，在派生类中重新定义基类中定义的虚函数时，会告诉编译器不要静态链接到该函数）；
//3、存在基类类型的指针或者引用，通过该指针或引用调用虚函数；（即利用父对象指针来访问子对象，然后根据子对象的函数来调用）
//
//父类的虚函数或纯虚函数在子类中依然是虚函数。有时我们并不希望父类的某个函数在子类中被重写，在 C++11 及以后可以用关键字 final 来避免该函数再次被重写。
//如果不希望一个类被继承，也可以使用 final 关键字。
//class Class_name final
//{
//    ...
//};//该类将不能被继承
class People
{
private:
    int num;
public:
    virtual void display()=0;//纯虚函数
};//包含纯虚函数的类是抽象类（通常称为ABC），抽象类不能被用于实例化对象，它只能作为接口使用
//因此，如果一个 ABC 的子类需要被实例化，则必须实现每个虚函数，这也意味着 C++ 支持使用 ABC 声明接口。
//如果没有在派生类中重写纯虚函数，就尝试实例化该类的对象，会导致编译错误。    可用于实例化对象的类被称为具体类
//
//virtual void display();对应下文解释
//在一个类中如果存在未定义的虚函数，那么不能直接使用该类的实例，可以理解因为未定义 virtual 函数，其类是抽象的，无法实例化。

class member1: public People
{
public:
    int m1;
    void display()
    {
        cout << "Member1 class." << endl;
    }
};
class member2: public People
{
public:
    int m2;
    void display()
    {
        cout << "Member2 class." << endl;
    }
};

void M_status()
{
    People *pt;//即利用父对象指针来访问子对象，然后根据子对象的函数来调用
    member1 p1;
    member2 p2;

    pt = &p1;
    pt->display();

    pt = &p2;
    pt->display();

}
/******************************数据抽象*********************************/
//数据抽象是指，只向外界提供关键信息，并隐藏其后台的实现细节，即只表现必要的信息而不呈现细节。
/******************************数据封装*********************************/
//封装是面向对象编程中的把数据和操作数据的函数绑定在一起的一个概念，这样能避免受到外界的干扰和误用，从而确保了安全。
//数据封装引申出了另一个重要的 OOP 概念，即数据隐藏。

#endif //C__TEST_CLASS_TEST_H