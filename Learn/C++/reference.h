//
// Created by q7 on 2019/11/27.
//

#ifndef C__TEST_REFERENCE_H
#define C__TEST_REFERENCE_H

int& test(int *p11, int a)//从函数中返回引用
{
    return p11[a];
}

void Reference() {
    cout << setfill('*') << setw(50) << "Reference:C++引用->从函数中返回引用" << setfill('*') << setw(5) << "*" << endl;

    int i11[5] = {0, 1, 2, 3, 4};
    for (int m = 0; m < 5; m++) {
        cout << i11[m] << endl;
    }
    test(i11, 4) = 8;
    cout << "\n";
    for (int m = 0; m < 5; m++) {
        cout << i11[m] << endl;
    }
}
#endif //C__TEST_REFERENCE_H