# C & C++

## Programing Trick

![](/Photo/无符号到有符号的转换.png)

逻辑与和逻辑或：P

```c++
#include<iostream>

int add(int a){
    int temp = 0;
    a && (temp = add(a-1));//(!a) || (temp = add(a-1))
    return temp+a;
}

int main(){
    sum = add(10);
    stdcout << sum << std::endl;
    
    return 0;
}
```

## C & C++相同点

```c++
#define   //define宏定义只是字符替换，编译时没有检查错误的功能
```

数组在使用时可以是一个含有变量的表达式，但是，在数组声明时必须用常量表达式。例如：

```c++
const int a=19;
long b[a];//合法

const int a=5;
long b[a+5];//合法

int a=19;
long b[a+5];//不合法

int a=19;
long b[(const int)a];//合法
```

## C & C++不同点

### NULL的区别

NULL表示指针不指向任何对象，C和C++中的NULL不等价

&：取址	*：取值

#### C中NULL为

```c
#define NULL (void*) 0 //但同时也允许将NULL定义为整常数0
```

#### C++中NULL为

```c++
#define NULL 0
```

C++中nullptr是一个字面值常量，是C++__空指针类型__的关键字，类型为std::nullptr_t，__空指针常数__可以转换为任意类型的指针类型。在c++中`(void *)`不能转化为任意类型的指针，即` int *p=(void *)`是错误的，但`int *p=nullptr`是正确的，原因是由于函数重载：若c++中` (void *)`支持任意类型转换，函数重载时将出现问题下列代码中`fun(NULL)`将不能判断调用哪个函数

```c++
void fun(int i){
    cout<<"1";
}
void fun(char *p){
    cout<<"2";
}
int main(){
	fun(NULL);  //输出1，c++中NULL为整数0
	fun(nullptr);//输出2，nullptr 为空指针常量。是指针类型
}
```

## C++中的存储类

### static

存储在静态数据区的变量会在程序刚开始运行时就完成初始化，也是唯一的一次初始化。共有两种变量存储在静态存储区：全局变量和 static 变量，只不过和全局变量比起来，static 可以控制变量的可见范围，说到底 static 还是用来隐藏的。

**static 修饰类的成员变量**

- 1). 静态成员变量是先于类的对象而存在
- 2). 这个类的所有对象共用一个静态成员
- 3). 如果静态成员是公有的，那么可以直接通过类名调用
- 4). 静态成员数据在声明时候类外初始化

```c++
#include <iostream>

using namespace std;
class Data
{
public:
    Data(){}
    ~Data(){}
    void show()
    {
        cout<<this->data<<" "<<number<<endl;
    }

    static void showData()//先于类的对象而存在
    {
        //这方法调用的时候不包含this指针
        cout<<" "<<number<<endl;
    }

private:
    int data;
public:
    static int number; //静态数据在声明时候类外初始化
};
int Data::number=0;//静态成员初始化

int main()
{
    Data::showData();//通过类名直接调用


    Data::number = 100;//通过类名直接使用
    Data d;
    d.show();
    d.showData();//通过对象调用

    cout << "Hello World!" << endl;
    return 0;
}
```

### extern

```c++
#include <iostream>
 
int count ;
extern void write_extern();
 
int main()
{
   count = 5;
   write_extern();
}

#include <iostream>
 
extern int count;
 
void write_extern(void)
{
   std::cout << "Count is " << count << std::endl;
}
```

```bash
$ g++ main.cpp support.cpp -o write
$ ./write
Count is 5
```

### thread_local

使用 thread_local 说明符声明的变量仅可在它在其上创建的线程上访问。 变量在创建线程时创建，并在销毁线程时销毁。 每个线程都有其自己的变量副本。

## C++定义常量

- 使用 **#define** 预处理器（#define identifier value）
- 使用 **const** 关键字。（const type variable = value;）

这两种方式的类型和安全检查不同、编译器处理不同、存储方式不同、定义域不同、定义后能否取消、是否可以做函数参数

![](/Photo/const_char.png)

## **volatileP**

修饰符 **volatile** 告诉编译器不需要优化volatile声明的变量，让程序可以直接从内存中读取变量。对于一般的变量编译器会对变量进行优化，将内存中的变量值放在寄存器中以加快读写效率。

