#include <iostream>
#include <ctime>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <exception>
#include <csignal>
#include <thread>
#include <pthread.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

#include "pointer.h"
#include "reference.h"
#include "lambda_p.h"
#include "Class_test.h"
#include "srand_fun.h"
#include "string_fun.h"
#include "document.h"
#include "Error.h"
#include "Memory_dyn.h"
#include "temp.h"
#include "define_file.h"
//#include "para_program.h"
#include "test.h"
#include "vector_test.h"
/*int gcd(int a, int b){
    if(a % b == 0){
        return b;
    }
    return gcd(b,a%b);
}*/

/*int gcd(int a, int b){
    return b==0 ? a:gcd(b,a%b);
}*/
//最大公因数和最小公倍数之间的性质：两个自然数的乘积等于这两个自然数的最大公约数和最小公倍数的乘积。

int main()
{
    //test();
    //srand_fun();
    //lambda_p();
    //string_length0();
    //string_length1();
    //Pointer();
    //Array_pointer();
    //Return_pointer();
    //Fun_pointer();
    //Reference();
    //classTest1();
    //classTest2();
    //copy_fun();
    //Attributes0();
    //plus_1();
    //M_status();
    //doc_save();
    //error_test();
    //memory_test();
    //memory_test_array1();
    //memory_test_array2();
    temp_test();
    //temp_class(20,30);
    //temp_class_2();
    //define_test();
    //para_p();
    //vector_test();
    /*char  var[3] = {'a', 'b', 'c'};
    char  *ptr;

    // 指针中的数组地址
    ptr = var;

    for (int i = 0; i < 3; i++)
    {

        cout << "Address of var[" << i << "] = ";
        cout << (int *)ptr << endl;//由于ptr是char *类型，所以，输出地址，必须强制转化为 void *，int *，float *， double * 等。

        cout << "Value of var[" << i << "] = ";
        cout << *ptr << endl;

        // 移动到下一个位置
        ptr++;
    }*/
/*    int a;
    int b;
    cin >> a;
    cin >> b;
    cout << gcd(a,b) << endl;*/


/*

    int a1[4] = {0,2,0,1};
    int a2[4] = {0,3,0,1};
    int a3[4] = {0,4,0,1};
    int* b[3] = {a1,a2,a3};
    //b[0][0] = 5;
    *b[0]= 5;
    for(int i = 0; i < 4; i++){
        cout << 'a1'<< '[' << i << ']';
        cout << a1[i] << endl;
    }
    for(int i = 0; i < 4; i++){
        cout << 'a2'<< '[' << i << ']';
        cout << a2[i] << endl;
    }
    for(int i = 0; i < 4; i++){
        cout << 'a3'<< '[' << i << ']';
        cout << a3[i] << endl;
    }

    for(int i = 0; i < 3; i++){
        cout << 'b'<< '[' << i << ']';
        cout << b[i] << endl;
    }
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 4; j++) {
            cout << 'b' << '[' << i << ']'<<'['<<j<<']';
            cout << b[i][j] << endl;
        }
    }
*/



    return 0;
}

