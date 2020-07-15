# 指针

错误例程：

```c
#include<stdio.h>
#include<stdlib.h>

#define size_init 2
/*该处宏定义就是单纯的把size_init变为2，且size_init在主程序中是作为常量存在的，因为其为常量，所以形如size_init=5的赋值操作是错误的*/

void copy_array(int *a, int num, int N){
    int copy[num];
    /*该处定义的copy，只能在copy_array内部有效，当copy_array被调用完成后，copy就会被释放；想要被调用函数调用完成后，内部的内容不被释放，能够供主函数使用，可以使用static int的定义方式，但是，该定义的情况下，不能含有变量，也就是说，通过static int的定义必须明确指定int的大小，例：static int copy[10]，不能使用变大小来定义*/
    for(int i = 0; i < N; i++){
		copy[i] = a[i];
	}
    a = copy;
}

void push(int *a, int *N){
	int item;
    printf("Please input the things to push:");
	scanf("%d", &item);
	a[(*N)++] = item;
}

int pop(int *a, int *N){
	int item = a[--(*N)];
	a[*N] = '0';
	return item;
}
	

int main(){
    int a[size_init];
    
    copy_array(a,2*size_init,5);
		
	return 0;
}
```

```c
#include<stdio.h>

int *test(int *a){
	static int copy[10];
	for(int i = 10; i >0; i--){
		copy[i] = i;
	}
    a[0] = 100;
    /*传递地址的情况下，在调用函数内部修改a，相当于直接修改了a*/
	a = copy;
    /*将copy的地址赋给了a，此时，在test内部，访问a就是访问copy，二者等价*/
	printf("a address doing:%#x\n", a);
	printf("copy address:%#x\n", copy);
	return copy;
}

int main(){
	int a[5];
	int *copy;
	
	for(int i = 0; i < 5; i++){
		a[i] = i;
	}
	printf("a address before:%#x\n", a);
	copy = test(a);
    /*传递了a的地址，在调用函数内部修改a，相当于直接修改了a*/
	printf("a adress after:%#x\n", a);
    /*调用函数结束后，由于mian函数中有a的地址，于是a的地址被再次赋值，被赋予为main函数中定义的a的地址*/
	printf("copy address after:%#x\n", copy);
	
	printf("\n");
	copy[5] = 11;
	printf("The num of the copy:");
	for(int j = 0; j < 10; j++){
		printf("%d", copy[j]);
	}
	printf("\n");
	printf("The a length:%lu\n", sizeof(a));
	printf("The num of the a:");
	for(int i = 0; i < 5; i++){
		printf("%d", a[i]);
        /*a[0]在test中被修改*/
	}
	printf("\n");
	
	return 0;
}
```

