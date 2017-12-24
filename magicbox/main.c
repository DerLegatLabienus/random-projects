#include <stdio.h>
#include <unistd.h>
#include <string.h>

int result_i = 0;
int **results = NULL;

#define ROW 3
#define 

void print_perms_in_arr(int arr[9], int num) {

	if (num == 10) {
		result_i++;
		results = realloc(results,result_i*sizeof(int *));
		results[result_i-1] = malloc(9*sizeof(int));
		memcpy(results[result_i-1],arr,9*sizeof(int));
		return;
	}
	
	int b[9][9];
	for (int i = 0; i < 9; i++)
		memcpy(b[i],arr,sizeof(int)*9);
	for (int j = 0; j < 9; j++) {
			if (arr[j] != 0) continue;
			b[j][j] = num;
			print_perms_in_arr(b[j],num+1);
	}
}


void print_perms() {
	int a[9] = {0};
	print_perms_in_arr(a,1);
}

void main() {
	print_perms();
	for (int j= 0; j < result_i; j++) {
		printf("%d: ",j);
		for (int i = 0 ; i < 9; i++)
			printf("%d,",results[j][i]);
		printf("\n");
	}

}
