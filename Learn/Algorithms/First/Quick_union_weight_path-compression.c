#include<stdio.h>

void array_init(int *id, int *sz, int num){
	for(int i = 0; i < num; i++){
		id[i] = i;
	}
	for(int j = 0; j < num; j++){
		sz[j] = 1;
	}
}//初始化数组

int root(int *id, int index){
	while(index != id[index]){
		id[index] = id[id[index]];//路径压缩，让各个节点都连接在自己的根节点上，仅当find的时候，被查找的数才被连接到root结点
		index = id[index];
	}

	return index;
}//查找根节点

void union_two(int *id, int *sz, int num){
	int pairs, id1, id2;
	int id1_v, id2_v;
	printf("How many pairs do you want:");
	scanf("%d", &pairs);
	for(int i = 0; i < pairs; i++){
		printf("Please input two number to union:(0-%d)", num);
		scanf("%d %d", &id1, &id2);
		id1_v = root(id, id1);
		id2_v = root(id, id2);
		if(id1_v == id2_v){
			printf("They are in a same root!\n");
		}
		else{
			if(sz[id1_v] < sz[id2_v]){
				id[id1_v] = id2_v;
				sz[id2_v] += sz[id1_v];
			}
			else{
				id[id2_v] = id1_v;
				sz[id1_v] += sz[id2_v];
			}
			
		}
	}
	
}

int find(int *id){
	int id1, id2;
	printf("Please input two num to search:");
	scanf("%d %d", &id1, &id2);
	
	return root(id, id1) == root(id, id2);
}

void printf_array(int *id, int num){
	printf("The num of the array:");
	for(int i = 0; i < num; i++){
		printf("%d", id[i]);
	}
	printf("\n");
}//打印数组内的数值

void printf_size(int *sz, int num){
	printf("The size of the each root:");
	for(int j = 0; j < num; j++){
		printf("%d", sz[j]);
	}
	printf("\n");
}//打印各个根节点的个数
 
int main(){
	int num;
	printf("Please input the array num:");
	scanf("%d", &num);
	int id[num], sz[num];
	array_init(id, sz, num);
	printf_size(sz,num);
	
	union_two(id,sz,num);
	printf("%d\n", find(id));
	printf_array(id,num);
	
	return 0;
}