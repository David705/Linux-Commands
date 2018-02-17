/*

							myod Command Manual

	NAME
		myod -- ASCII Dump in octal, decimal, and hex

	SYNOPSIS
		myod [options] < [file]

	DESCRIPTION
		The myod utility dumps all the ASCII characters provided in the file.
		By default, the format looks like this:
			<offset> <character> <character> ... <character>
			<offset> <character> <character> ... <character>
			<offset>
		Depending on the options provided, the offset can be represented in
		octal (default), decimal, and hex. You can also opt to eliminte the
		offset and just print out the characters.

	OPTIONS
		-A [odxn]
			Tells myod to print the offset in octal (o), decimal (d),
			hexadecimal (x), or none (n)
		-j [offset]
			Tells myod to skip the first couple of bytes
		-N [bytes]
			Tells myod to print only a specified amount of bytes

	EXAMPLES
		myod < file
			Dumps all the ASCII characters in the file. The offset is
			represented in octal.
		myod -A x -j 1 < file
			Dumps all the ASCII characters in the file after the
			first character. Offset is represented in hexadecimal.
		myod -A n -j 1 -N 1 < file
			Dumps the second character from the file. The offset is omitted.

	AUTHOR
		Written by David Chavarria

*/


#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool;

int offset = 0;
char offset_format = 'o';
int bytes_to_print = 0;

unsigned int strLength(char *str);
bool streq(char *str1, char *str2);
void readOptions(int argc, char *argv[]);
void myod(void);

unsigned int strLength(char *str){
	unsigned int size = 0;
	while(str[size] != '\0'){
		size++;
	}
	return size;
}

bool streq(char *str1, char *str2){
	unsigned int len1 = strLength(str1);
	unsigned int len2 = strLength(str2);
	if(len1 != len2) return false;
	unsigned int i = 0;
	for(i = 0; i < len1; i++){
		if(str1[i] != str2[i])
			return false;
	}
	return true;
}

void readOptions(int argc, char *argv[]){
	int i = 1;
	for(i = 1; i < argc; i++){
		if(streq(argv[i], "-A")){
			i++;
			if(i < argc){
				if(streq(argv[i], "d")){
					offset_format = 'd';
				}else if(streq(argv[i], "o")){
					offset_format = 'o';
				}else if(streq(argv[i], "x")){
					offset_format = 'x';
				}else if(streq(argv[i], "n")){
					offset_format = 'n';
				}else{
					printf("[-] -A option only accepts arguments: d, o, x, n\n");
					exit(-1);
				}
			}else{
				printf("[-] -A option is missing an argument: d, o, x, n\n");
				exit(-1);
			}
		}else if(streq(argv[i], "-j")){
			i++;
			
			offset = atoi(argv[i]);if(i < argc){
			
			if(offset < 1){
			
					printf("[-] -j only accepts a positive integer\n");
					exit(-1);
				}
			}else{
				printf("[-] -j option is missing a positive integer as an argument\n");
				exit(-1);
			}
		}else if(streq(argv[i], "-N")){
			i++;
			if(i < argc){
				bytes_to_print = atoi(argv[i]);
				if(bytes_to_print < 1){
					printf("[-] -N only accepts a positive integer\n");
					exit(-1);
				}
			}else{
				printf("[-] -N option is missing a positive integer as an argument\n");
				exit(-1);
			}
		}
	}
}

void myod(void){
	char *ascii[33] = {
		"nul", "soh", "stx", "etx", "eot", "enq", "ack",
		"bel", "bs", "ht", "nl", "vt", "ff", "cr", "so",
		"si", "dle", "dc1", "dc2", "dc3", "dc4", "nak",
		"syn", "etb", "can", "em", "sub", "esc", "fs",
		"gs", "rs", "us", "sp"
	};
	int c = 0;
	int count = 0;
	// skip over to the offset
	while(count < offset){
		if((c = getchar()) == EOF){
			printf("[-] Offset provided is bigger than the file length\n");
			exit(-1);
		}
		count++;
	}
	count = 0;
	// read stdio
	while((c=getchar()) != EOF){
		// every 16 characters, print address
		if(count % 16 == 0){
			if(offset_format == 'd'){
				printf("%07d", (count + offset));
			}else if(offset_format == 'o'){
				printf("%07o", (count + offset));
			}else if(offset_format == 'x'){
				printf("%06x", (count + offset));
			}
		}
		// if c is a standard ASCII character, print c
		if(0 <= c && c <= 32){
			printf("%4s", ascii[c]);
		}else if(33 <= c && c <= 126){
			printf("%4c", c);
		}else if(c == 127){
			printf("%4s", "del");
		}else{
			printf("%4s", "???");
		}
		count++;
		if(count % 16 == 0) printf("\n");
		// if we reached bytes_to_print, break
		if(0 < bytes_to_print && bytes_to_print <= count)
			break;
	}
	if(count % 16 != 0) printf("\n");
	if(offset_format == 'd'){
		printf("%07d\n", (count + offset));
	}else if(offset_format == 'o'){
		printf("%07o\n", (count + offset));
	}else if(offset_format == 'x'){
		printf("%06x\n", (count + offset));
	}
}

int main(int argc, char *argv[], char *envp[]){
	// read options
	readOptions(argc, argv);
	// run myod
	myod();
	return 0;
}