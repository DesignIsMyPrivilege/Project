#include<stdio.h>

void init_array(int index, int *id){
   for(int i = 0; i < index; i++){
//      *(id+i) = i;
       id[i] = i;
   }

}//初始化数组

void printf_array(int *id, int index){
   printf("The array num is:"); 
   for(int i = 0; i < index; i++){
      printf("%d ", id[i]);
   }
   printf("\n");
}//打印数组的内容

int find(int *id, int index){
   int id1, id2;
   printf("Input two num to search(num1:0-%d,num2:0-%d)",index,index);
   scanf("%d %d", &id1, &id2);
   return id[id1] == id[id2];
}//查看输入的两个数字是否相通

void union_two(int *id, int pairs, int index){
	int id1, id2;
	for(int i = 0; i < pairs; i++){
		printf("Please input two num:(0-%d)", index);
		scanf("%d %d", &id1, &id2);
		if(id[id1] == id[id2]){
			printf("They are already in the same range!\n");
		}
		else{
			for(int j = 0; j < index; j++){//每次遍历修改更新值
				if(id[j] == id[id1]){
					id[j] = id[id2];
				}
			}
		}

	}
}//合并两个数

int main(int argc, char* argv[]){
   int index;
   int *p;
   int pairs;

   printf("Quick-find!\n");
   printf("Please input number:");
   scanf("%d", &index);
   int id[index];
   init_array(index, id);
   
   printf("How many pairs do you want to union:");
   scanf("%d", &pairs);
   union_two(id, pairs, index);
   
   printf("%d(1:equal)\n", find(id,index));
   printf_array(id, index);
   

   return 0;
}