volatile 关键字是一种类型修饰符，用它声明的类型变量表示可以被某些编译器未知的因素更改，比如：操作系统、硬件或者其它线程等。遇到这个关键字声明的变量，编译器对访问该变量的代码就不再进行优化，从而可以提供对特殊地址的稳定访问。

线程并发访问共享变量时，一个线程改变了变量的值，怎样让改变后的值对其它线程 visible。一般说来，volatile用在如下的几个地方：

1. 中断服务程序中修改的供其它程序检测的变量需要加 volatile；

2. 多任务环境下各任务间共享的标志应该加 volatile；

3. 存储器映射的硬件寄存器通常也要加 volatile 说明，因为每次对它的读写都可能由不同意义；

## 调用系统头文件

### [实现mkdir](https://baike.baidu.com/item/MKDIR#2)

**mkdir()**函数以**mode**方式创建一个以参数**pathname**命名的目录，**mode**定义新创建目录的权限。

#### 头文件库

```c
#include<sys/stat.h>//头文件
#include<sys/types.h>
```

#### 声明

```c
int mkdir(const char *pathname, mode_t mode)
```

#### 参数

* **pathname** -- 指向文件名（char类型）的指针

* **mode** -- 赋予新建目录的权限

  |       命令        | 权限  |                    权限                    |
  | :---------------: | :---: | :----------------------------------------: |
  |      S_IRWXU      | 00700 | 代表该文件所有者拥有读，写和执行操作的权限 |
  | S_IRUSR(S_IREAD)  | 00400 |       代表该文件所有者拥有可读的权限       |
  | S_IWUSR(S_IWRITE) | 00200 |       代表该文件所有者拥有可写的权限       |
  | S_IXUSR(S_IEXEC)  | 00100 |       代表该文件所有者拥有执行的权限       |
  |      S_IRWXG      | 00070 | 代表该文件用户组拥有读，写和执行操作的权限 |
  |      S_IRGRP      | 00040 |       代表该文件用户组拥有可读的权限       |
  |      S_IWGRP      | 00020 |       代表该文件用户组拥有可写的权限       |
  |      S_IXGRP      | 00010 |       代表该文件用户组拥有执行的权限       |
  |      S_IRWXO      | 00007 |   代表其他用户拥有读，写和执行操作的权限   |
  |      S_IROTH      | 00004 |         代表其他用户拥有可读的权限         |
  |      S_IWOTH      | 00002 |         代表其他用户拥有可写的权限         |
  |      S_IXOTH      | 00001 |         代表其他用户拥有执行的权限         |

  mode方式：可多个权限相或，如0755表示：**S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH**

####  返回值

若目录创建成功，则返回0；否则返回-1，并将错误记录到全局变量errno中。

# C++

## C++中的智能指针

只要将 new 运算符返回的指针 p 交给一个 `shared_ptr` 对象“托管”，就不必担心在哪里写`delete p`语句——实际上根本不需要编写这条语句，托管 p 的 `shared_ptr` 对象在消亡时会自动执行`delete p`。而且，该 `shared_ptr` 对象能像指针 p —样使用，即假设托管 p 的 `shared_ptr` 对象叫作 ptr，那么 *ptr 就是 p 指向的对象。

通过 `shared_ptr` 的构造函数，可以让 `shared_ptr` 对象托管一个 new 运算符返回的指针，写法如下：

```c++
 shared_ptr<T> ptr(new T);// T 可以是 int、char、类等各种类型
```

此后，ptr 就可以像 T* 类型的指针一样使用，即 *ptr 就是用 new 动态分配的那个对象；多个 `shared_ptr` 对象可以共同托管一个指针 p，当所有曾经托管 p 的 `shared_ptr` 对象都解除了对其的托管时，就会执行`delete p`。

只有指向**动态分配**的对象的指针才能交给 `shared_ptr` 对象托管。将指向**普通局部变量、全局变量**的指针交给 `shared_ptr` 托管，编译时不会有问题，但程序运行时会出错，因为不能析构一个并没有指向动态分配的内存空间的指针。

