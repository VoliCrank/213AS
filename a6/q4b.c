#include <stdio.h>
#include <stdlib.h>

int x[] = {1,2,3,-1,-2,0,184,340057058};
int y[] = {0,0,0,0,0,0,0,0};


int f(int a){
	int ret = 0;
	while (a != 0){
		int c = a & 0x80000000;
		if (c != 0){
		ret++;
	}
		a = a<<1;
	}
return ret;
}

int main(int ac, char** av){
	int i = 8;
	while (i!= 0){
		i--;
		y[i]=f(x[i]);
	}

	for (int i = 0; i < 8; i++){
		printf("%d \n", x[i]);
	}
		for (int i = 0; i < 8; i++){
		printf("%d \n", y[i]);
	}
}