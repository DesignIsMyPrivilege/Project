//
// Created by q7 on 2019/12/9.
//

#ifndef C__TEST_PARA_PROGRAM_H
#define C__TEST_PARA_PROGRAM_H

#define NUM_THREADS 5

void* say_hello(void *th)
{
    cout << "Hello!" << endl;
}

void para_p()
{
    pthread_t flag[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        int flag1 = pthread_create(&flag[i], NULL, say_hello, NULL);
        if (flag1 != 0)
        {
            cout << "pthread_create error: error_code=" << flag1 << endl;
        }
    }

    pthread_exit(NULL);
}

#endif //C__TEST_PARA_PROGRAM_H
