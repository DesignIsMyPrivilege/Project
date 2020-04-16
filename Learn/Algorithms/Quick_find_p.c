#include<stdio.h>

void init_array(int index, int *a){
   for(int i = 0; i < index; i++){
//      *(a+i) = i;
       a[i] = i;
   }

}//初始化数组

void printf_array(int *a, int index){
   for(int i = 0; i < index; i++){
      printf("%d ", a[i]);
   }
   printf("\n");
}//打印数组的内容

void find(int *a, int index){
   int id_s1, id_s2;
   printf("Input two num to search(num1:0-%d,num2:0-%d)",index,index);
   scanf("%d %d", &id_s1, &id_s2);
   if(a[id_s1] == a[id_s2]){
      printf("They are have the same value!\n");
   }
   else{
      printf("They are have the different value!\n");
   }
}//查看输入的两个数字是否相通

void search(int i, int *a, int *id1_array, int *id2_array){
      int temp1, temp2;
      if(i != 0){
         for(int j = (i-1); j >= 0; j--){
            if(id1_array[i] == id2_array[j]){
               temp1 = id1_array[j];
               temp2 = id2_array[i];
               a[temp1] = a[temp2];
               search(j, a, id1_array, id2_array);
            }
         }
      }
}

void union_two(int *a, int pairs){
   int id_1, id_2;
   int id1_array[pairs], id2_array[pairs];
   for(int i = 0; i < pairs; i++){
      printf("Input two numbers to be unioned:");
      scanf("%d %d", &id_1, &id_2);
      id1_array[i] = id_1;
      id2_array[i] = id_2;
      if(a[id_1] == a[id_2]){
         printf("Equal!\n");
      }
      else{
         a[id_1] = a[id_2];//输入的两个数字不相通时，后者的值赋值给前者
         printf("Action!\n");
         search(i, a, id1_array, id2_array);//修改其他与被赋值数字相通的数字，使相同的数都保持和最后一个数字的值相同
      }
   }
}

int main(int argc, char* argv[]){
   int index;
   int *p;
   int pairs;

   printf("Quick-find!\n");
   printf("Please input number:");
   scanf("%d", &index);
   int a[index];
   init_array(index, a);
   printf("How many pairs do you want to union:");
   scanf("%d", &pairs);
   union_two(a, pairs);
   find(a,index);
   printf_array(a, index);
   

   return 0;
}
