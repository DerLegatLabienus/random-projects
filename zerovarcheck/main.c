#include <stdio.h>

#pragma pack(0)	
struct var {
	int a;
	char r[];
} var;
int main(int argc, char **argv) {


	struct var *s = (struct var *)malloc(sizeof(var)+12*sizeof(char));

	s->r[0] = 'a';
	s->r[1] = 'a';
	s->r[2] = 'a';
	s->r[3] = 'a';
	s->r[4] = 'a';
	s->r[5] = 'a';
	s->r[6] = 'a';

	for (int i =0; i<12 ; i++)
		printf("this is %d ", s->r[i]);
	printf("s2 sizeof == %d\n", sizeof(var));
}
