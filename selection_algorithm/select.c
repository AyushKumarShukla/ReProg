#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_LEN 100

typedef struct{
	int vals[MAX_LEN];
	int cnt;
}list;

list make_list(int vals[],int cnt){
	list l;
	memcpy(l.vals, vals, sizeof(l.vals));
	l.cnt = cnt;

	return l;
}

void print_list(list l){
	for(int i=0;i<l.cnt;i++){
		printf("%d ",l.vals[i]);
	}
	printf(", cnt:%d",l.cnt);
	printf("\n");
}

list main_list, left_list, right_list;
int num_elements;


void split_list(int pivot){
	
	for(int j=0;j<main_list.cnt;j++){
		if(main_list.vals[j]<pivot){
			left_list.vals[left_list.cnt] = main_list.vals[j];
			left_list.cnt++;
		}
		else if(main_list.vals[j]>pivot){
			right_list.vals[right_list.cnt] = main_list.vals[j];
			right_list.cnt++;
		}
	}

}

int cnt=1;
void print_trace(int pivot){
	printf("\n\t\tSplit %d\n",cnt++);
	printf("\n---------------------------------\n");
	printf("Left_List: ");
	print_list(left_list);
	printf("Right_List: ");
	print_list(right_list);
	printf("main_List: ");
	print_list(main_list);
	printf("\nPivot = %d",pivot);
	printf("\n---------------------------------\n");

}

/*
 * WORST CASE : 1 3 5 7 9 11 13 15 17 19 21 23 25 27 29 30 28 26 24 22 20 18 16 14 12 10 8 6 4 2 with p:30, i:1
 * BEST  CASE : 22 6 16 30 8 29 10 1 14 9 28 23 27 15 24 25 20 7 17 26 19 12 4 18 3 2 21 13 5 11 with p:25, i:25
 * */

// Time Complexity : O(n)
int find(int pivot, int i){
	while(1){
		
		split_list(pivot);
		print_trace(pivot);		
		
		if(left_list.cnt > i-1){
			memcpy(main_list.vals, left_list.vals, sizeof(main_list.vals));
			main_list.cnt = left_list.cnt;
			pivot = left_list.vals[left_list.cnt/2];
		}

		else if(left_list.cnt == i-1)
				break;

		else if(left_list.cnt < i-1){
			i -= left_list.cnt + 1;
			memcpy(main_list.vals, right_list.vals, sizeof(main_list.vals));
			main_list.cnt = right_list.cnt;
			pivot = right_list.vals[right_list.cnt/2];
		}
		
		//clear Al
		memset(left_list.vals,0,sizeof(left_list.vals));
		left_list.cnt = 0;
		
		//clear Ar
		memset(right_list.vals,0,sizeof(right_list.vals));
		right_list.cnt = 0;
			
	}
	return pivot;
}

int main(int argc, char* argv[]){
	
	if(argc < 2){
		printf("Usage: %s <num1> <num2> ... \n",argv[0]);
		return 0; 
	}
	
	num_elements = argc - 1; //n
	int *int_array = malloc(num_elements * sizeof(int));
	
	for(int i = 0; i < num_elements; i++) {
        	// argv[i + 1] skips the program name at argv[0]
        	int_array[i] = atoi(argv[i + 1]);
    	}


	main_list = make_list(int_array, num_elements);
	left_list = make_list((int[]){},0);
	right_list = make_list((int[]){},0);
	
	int pivot;
	int i;
	printf("p, i:\t");
	scanf("%d%d",&pivot,&i);
	printf("\n\n%dth smallest element: %d\n",i,find(pivot,i));
	return 0;

}
