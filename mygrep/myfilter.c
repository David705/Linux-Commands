#include <stdio.h>
#include <stdlib.h>
#include "mypattern.h"
#include "mystrings.h"
#include "myfilter.h"

void grepFile(char *filename){

	FILE *fp = fopen(filename, "r");

	if(fp == NULL){
		printf("[-] Unable to open file: %s\n", filename);
		freeMemory();
		exit(-1);
	}

	char *line = NULL;
	size_t dummy = 0;
	ssize_t lineSize = 0;
	ssize_t maxSize = 0-1;

	while(lineSize != maxSize){
		// get line
		line = malloc(sizeof(char));
		lineSize = getline(&line, &dummy, fp);
		// find matches
		struct row *curr = head;
		ssize_t index = 0;
		while(index < lineSize){
			if(curr == NULL){
				printf("%s", line);
				break;
			}
			if(curr->ascii[line[index]] == 1){
				// matches
				index = index + 1;
				if(curr->ascii[128] == 1 || curr->ascii[129] == 1){
					while(curr->ascii[line[index]] == 1){
						index = index + 1;
					}
				}
				curr = curr->next;
			}else{
				// doesn't match
				if(curr->ascii[128] == 1){
					// one or more (life-saver)
					curr = curr->next;
				}else{
					// your screwed, start all over again
					curr = head;
					if(curr->ascii[line[index]] == 1){
						// this character is part of the sequence
						curr = curr->next;
					}
					index = index + 1;
				}
			}
		}
		// free line
		free(line);
	}

}