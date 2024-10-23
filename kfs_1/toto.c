#include <stdio.h>

int main(){
	char toto[12] = {[2] = 'b', [5] = 'z'};

	for (int i = 0; i < 12; ++i){
		
		printf("%d: %d\n", i, toto[i]);}
}

