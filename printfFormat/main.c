#include <stdio.h>
#include "main.h"
int main(int argc, char **argv)
{

/*	char a[3];
	a[0] = a[1] = a[2] = 'a';
	int i = 0;

	a[1 + (i++) ] = 'b';


	printf("%c %c %c\n", a[0], a[1], a[2]);

*/

#define TITLE1 "Celsius"
#define TITLE2 "Farhenheit"


gcov_clear();
int f = fun(3);
int x = strlen(TITLE1);
int len2 = strlen(TITLE2);
char readingsFormatting [300]  = {'\0'};
char headerFormatting [300] = {'\0'};
char tmp[50] ={'\0'};

sprintf(tmp, "%%%d.%ds %%%d.%ds ",x+1,x+1, len2+1,len2+1);
for (int i = 0; i < 2 ; i++)
	strcat(headerFormatting, tmp);
strcat(headerFormatting, "\n");
printf ("%s\n", headerFormatting);

printf (headerFormatting, TITLE1, TITLE2, TITLE1, TITLE2);
printf (headerFormatting, "==================", "==================", "==================", "==================");

char *tmp1 = strchr(headerFormatting,'s');
while (tmp1!=NULL)
{
 *tmp1 = 'd';	
 tmp1 = strchr(tmp1+1,'s');
}
//printf("%s\n",headerFormatting);

printf(headerFormatting, 7 , 220, 300, 100);
}
