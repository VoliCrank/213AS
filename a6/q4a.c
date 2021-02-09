#include <stdio.h>
#include <stdlib.h>


int b[10]  = {0,0,0,0,0,0,0,0,0,0};
int* a =b;

void foo(int x,int y){
	a[y] = a[y] + x;
}

int main(int ac, char** av){
	int x = 1;
	int y = 2;
	foo(3,4);
	foo(x,y);
	for (int i = 0; i < 10; i++){
		printf("%d \n", a[i]);
	}
}