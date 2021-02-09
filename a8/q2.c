#include <stdlib.h>
#include <stdio.h>

// int a = 18;
// int b = 20;
// int c = 14;
// int d = 0;

int q2(int arg1, int arg2, int arg3){
	int j;
	switch(arg1) {
		case 10: j = arg2 + arg3; break;
		case 12: j = arg2 - arg3; break;
		case 14: if (arg2 > arg3) {
			j = 1;
		} else {
			j = 0;
		} break;
		case 16: if (arg3>arg2){
			j = 1;
		} else {
			j = 0;
		} break;
		case 18: if (arg2 == arg3){
			j = 1;
		} else {
			j = 0;
		} break;
		default: j = 0; break;
	}
	return j;
}

// int main(){
// 	d= q2(a,b,c);
// 	printf("%d, %d, %d, %d",a,b,c,d);
// }