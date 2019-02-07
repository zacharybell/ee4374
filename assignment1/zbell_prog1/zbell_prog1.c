#include <stdio.h>
#include <stdlib.h>
#include "zbell_argtok.h"

#define MAX 1000

int main() {

	// read input
	printf("$ ");
	char buffer[MAX];
	fgets(buffer, MAX, stdin);

	char** tokens = argtok(buffer);

	char** tok_ptr = tokens;
	while(*tok_ptr) {
		printf("%s\n", *tok_ptr);
		tok_ptr++;
	}

	free_tokens(tokens);
	
	return 0;
}
