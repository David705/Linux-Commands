#include <stdio.h>
#include <stdlib.h>
#include "mypattern.h"
#include "mystrings.h"

void retrieveRegex(char *regex_file){

	FILE *fp = fopen(regex_file, "r");

	if(fp == NULL){
		printf("[-] Unable to open file: %s\n", regex_file);
		exit(-1);
	}

	char c;
	regexSize = 0;

	while((c = getc(fp)) != EOF){
		regexSize++;
	}

	if(regexSize == 0){
		printf("[-] Regex file contains nothing\n");
		exit(-1);
	}

	rewind(fp);

	size_t index = 0;
	regex = malloc(regexSize + 1);

	while((c = getc(fp)) != EOF){
		regex[index++] = c;
	}

	regex[index] = '\0';

}

void createTable(){

	size_t index = 0;

	if(regex[index] == '+' || regex[index] == '*'){
		printf("[-] The characters + and * must follow other characters\n");
		exit(-1);
	}

	head = NULL;
	tail = NULL;
	struct row *curr = NULL;

	while(index < regexSize){
		if(regex[index] == '*'){
			curr->ascii[128] = 1;
			index = index + 1;
			continue;
		}
		if(regex[index] == '+'){
			curr->ascii[129] = 1;
			index = index + 1;
			continue;
		}
		// create a row
		if(curr == NULL){
			// curr equals null if this is the first character
			curr = malloc(sizeof(struct row));
			setAsciiToZero(curr);
			curr->prev = NULL;
			curr->next = NULL;
			head = curr;
		}else{
			curr->next = malloc(sizeof(struct row));
			setAsciiToZero(curr->next);
			curr->next->prev = curr;
			curr->next->next = NULL;
			curr = curr->next;
		}
		// implement regex stuff
		if(regex[index] == '.'){
			for(int i = 0; i < 128; i++){
				curr->ascii[i] = 1;
			}
			index = index + 1;
			continue;
		}
		if(regex[index] == '\\'){
			if(index + 1 < regexSize){
				curr->ascii[regex[index+1]] = 1;
				index = index + 2;
				continue;
			}else{
				printf("[-] Escape character, \\, must precede another character\n");
				freeMemory();
				exit(-1);
			}
		}
		if(regex[index] == '@'){
			if(index + 2 < regexSize){
				if('0' <= regex[index+1] && regex[index+1] <= '7'){
					if('0' <= regex[index+2] && regex[index+2] <= '9'){
						char c = ((regex[index+1] - 48) * 16) + (regex[index+2] - 48);
						curr->ascii[c] = 1;
						index = index + 3;
						continue;
					}else if('A' <= regex[index+2] && regex[index+2] <= 'F'){
						char c = ((regex[index+1] - 48) * 16) + (regex[index+2] - 65 + 10);
						curr->ascii[c] = 1;
						index = index + 3;
						continue;
					}else if('a' <= regex[index+2] && regex[index+2] <= 'f'){
						char c = ((regex[index+1] - 48) * 16) + (regex[index+2] - 97 + 10);
						curr->ascii[c] = 1;
						index = index + 3;
						continue;
					}else{
						printf("[-] Not a compatible hex character\n");
						freeMemory();
						exit(-1);
					}
				}else{
					printf("[-] Not a compatible hex character\n");
					freeMemory();
					exit(-1);
				}
			}else{
				printf("[-] At character, @, must precede two compatible hex characters\n");
				freeMemory();
				exit(-1);
			}
		}
		if(regex[index] == '['){
			if(index + 1 < regexSize){
				if(regex[index+1] == ']'){
					printf("[-] A set, [], must contain at least one character\n");
					freeMemory();
					exit(-1);
				}
				bool complement = false;
				if(regex[index+1] == '^'){
					if(index + 2 < regexSize){
						if(regex[index+2] == ']'){
							printf("[-] The complement of a set, [^], must contain at least one character\n");
							freeMemory();
							exit(-1);
						}
						complement = true;
						index = index + 2;
					}else{
						printf("[-] A set, [], must be terminated\n");
						freeMemory();
						exit(-1);
					}
				}else{
					index = index + 1;
				}
				while(index < regexSize){
					if(regex[index] == ']'){
						index = index + 1;
						break;
					}
					if(regex[index] == '\\'){
						if(index + 1 < regexSize){
							curr->ascii[regex[index+1]] = 1;
							index = index + 2;
							continue;
						}else{
							printf("[-] Escape character, \\, must precede another character\n");
							freeMemory();
							exit(-1);
						}
					}
					if(regex[index] == '@'){
						if(index + 2 < regexSize){
							if('0' <= regex[index+1] && regex[index+1] <= '7'){
								char c;
								if('0' <= regex[index+2] && regex[index+2] <= '9'){
									c = ((regex[index+1] - 48) * 16) + (regex[index+2] - 48);
									curr->ascii[c] = 1;
									index = index + 3;
								}
								else if('A' <= regex[index+2] && regex[index+2] <= 'F'){
									c = ((regex[index+1] - 48) * 16) + (regex[index+2] - 65 + 10);
									curr->ascii[c] = 1;
									index = index + 3;
								}
								else if('a' <= regex[index+2] && regex[index+2] <= 'f'){
									c = ((regex[index+1] - 48) * 16) + (regex[index+2] - 97 + 10);
									curr->ascii[c] = 1;
									index = index + 3;
								}
								else{
									printf("[-] Not a compatible hex character\n");
									freeMemory();
									exit(-1);	
								}
								if(index < regexSize){
									if(regex[index] == '-'){
										if(index + 3 < regexSize){
											if(regex[index+1] == '@'){
												if('0' <= regex[index+2] && regex[index+2] <= '7'){
													char c2;
													if('0' <= regex[index+2] && regex[index+2] <= '9'){
														c2 = ((regex[index+2] - 48) * 16) + (regex[index+3] - 48);
														curr->ascii[c2] = 1;
														index = index + 4;
													}
													else if('A' <= regex[index+2] && regex[index+2] <= 'F'){
														c2 = ((regex[index+2] - 48) * 16) + (regex[index+3] - 65 + 10);
														curr->ascii[c2] = 1;
														index = index + 4;
													}
													else if('a' <= regex[index+2] && regex[index+2] <= 'f'){
														c2 = ((regex[index+2] - 48) * 16) + (regex[index+3] - 97 + 10);
														curr->ascii[c2] = 1;
														index = index + 4;
													}
													else{
														printf("[-] Not a compatible hex character\n");
														freeMemory();
														exit(-1);	
													}
													if(c <= c2){
														while(c <= c2){
															curr->ascii[c++] = 1;
														}
														continue;
													}else{
														printf("[-] Invalid range\n");
														freeMemory();
														exit(-1);
													}
												}else{
													printf("[-] Not a compatible hex character\n");
													freeMemory();
													exit(-1);
												}
											}else{
												printf("[-] Invalid range\n");
												freeMemory();
												exit(-1);
											}
										}else{
											printf("[-] Invalid range\n");
											freeMemory();
											exit(-1);
										}
									}
								}
								continue;
							}else{
								printf("[-] Not a compatible hex character\n");
								freeMemory();
								exit(-1);	
							}
						}else{
							printf("[-] At character, @, must precede two compatible hex characters\n");
							freeMemory();
							exit(-1);
						}
					}
					curr->ascii[regex[index]] = 1; 
					index = index + 1;
				}
				if(complement){
					for(int i = 0; i < 128; i++){
						if(curr->ascii[i] == 1)
							curr->ascii[i] = 0;
						else
							curr->ascii[i] = 1;
					}
				}
				continue;
			}else{
				printf("[-] A set, [], must be terminated\n");
				freeMemory();
				exit(-1);
			}
		}
		curr->ascii[regex[index]] = 1;
		index = index + 1;
	}

	tail = curr;

}

void setAsciiToZero(struct row *row){

	for(int i = 0; i < 130; i++){
		row->ascii[i] = 0;
	}

}

void freeMemory(){

	free(regex);

	while(head->next != NULL){
		head = head->next;
		free(head->prev);
	}

	free(head);

	regex = NULL;
	head = NULL;
	tail = NULL;

}

void printTable(){

	struct row *curr = head;

	while(curr != NULL){
		for(int c = 0; c < 130; c++){
			printf("%d ", curr->ascii[c]);
		}
		printf("\n");
		curr = curr->next;
	}

}

void printCharacters(){

	struct row *curr = head;

	while(curr != NULL){
		for(int c = 0; c < 130; c++){
			if(curr->ascii[c] == 1)
				printf("%d ", c);
		}
		printf("\n");
		curr = curr->next;
	}

}