#include <stdio.h>
#include "mystrings.h"

size_t strSize(char *str){

	size_t size = 0;

	while(str[size] != '\0'){
		size++;
	}

	return size;

}

bool strEq(char *str1, char *str2){

	size_t size1 = strSize(str1);
	size_t size2 = strSize(str2);

	if(size1 != size2) return false;

	size_t index = 0;

	for(index = 0; index < size1; index++){
		if(str1[index] != str2[index])
			return false;
	}

	return true;

}