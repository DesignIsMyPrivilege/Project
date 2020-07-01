#include<stdio.h>

void array_init(int *id, int num){
	for(int i = 0; i < num; i++){
		id[i] = i;
	}
}//初始化数组

int find_root(int *id, int index){
	while(index != id[index]){
		index = id[index];
	}
	return index;
}//查找一个数的根节点

void union_two(int num, int *id){
	int id1, id2, pairs;
	int id1_v, id2_v;
	printf("How many pairs do you want to union:");
	scanf("%d", &pairs);
	for(int i = 0; i < pairs; i++){
		printf("Please input two num to union:(0-%d)", num);
		scanf("%d %d", &id1, &id2);
		id1_v = find_root(id, id1);
		id2_v = find_root(id, id2);
		if(id1_v == id2_v){
			printf("They are in the same region!\n");
		}
		else{
			id[id1_v] = id2_v;
		}
	}
}//合并两个数

int find(int *id){
	int id1, id2;
	printf("Please input two to find:");
	scanf("%d %d", &id1, &id2);
	return find_root(id,id1) == find_root(id,id2);
}//查看两个数是否属于一个连通域

void printf_array(int *id, int num){
	printf("The array value is:");
	for(int i = 0; i < num; i++){
		printf("%d ", id[i]);
	}
	printf("\n");
}

int main(){
	int num;
	printf("Please into the num of array:");
	scanf("%d", &num);
	int id[num];
	array_init(id, num);
	
	union_two(num, id);
	printf("%d(1:Same root)\n",find(id));
	printf_array(id, num);
	
	return 0;
}