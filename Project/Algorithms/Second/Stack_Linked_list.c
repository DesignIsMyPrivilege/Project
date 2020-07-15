#include<stdio.h>
#include<stdlib.h>

#define TYPE char
#define ITERATION 8

typedef struct Node{
	TYPE item;
	struct Node *next;
}node, *nodeptr;

struct Linked_list{
	nodeptr top;
	int num;
}Linked;

void push(node *s, struct Linked_list *S, TYPE *item){	
	s->item = *item;
	s->next = S->top;
	S->top = s;
	S->num++; 
}

void pop(struct Linked_list *S){
	nodeptr old_s;
	old_s = S->top;
	S->top = old_s->next;
	printf("%c\n", old_s->item);
	free(old_s);
	S->num--;
} 


int main(){
	TYPE item;
	struct Linked_list S;
	node *sptr;
	node s;
	
	S.top = NULL;
	S.num = 0;
	printf("Please input the thing:");
	for(int i = 0; i < ITERATION; i++){
		sptr = (nodeptr)malloc(sizeof(node));
		scanf("%c", &item);
		push(sptr, &S, &item);
	}
	printf("The num:%d\n", S.num);
	
	for(int j = 0; j < ITERATION; j++){
		pop(&S);
	}
	printf("The num:%d\n", S.num);
	
	// isEmpty();
	// size();
	
	return 0;
}