```C++
class A{
public:
    int i;
    A(int n);
    ~A() { cout << i << " " << "destructed" << endl; }
};
A::A(int n){
    i = n;
}
//main函数
    shared_ptr<A> sp1(new A(2)); //A(2)由sp1托管，
    shared_ptr<A> sp2(sp1);      //A(2)同时交由sp2托管
    shared_ptr<A> sp3;
    sp3 = sp2;                   //A(2)同时交由sp3托管
    cout << sp1->i << "," << sp2->i <<"," << sp3->i << endl;
    A * p = sp3.get();   // get返回托管的指针，p 指向 A(2)
    cout << p->i << endl;//输出 2
    sp1.reset(new A(3));//reset导致托管新的指针, 此时sp1托管A(3)
    sp2.reset(new A(4)); // sp2托管A(4)
    cout << sp1->i << endl;//输出 3
    sp3.reset(new A(5)); //sp3托管A(5),A(2)无人托管，被delete
    cout << "end" << endl;
```

## C++中的Vector容器[^参考vector]

Vector[^简单认为]是一个封装了动态大小数组的顺序容器，能够存放各种类型的对象。

vector特性：**顺序序列、动态数组、能够感知内存分配器的**

```c++
#include<vector>
using namespace std;

vector<int>obj; //创建一个向量存储容器int
//vector<类型>标识符
//vector<类型>标识符(最大容量)
//vector<类型>标识符(最大容量，初始所有值)
//Vector< vector< int> >v; 二维向量，这里最外的<>要有空格。否则在比较旧的编译器下无法通过
obj.push_back();//在数组的最后添加一个数据
obj.pop_back();//去掉数组的最后一个数据
obj.clear();//清除容器中所有数据
obj.size();//当前使用数据的大小
```

[^参考vector]:https://www.runoob.com/w3cnote/cpp-vector-container-analysis.html
[^简单认为]:vector是一个能够存放任意类型的动态数组。

## C++ 库函数stringstream[^stringstream参考]

C++标准库中的`<sstream>`提供了比ANSI C的<stdio.h>更高级的一些功能，即单纯性、类型安全和可扩展性。

### 实例

1. 基本数据类型转换 `int->string`

```c++
#include<string>
#include<sstream>
#include<iostream>

int main(){
    std::stringstream stream;
    std::string result;
    
    int i = 1000;
    stream<<i;//将int输入流
    stream>>result;//从stream中取出插入的int值
    std::cout<<result<<std::endl;
    //print the string "1000"
    return 0;
}
```

2. 在进行多次转换[^原因]时，必须调用`stringstream`的成员函数`clear()`[^注clear]

```c++
#include<iostream>
#include<string>
#include<sstream>

int main(){
    int first, secend;
    std::string a1="123", a2="789";
    std::stringstream stream;
    
    stream<<a1;
    stream>>first;
    std::cout<<first<<std::endl;
    stream.clear();//在进行多次转换前，必须清除stream
    stream<<a3;
    stream>>second;
    std::cout<<second<<std:endl;
    
    return 0;
}
```

[^注clear]:`clear`方法并不是清空`stringstream`中的内容，而是清空该流的错误标记，也就是之前流占用的内存空间并没有释放！！！，需要使用`stringstream.str("")`来清空`stringstream`，重新给`stringstream`赋值，也就是释放了之前的内存空间。
[^原因]:多次转换中使用同一个stringstream对象，记住再每次转换前要使用clear()方法；在多次转换中重复使用同一个stringstream（而不是每次都创建一个新的对象）对象最大的好处在于效率。stringstream对象的构造和析构函数通常是非常耗费CPU时间的。
[^stringstream参考]:http://cppblog.com/Sandywin/archive/2007/07/13/27984.html

# C

## C标准库的I/O缓冲区[^I/O缓冲区参考]

