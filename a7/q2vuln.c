#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Str {
	int size;
	char* str_;
};

struct Str str1 = {30, "Welcome! Please enter a name: \n"};
struct Str str2 = {11, "Good luck, \n"};


void print(struct Str arg1){
	int size = arg1.size;
	write(1, arg1.str_, size);
}


int main(){
	char buf[128];
	print(str1);
	read(0,buf,256);
	print(str2);
	write(1,buf,256);
}

