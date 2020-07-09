//
// Created by q7 on 2019/11/28.
//

#ifndef C__TEST_SRAND_FUN_H
#define C__TEST_SRAND_FUN_H

void srand_fun() {
    cout << setfill('*') << setw(20) << "srand" << setfill('*') << setw(15) << "*" << endl;
    const int num = 5;
    int i = 1024, j;
    srand((unsigned) time(NULL));//提供种子，不同的种子对应不同的随机数，同样的种子对应一样的随机数
    for(int count = 0; count < 10; count++){
        j = rand()%(20-10+1)+10;
        //产生[m,n]范围内的随机整数j，即使用 rand() 和 srand() 产生指定范围内的随机整数的方法：
        //方法一：“模除+加法”的方法
        //方法二：int u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min + 1)+ range_min
        //方法一得到的整数概率可能不同，方法二得到的概率相同
        cout << j << endl;
    }
}

#endif //C__TEST_SRAND_FUN_H