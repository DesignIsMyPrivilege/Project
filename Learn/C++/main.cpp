#include <iostream>
#include <ctime>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <exception>
//#include <csignal>
#include <thread>
#include <pthread.h>
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
#include "para_program.h"

int main()
{
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
    //temp_test();
    //temp_class(20,30);
    //temp_class_2();
    //define_test();
    para_p();

    return 0;
}