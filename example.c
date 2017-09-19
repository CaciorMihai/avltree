#include <stdio.h>
#include <string.h>
#include "AVLTree.h"

// Define the length of the string
#define ELEMENT_TREE_LENGTH 3

void* createLong(void* value){
	long *l = malloc(sizeof(long));
	*l = *((long*) (value));
	return l;
}

void destroyLong(void* value){
	free((long*)value);
}

int compareLong(void* a, void* b){
	if(*((long*)a) < *((long*)b))
		return -1;
	if(*((long*)a) > *((long*)b))
		return 1;
	return 0;
}

void* createStrElement(void* str){
	char *c = (char*)malloc((ELEMENT_TREE_LENGTH + 1) * sizeof(char));
	if(!c){
		printf("Eroare\n");
		exit(1);
	}
	strncpy(c, (char*)str, ELEMENT_TREE_LENGTH);
	c[3] = '\0';
	return (void*)c;
}

void destroyStrElement(void* elem){
	free((char*)elem);
}

int compareStr(void* str1, void* str2){
	return strcmp( (char*)str1, (char*)str2 );
}

void main(){
	printf("Hey there\n");
}