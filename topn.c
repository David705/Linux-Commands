/*
	I decided to approach this problem with structures. Every structure
	has a word and a count. Before I create a structure for a four
	character sequence, I check to see if I already have a that four
	character word. If I do, then I increment the count for that word.
	Otherwise, I create a structure with that word and initialize the
	count to 1. After I have captured every possible sequence, I
	sort the set using the selection sort algorithm. Once everything is
	sorted, I print the data out.

	My program runs relatively fast with small input, but as the input
	gets bigger, it takes longer to finish. This is because of two
	things:
		1) Every time I get a new sequence, I iterate through the
			entire set to see if it is there.
		2) The selection sort algorithm has a time complexity of
			O(n^2)
	Put one and two together, and you get a very slow program when
	big input is provided.

	Another thing to note is that this program is memory dependent.
	This might not be an issue with systems with huge amounts of
	memory, but it might be an issue with systems with small amounts
	of memory.

	Overall, this program can be improved, but because of the time
	frame and my busy schedule, this is what I came up with.
*/

#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool;

bool DEBUG = false;
int arguments = 10; // the number of arguments to print; 10 is default
struct Word{
	char str[5];
	int count;
	struct Word *prev;
	struct Word *next;
};
struct Word *head;
struct Word *tail;

int len(char *str);
bool equal(char *str1, char *str2);
char* copy(char *str1, char *str2);
bool inSet(char *str);
void switchContent(struct Word *word1, struct Word *word2);
void sort(void);
bool allPrintable(char *str);
void printCount(void);
void freeMem(void);
void getStdio(void);

int main(int argc, char *argv[], char *envp[]){
	// read options
	int i = 1;
	for(i = 1; i < argc; i++){
		if(equal(argv[i], "-n")){
			// check for the next argument
			i++;
			if(i < argc){
				arguments = atoi(argv[i]);
				if(arguments < 1){
					printf("[-] Invalid Argument: %s (must be greater than 0)\n", argv[i]);
					return -1;
				}
			}
		}else{
			break;
		}
	}
	// get stdio
	getStdio();
	return argc;
}

// returns the length of the string
int len(char *str){
	int len = 0;
	while(str[len] != '\0'){
		len++;
	}
	return len;
}

// returns true if str1 and str2 are equal
// returns false otherwise
bool equal(char *str1, char *str2){
	int len1 = len(str1); // length of str1
	int len2 = len(str2); // length of str2
	if(len1 == len2){
		int i = 0;
		for(i = 0; i < len1; i++){
			if(str1[i] != str2[i]) return false;
		}
		return true;
	}
	return false;
}

// copies str1 into str2
char* copy(char *str1, char *str2){
	int size = len(str1);
	int i = 0;
	for(i = 0; i < size; i++){
		str2[i] = str1[i];
	}
	return str2;
}

// returns true if str is in the set
// returns false otherwise
bool inSet(char *str){
	struct Word *tmp = head;
	while(true){
		if(equal(tmp->str, str)){
			tmp->count++;
			return true;
		}
		if(tmp->next == NULL) break;
		tmp = tmp->next;
	}
	return false;
}

// switches the content of word1 and word2
void switchContent(struct Word *word1, struct Word *word2){
	// temporarily store word1 content
	char str[5];
	copy(word1->str, str);
	int count = word1->count;
	// put word2 content on word1
	copy(word2->str, word1->str);
	word1->count = word2->count;
	//put word1 content on word2
	copy(str, word2->str);
	word2->count = count;
}

// sorts the set according to count
// max count goes in the front
void sort(void){
	struct Word *curr = head;
	struct Word *max = head;
	struct Word *tmp = head;

	while(curr != NULL){
		max = curr;
		tmp = curr->next;
		while(tmp != NULL){
			if(max->count < tmp->count){
				max = tmp;
			}
			tmp = tmp->next;
		}
		switchContent(curr, max);
		curr = curr->next;
	}
}

// checks if every character in the string is pritable
bool allPrintable(char *str){
	int size = len(str);
	int i = 0;
	for(i = 0; i < size; i++){
		if(!(32 <= str[i] && str[i] <= 126))
			return false;
	}
	return true;
}

void printCount(void){
	struct Word *tmp;
	tmp = head;
	int a = 0;
	while(a++ < arguments){
		if(allPrintable(tmp->str))
			printf("%d \"%s\"\n", tmp->count, tmp->str);
		else
			printf("%d \" 0x%x%x%x%x\"\n", tmp->count, tmp->str[0],
				tmp->str[1], tmp->str[2], tmp->str[3]);
		if(tmp->next == NULL) break;
		tmp = tmp->next;
	}
}

// free all memory allocated
void freeMem(void){
	while(head->next != NULL){
		head = head->next;
		if(DEBUG) printf("Freed: %s %d\n", head->prev->str, head->prev->count);
		free(head->prev);
	}
	if(DEBUG) printf("Freed: %s %d\n", head->str, head->count);
	free(head);
	head = NULL;
	tail = NULL;
}

// reads stdio
void getStdio(void){
	char str[5] = {0, 0, 0, 0, 0};
	int c = 0;
	// get the initial set up working
	// then continue getting stdio
	if((c=getchar()) != EOF){
		str[0] = c;
		if((c=getchar()) != EOF){
			str[1] = c;
			if((c=getchar()) != EOF){
				str[2] = c;
				if((c=getchar()) != EOF){
					str[3] = c;
					head = malloc(sizeof(struct Word));
					copy(str, head->str);
					head->count = 1;
					head->prev = NULL;
					head->next = NULL;
					tail = head;
					while((c=getchar()) != EOF){
						str[0] = str[1];
						str[1] = str[2];
						str[2] = str[3];
						str[3] = c;
						if(!inSet(str)){
							struct Word *tmp = malloc(sizeof(struct Word));
							copy(str, tmp->str);
							tmp->count = 1;
							tmp->prev = tail;
							tmp->next = NULL;
							tail->next = tmp;
							tail = tmp;
						}
					}
					if(DEBUG) printf("\n\nPRINT:\n");
					if(DEBUG) printCount();
					if(DEBUG) printf("\nSORT\n");
					sort();
					if(DEBUG) printf("\nPRINT\n");
					printCount();
					if(DEBUG) printf("\nFREE\n");
					freeMem();
				}
			}
		}
	}
}