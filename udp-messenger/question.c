#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
 char x[] = "Hello, world!";
 char *y = malloc(14);
 sprintf(y, "Hello, world!");
 printf("x = %s\n", x);
 printf("y = %s\n", y);
 printf("sizeof(x) = %d\n", sizeof(x));
 printf("sizeof(y) = %d\n", sizeof(y));
 printf("strlen(x) = %d\n", strlen(x));
 printf("strlen(y) = %d\n", strlen(y));
 return 0;
}
