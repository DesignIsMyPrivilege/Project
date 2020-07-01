#include<stdio.h>

void swap_plus(int *array, int i, int j){
	array[i] = array[i] + array[j];
	array[j] = array[i] - array[j];
	array[i] = array[i] - array[j];
}

void swap_minus(int *array, int i, int j){
	array[i] = array[i] - array[j];
	array[j] = array[i] + array[j];
	array[i] = array[j] - array[i];
}

void swap_logic(int *array, int i, int j){
	int temp = (array[i] & array[j]) ^ (array[i] | array[j]);
	array[i] = temp ^ array[i];
	array[j] = temp ^ array[j];
}

void swap_XOR(int *array, int i, int j){
	if(i != j){
		array[i] ^= array[j];
		array[j] ^= array[i];
		array[i] ^= array[j];
	}
}

int main(){
	int array[] = {1,2,3,4,5};
	
	for(int i = 0; i < sizeof(array)/sizeof(int); i++){
		printf("%d ", array[i]);
	}
	printf("\n");
	
	swap_minus(array, 1,2);
	for(int i = 0; i < sizeof(array)/sizeof(int); i++){
		printf("%d ", array[i]);
	}
	printf("\n");
	
	return 0;
}