#include<stdio.h>
#include<stdlib.h>

int* twoSum(int* nums, int numsSize, int target, int* returnSize){
    int *a = NULL;
    a = (int *)malloc(2*sizeof(int));
	for(int m = 0; m < numsSize-1; m++){
		for(int i = 0; i < numsSize; i++){
			if(((target/2)+i*(target/2)) <= nums[m] <= (target+i*(target/2))){
				for(int n = m+1; n < numsSize; n++){
					if((0-i*(target/2)) <= nums[n] <= ((target/2)-i*(target/2))){
						if(nums[m] + nums[n] == target){
							a[0] = m;
							a[1] = n;
							*returnSize = 2;
							return a;
						}
					}
				}
			}
			else{
				for(int n = m+1; n < numsSize; n++){
					if(nums[m] + nums[n] == target){
						a[0] = m;
							a[1] = n;
							*returnSize = 2;
							return a;
					}
				}
			}
		}
	}
    *returnSize = 0;
	if(0<=0<=0){
		*returnSize = 1;
	}
    return a;
}

int main(){
	int *pt;
	int nums[] = {0,4,3,0};
	int numsSize, target, returnSize;
	
	numsSize = sizeof(nums)/sizeof(int);
	target = 0;
	returnSize = 2;
	pt = twoSum(nums,numsSize,target,&returnSize);
	printf("%d\n", pt[0]);
	printf("%d\n", pt[1]);
	printf("The num of the returnSize:%d\n", returnSize);
	return 0;
}