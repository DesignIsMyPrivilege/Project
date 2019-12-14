#include<stdio.h>

int main()
{
	int a = 10, c;
	printf("先赋值后计算：\n");
	c = a ++;
	printf("Line 1 - c 的值为:%d\n",c);
	printf("Line 2 - a 的值为:%d\n",a);

	a = 10;
	c = a --;
	printf("Line 3 - c 的值为:%d\n",c);
	printf("Line 4 - a 的值为:%d\n",a);
	
	printf("先计算后赋值：\n");
	c = ++a;
	printf("Line 5 - c 的值为:%d\n",c);
	printf("Line 6 - a 的值为:%d\n",a);
	c = --a;
	printf("Line 7 - c 的值为:%d\n",c);
	printf("Line 8 - a 的值为:%d\n",a);

	return 0;
}
