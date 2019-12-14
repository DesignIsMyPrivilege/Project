//
// Created by q7 on 2019/11/28.
//

#ifndef C__TEST_SRAND_FUN_H
#define C__TEST_SRAND_FUN_H

#endif //C__TEST_SRAND_FUN_H

void srand_fun() {
    cout << setfill('*') << setw(20) << "srand" << setfill('*') << setw(15) << "*" << endl;
    const int num = 5;
    int i = 1024, j;
    srand((unsigned) time(NULL));
    j = rand();
    cout << j << endl;
}