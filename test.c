#include <stdio.h>

int main(void) {
	printf("%d\n", printf("%#010x\n", 42));
	printf("%d\n", printf("%#10x\n", 42));
}