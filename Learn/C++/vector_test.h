//
// Created by q7 on 2020/6/30.
//

#ifndef C__TEST_VECTOR_TEST_H
#define C__TEST_VECTOR_TEST_H

void vector_test() {
    vector <vector<int>> arry; //写成arry(5) 可理解为设定大小5行
    vector <int> d;        //定义一个一维的数组;
    int i, j, k, n;
    int number;

    scanf("%d", &n);
    /*可以这样实现对vector二维的初始化，得到的是n行n列的矩阵*/
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &number);
            d.push_back(number);
        }
        sort(d.begin(), d.end()); //pai xu xuyao头文件algorithm
        arry.push_back(d);
        //d.clear();        //清空一维的数组
        d.resize(0);
    }
    /*遍历输出*/
    if (arry.empty())
        printf("0\n");
    else {
        for (i = 0; i < arry.size(); i++) {
            for (j = 0; j < arry[0].size(); j++) {
                printf("%d ", arry[i][j]);
            }
            printf("\n");
        }
    }
}
#endif //C__TEST_VECTOR_TEST_H
