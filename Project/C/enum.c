//
// Created by q7 on 2019/10/9.
//
#include <stdio.h>
#include <stdlib.h>

enum color
{
    red = 1, green, yellow
}favorite_color;
enum Day
{
    mon = 1, tue, wed, thu = 7, fri, sat, sun
}day;

int main()
{
    int m;

    printf("输入你喜欢颜色的数字：（1:red 2:green 3:yellow）");
    scanf("%d", &favorite_color);

    switch (favorite_color)
    {
        case red:
            printf("你喜欢的颜色是red\n");
            break;
        case green:
            printf("你喜欢的颜色是green\n");
            break;
        case yellow:
            printf("你喜欢的颜色是yellow\n");
            break;
        default:
            printf("你输入的数字有误！\n");
    }


//printf("%d", m);
    return 0;
}
