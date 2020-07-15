#include<stdio.h>
#include<stdlib.h>

#define TYPE char
#define ITERATION 5

typedef struct Node{
	TYPE item;
	struct Node *next;
}node, *nodeptr;

struct Linked{
	nodeptr top;
	nodeptr end;
	int count;
};

/* struct Linked{
	nodeptr top[ITERATION];
	nodeptr end;
	int count;
}; */
//使用单独的结构体指针数组来存放队列中进入的元素地址

/* void enqueue(node *temp_node, struct Linked *list, TYPE *item){
	temp_node->item = *item;
	temp_node->last = list->end;
	list->end = temp_node;
	list->count++;
} 
void dequeue(struct Linked *list, int j){
	printf("%c\n", list->top[j]->item);
	free(list->top[j]);
}*/
//配合：使用单独的结构体指针数组来存放队列中进入的元素地址
void enqueue(nodeptr temp_node, struct Linked *list, TYPE *item){
	temp_node->item = *item;
	temp_node->next = NULL;
	if(list->count == 0){
		list->top = temp_node;
	}
	list->end->next = temp_node;
	list->count++;
}
void dequeue(struct Linked *list){
	nodeptr top_temp;
	top_temp = list->top;
	//printf("The address of the list-top0:%#x\n", list->top);
	////printf("The address of the top_temp:%#x\n", top_temp);
	printf("%c\n", top_temp->item);
	//printf("The address of the list-top1:%#x\n", list->top);
	list->top = list->top->next;
	free(top_temp);
	list->count--;
	//printf("The address of the list-top2:%#x\n", list->top);
}

int main(){
	nodeptr temp_node;
	struct Linked list;
	TYPE item;
	
	list.end = NULL;
	list.count = 0;
	
	printf("Please input the thing:");
	temp_node = (nodeptr)malloc(sizeof(node));
	for(int i = 0; i < ITERATION; i++){
		list.end = temp_node;
		temp_node = (nodeptr)malloc(sizeof(node));
		scanf("%c", &item);
		enqueue(temp_node,&list,&item);
		//list.top[i] = temp_node;
		//存放元素的地址
	}
	
	for(int j = 0; j < ITERATION-1; j++){
		dequeue(&list);
	}
	
	printf("The num is:%d\n", list.count);
	
	return 0;
}