用户程序调用C标准I/O库函数读写文件或设备，而这些库函数要通过系统调用把读写请求传给内核（以后我们会看到与I/O相关的系统调用），最终由内核驱动磁盘或设备完成I/O操作。C标准库为每个打开的文件分配一个I/O缓冲区以加速读写操作，通过文件的FILE结构体可以找到这个缓冲区，用户调用读写函数大多数时候都在I/O缓冲区中读写，只有少数时候需要把读写请求传给内核。以fgetc/fputc为例，当用户程序第一次调用fgetc读一个字节时，fgetc函数可能通过系统调用进入内核读1K字节到I/O缓冲区中，然后返回I/O缓冲区中的第一个字节给用户，把读写位置指向I/O缓冲区中的第二个字符，以后用户再调fgetc，就直接从I/O缓冲区中读取，而不需要进内核了，当用户把这1K字节都读完之后，再次调用fgetc时，fgetc函数会再次进入内核读1K字节到I/O缓冲区中。在这个场景中用户程序、C标准库和内核之间的关系就像CPU、Cache和内存之间的关系一样，C标准库之所以会从内核预读一些数据放在I/O缓冲区中，是希望用户程序随后要用到这些数据，C标准库的I/O缓冲区也在用户空间，直接从用户空间读取数据比进内核读数据要快得多。另一方面，用户程序调用fputc通常只是写到I/O缓冲区中，这样fputc函数可以很快地返回，如果I/O缓冲区写满了，fputc就通过系统调用把I/O缓冲区中的数据传给内核，内核最终把数据写回磁盘。有时候用户程序希望把I/O缓冲区中的数据立刻传给内核，让内核写回设备，这称为Flush操作，对应的库函数是fflush，fclose函数在关闭文件之前也会做Flush操作。

### I/O缓冲区分类

1. 全缓冲：如果缓冲区写满了就写回内核
   * 常规文件通常是全缓冲的

2. 行缓冲：如果用户程序写的数据中有换行符，就把这一行写回内核，或者如果缓冲区写满了就写回内核。
   * 标准输入输出对应终端设备时通常是行缓冲

3. 无缓冲：用户程序每次调用库函数做写操作都要通过系统调用写回内核。
   * 标准错误输出通常是无缓存的，这样产生的错误信息可以尽快输出到设备

[^I/O缓冲区参考]:https://www.cnblogs.com/lzq1126/p/5596840.html

## C库函数 - fseek[^参考fseek]

C 库函数 **int fseek(FILE \*stream, long int offset, int whence)** 设置流 **stream** 的文件位置为给定的偏移 **offset**，参数 offset 意味着从给定的 **whence** 位置查找的字节数。

### 声明

```c
int fseek(FILE *stream, long int offset, int whence)
```

### 参数

- **stream** -- 这是指向 FILE 对象的指针，该 FILE 对象标识了流。
- **offset** -- 这是相对 whence 的偏移量，以字节为单位。
- **whence** -- 这是表示开始添加偏移 offset 的位置。它一般指定为下列常量之一：

|   常量   |        描述        |
| :------: | :----------------: |
| SEEK_SET |     文件的开头     |
| SEEK_CUR | 文件指针的当前位置 |
| SEEK_END |     文件的末尾     |

### 返回值

成功返回零，否则返回非零值。

[^参考fseek]:https://www.runoob.com/cprogramming/c-function-fseek.html

## C库函数 - ftell[^参考ftell]

C 库函数 **long int ftell(FILE \*stream)** 返回给定流 stream 的当前文件位置。

### 声明

```C
long int ftell(FILE *stream)
```

### 参数

- **stream**  -- 这是指向 FILE 对象的指针，该 FILE 对象标识了流。

### 返回值

该函数返回位置标识符的当前值。如果发生错误，则返回 -1L，全局变量 errno 被设置为一个正值。

### 实例

```c
#include <stdio.h>

int main (){
   FILE *fp;
   int len;

   fp = fopen("file.txt", "r");
   if( fp == NULL ) {
      perror ("打开文件错误");
      return(-1);
   }
   fseek(fp, 0, SEEK_END);

   len = ftell(fp);
   fclose(fp);

   printf("file.txt 的总大小 = %d 字节\n", len);
   
   return(0);
}
```

假设我们有一个文本文件 **file.txt**，它的内容如下：

```markdown
This is runoob.com
```

让我们编译并运行上面的程序，如果文件内容如上所示，这将产生以下结果，否则会根据文件内容给出不同的结果：

