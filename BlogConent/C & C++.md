# C & C++

```c++
#define   //define宏定义只是字符替换，编译时没有检查错误的功能
```

***

## NULL的区别

NULL表示指针不指向任何对象，C和C++中的NULL不等价，

C中NULL为

```c
#define NULL (void*) 0 //但同时也允许将NULL定义为整常数0
```

C++中NULL为

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

***

## C++中的智能指针

只要将 new 运算符返回的指针 p 交给一个 `shared_ptr` 对象“托管”，就不必担心在哪里写`delete p`语句——实际上根本不需要编写这条语句，托管 p 的 `shared_ptr` 对象在消亡时会自动执行`delete p`。而且，该 `shared_ptr` 对象能像指针 p —样使用，即假设托管 p 的 `shared_ptr` 对象叫作 ptr，那么 *ptr 就是 p 指向的对象。

通过 `shared_ptr` 的构造函数，可以让 `shared_ptr` 对象托管一个 new 运算符返回的指针，写法如下：

```c++
 shared_ptr<T> ptr(new T);// T 可以是 int、char、类等各种类型
```

此后，ptr 就可以像 T* 类型的指针一样使用，即 *ptr 就是用 new 动态分配的那个对象；多个 `shared_ptr` 对象可以共同托管一个指针 p，当所有曾经托管 p 的 `shared_ptr` 对象都解除了对其的托管时，就会执行`delete p`。

只有指向**动态分配**的对象的指针才能交给 `shared_ptr` 对象托管。将指向**普通局部变量、全局变量**的指针交给 `shared_ptr` 托管，编译时不会有问题，但程序运行时会出错，因为不能析构一个并没有指向动态分配的内存空间的指针。

