#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void print_char(char input);
void put_string(char * str, int bytes);
char read_char();
void get_string(int bytes);

int main()
{
    // Writing Section starts here
    printf("====Printing====\n");
	char * buffer_out = "Print Me";
	put_string(buffer_out, strlen(buffer_out));
	// Reading portion starts here
	printf("====READING====(Type)\n");
	get_string(5);
	// Reading portion starts here
	return 0;
}

void put_string(char * str, int bytes){
	int index = 0;
	while (index <bytes){
		print_char(*str++);
		index++;
	}
	print_char('\n');
}
void get_string(int bytes){
	int index = 0;
	char arr[64];
	printf("Read: ");
	while (index < bytes){
		printf("%c", read_char());
		index++;
	}
	printf("\n");
}
void print_char(char input){
	int ret;
	__asm__ __volatile__(
	"int $0x80"
	:"=a"(ret)
	:"a"(4), "b"(1), "c"(&input), "d"(1)
	: "memory"
	);
}

char read_char(){
	int ret;
	char buf;
	__asm__ __volatile__(
	"int $0x80"
	:"=a"(ret)
	:"a"(3), "b"(0), "c"(&buf), "d"(1)
	: "memory"
	);
	return buf;
}