```markdown
file.txt 的总大小 = 19 字节
```

[^参考ftell]:https://www.runoob.com/cprogramming/c-function-ftell.html

## C库函数 - fread[^fread参考]

C 库函数 **size_t fread(void \*ptr, size_t size, size_t nmemb, FILE \*stream)** 从给定流 **stream** 读取数据到 **ptr** 所指向的数组中。

### 声明

```c
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
```

### 参数

- **ptr**  -- 这是指向带有最小尺寸 *size\*nmemb* 字节的内存块的指针。
- **size** -- 这是要读取的每个元素的大小，以字节为单位。
- **nmemb**  -- 这是元素的个数，每个元素的大小为 size 字节。
- **stream** -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输入流。

### 返回值

成功读取的元素总数会以 size_t 对象返回，size_t 对象是一个整型数据类型。如果总数与 nmemb 参数不同，则可能发生了一个错误或者到达了文件末尾。

### 实例

```c
#include <stdio.h>
#include <string.h>
 
int main(){
   FILE *fp;
   char c[] = "This is runoob";
   char buffer[20];
   /* 打开文件用于读写 */
   fp = fopen("file.txt", "w+");
   /* 写入数据到文件 */
   fwrite(c, strlen(c) + 1, 1, fp);
   /* 查找文件的开头 */
   fseek(fp, 0, SEEK_SET);
   /* 读取并显示数据 */
   fread(buffer, strlen(c)+1, 1, fp);
   printf("%s\n", buffer);
   fclose(fp);
   return(0);
}
```

编译并运行上面的程序，将创建一个文件 **file.txt**，然后写入内容 *This is runoob*。接下来使用 **fseek()** 函数来重置写指针到文件的开头，读取并显示数据。

[^fread参考]:https://www.runoob.com/cprogramming/c-function-fread.html

## C函数库 - fwrite[^fwrite参考]

C 库函数 **size_t fwrite(const void \*ptr, size_t size, size_t nmemb, FILE \*stream)** 把 **ptr** 所指向的数组中的数据写入到给定流 **stream** 中。

### 声明

```c
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
```

### 参数

- **ptr**  -- 这是指向要被写入的元素数组的指针。
- **size**  -- 这是要被写入的每个元素的大小，以字节为单位。
- **nmemb**  -- 这是元素的个数，每个元素的大小为 size 字节。
- **stream**  -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输出流。

### 返回值

如果成功，该函数返回一个 size_t 对象，表示元素的总数，该对象是一个整型数据类型。如果该数字与 nmemb 参数不同，则会显示一个错误。

### 实例

```c
#include<stdio.h>

int main(){
    FILE *fpout;
    char str[]="This is a test.";
    
    fpout = fopen("test.txt", "w");
    fwrite(str, sizeof(str), 1, fpout);
    fclose(fpout);
    
    return 0;
}
```

[^fwrite参考]:https://www.runoob.com/cprogramming/c-function-fwrite.html

## C库函数 - [access](https://baike.baidu.com/item/access/5245577)

access to作为有权使用什么，即可理解access（）函数想表达有做某事的权限。函数参数有两个，第一个为文件，对应的第二个参数为文件有那些权限和是否存在。

### 头文件库

```c
#include<unistd.h>
```

### 声明

 ```c
int access(const char *filenpath, int mode); 或者
int _access( const char *path, int mode );
 ```

### 参数

* **filenpath**  -- 文件或者文件夹的路径，当前目录直接使用文件或文件夹。
  1. 当该参数为文件的时候，**access函数**能使用mode参数所有的值，当该参数为文件夹的时候，access函数值能判断文件夹是否存在。
* **mode**  -- 要判断的模式

| unistd.h中的预定义 |           判断权限           |
| :----------------: | :--------------------------: |
|   #define R_OK 4   |  Test for read permission.   |
|   #define W_OK 2   |  Test for write permission.  |
|   #define X_OK 1   | Test for execute permission. |
|   #define F_OK 0   |     Test for existence.      |

### 返回值

确定文件或文件夹的访问权限。即，检查某个文件的存取方式，比如说是只读方式、只写方式等。如果指定的存取方式有效，则函数返回0，否则函数返回-1。