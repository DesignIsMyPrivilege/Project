//
// Created by q7 on 2019/11/27.
//

#ifndef C__TEST_LAMBDA_P_H
#define C__TEST_LAMBDA_P_H


void lambda_p()
{
    int i = 1024;

    cout << setfill('*') << setw( 20) << "lambda" << setfill('*') << setw( 15) << "*"<< endl;
    auto lambda = [&](int a, int b){cout << i << endl; cout << a << "\n"<< b << endl; i = 1000;};
    lambda(10, 15);
    cout << i << endl;
}

#endif //C__TEST_LAMBDA_P_H