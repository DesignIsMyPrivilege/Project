#include<stdio.h>
#include<stdlib.h>

#define size_init 2
#define iteration 9
#define type char


void push(type *a, int *N){
	type item;
	scanf("%c", &item);
	a[(*N)++] = item;
}

int pop(type *a, int *N){
	type item = a[--(*N)];
	a[*N] = '0';
	return item;
}
	

int main(){
	type *a, *copy_push, *copy_pop;
	int N = 0;
	int size = size_init;
	
	a = (type *)malloc(size*sizeof(type));
	
	printf("Please input the things to push:");
	for(int i = 0; i < iteration; i++){
		push(a, &N);
		if(N == size){
			size = 2*size;
			copy_push = (type *)malloc(size*sizeof(type));
			for(int i = 0; i < N; i++){
				copy_push[i] = a[i];
			}
			free(a);
			/* for(int j = 0; j < size; j++){
				printf("The copy num:%d\n",copy[j]);
			} */
			a = copy_push;
		}
	}
	for(int i = 0; i < 6; i++){
		printf("pop:%c\n", pop(a,&N));
		if(N == (size/4) && N > 0){;
			size = size/2;
			copy_pop = (type *)malloc(size*sizeof(type));
			for(int i = 0; i < N; i++){
				copy_pop[i] = a[i];
			}
			free(a);
			a = copy_pop;
		}
	}
	for(int j = 0; j < size; j++){
		printf("The a num is :%c\n", a[j]);
	}
	free(a);
	
	// isEmpty();
	// size();
	
	
	return 0;
}