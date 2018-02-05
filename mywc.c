/*

							mywc Command Manual

	NAME
		mywc -- counts lines, words, and characters

	SYNOPSIS
		mywc [options] [file ...]
		mywc [options]

	DESCRIPTION
		The mywc utility displays the number of lines, words, and characters
		contained in the files or standard input provided. A line is defined
		as a string of characters delimited by a <newline> character. A word
		is defined as a string of characters delimited by the 
		<horizontal_tab>, <newline>, <vertical_tab>, <newpage>, 
		<carriage_return>, and <space> characters. A character is defined
		as any character in the ASCII Table. If more than one file is
		provided, a cumulative count is displayed.

	OPTIONS
		-l		Displays the number of lines in each input file

		-w		Displays the number of words in each input file

		-c		Displays the number of characters in each input file

		-C		Eliminates single-line C-language comments. Does not
				include them in the line, word, and character count.
				Example of a single-line comment: "// this is a comment"

		mywc only reports the information requested. For example, using the
		"-l" option only reports the number of lines in the input file. Not
		including any options is the same as using the "-l", "-w", "-c"
		options.

		If no files are specified, standard input is used. To exit the
		prompt press [^D], or send an End Of File (EOF).

		The order of the output will always be of the form:
		<num_of_lines> <num_of_words> <num_of_characters> <filename>

	EXAMPLES
		mywc file1 file2
			Counts the number of lines, words, and characters of each file,
			display the count for each, and then display the comulative
			count.

		mywc -c file1
			Displays the number of characters in file1

		mywc -w -C
			Displays the number of words provided by the input
			stream, excluding single-line comments

	FUTURE CONSIDERATIONS
		When comparing the output of mywc with that of the the linux wc
		command, mywc displays a smaller word count. Further examination
		of the wc command will have to come into place to fix this issue.

*/


#include "stdio.h"
#include "string.h"
#include "ctype.h"

typedef enum {false, true} bool;

// options
bool printLines = false;
bool printWords = false;
bool printChars = false;
bool omitComments = false;

// total number of lines, words, and chars
unsigned long lines = 0;
unsigned long words = 0;
unsigned long chars = 0;

void readStdin(void);
void readFile(char *name);
void myPrintf(unsigned long lines, unsigned long words, unsigned long chars, char *name);

int main(int argc, char *argv[], char *envp[]){
	// read options
	int i = 0;
	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-l") == 0){
			printLines = true;
		}else if(strcmp(argv[i], "-w") == 0){
			printWords = true;
		}else if(strcmp(argv[i], "-c") == 0){
			printChars = true;
		}else if(strcmp(argv[i], "-C") == 0){
			omitComments = true;
		}else{
			break;
		}
	}
	// no options were provided; thus, turn on all options
	if(!(printLines || printWords || printChars)){
		printLines = true;
		printWords = true;
		printChars = true;
	}
	// if files were provided, read file
	// otherwise, read from stdin
	if(i < argc){
		bool keepTotal = (argc - i > 1) ? true: false;
		for(i; i < argc; i++){
			readFile(argv[i]);
		}
		if(keepTotal) myPrintf(lines, words, chars, "total");
	}else{
		readStdin();
	}
	return argc;
}

void readStdin(void){
	// keeps count for stdin
	unsigned long tmpChars = 0;
	unsigned long tmpWords = 0;
	unsigned long tmpLines = 0;
	// begin counting chars, words, and lines
	bool word = false; // is the loop still counting a word
	int c = 0;
	while((c=getchar()) != EOF){
		// if remove comment option is set, check for comments
		if(omitComments){
			// if comment detected, remove from count
			if(c == 47){
				int tmpc = getchar();
				if(tmpc == 47){
					// comment detected; remove from count
					do{
						c=getchar();
					}while((c != 10) && (c != EOF));
					if(c == EOF) break;
				}else{
					ungetc(tmpc, stdin);
				}
			}
		}
		// character detected; update char count
		tmpChars++;
		chars++;
		// character is part of a word
		if(33 <= c && c <= 126){
			// counts the start of a word only
			if(!word){
				word = true;
				tmpWords++;
				words++;
			}
		}
		// character is a delimiter
		else if((9 <= c && c <= 13) || (c == 32)){
			word = false;
			// carriage return detected update line count
			if(c == 10){
				tmpLines++;
				lines++;
			}
		}
	}
	// print out results for stdin
	myPrintf(tmpLines, tmpWords, tmpChars, "");
}

void readFile(char *name){
	// open file
	FILE *fp = NULL;
	fp = fopen(name, "r");
	if(fp != NULL){
		// keeps count for this file
		unsigned long tmpChars = 0;
		unsigned long tmpWords = 0;
		unsigned long tmpLines = 0;
		// begin counting chars, words, and lines
		int c = 0;
		bool word = false; // is the loop still counting a word
		while((c=getc(fp)) != EOF){
			// if remove comment option is set, check for comments
			if(omitComments){
				// if comment detected, remove from count
				if(c == 47){
					int tmpc = getc(fp);
					if(tmpc == 47){
						// comment detected; remove from count
						do{
							c=getc(fp);
						}while((c != 10) && (c != EOF));
						if(c == EOF) break;
					}else{
						ungetc(tmpc, fp);
					}
				}
			}
			// character detected; update char count
			tmpChars++;
			chars++;
			// character is part of a word
			if(33 <= c && c <= 126){
				// counts the start of a word only
				if(!word){
					word = true;
					tmpWords++;
					words++;
				}
			}
			// character is a delimiter
			else if((9 <= c && c <= 13) || (c == 32)){
				word = false;
				// carriage return detected update line count
				if(c == 10){
					tmpLines++;
					lines++;
				}
			}
		}
		fclose(fp);
		// print out results for this file
		myPrintf(tmpLines, tmpWords, tmpChars, name);
	}
}

void myPrintf(unsigned long lines, unsigned long words, unsigned long chars, char *name){
	if(printLines)
		printf("%8lu", lines);
	if(printWords)
		printf("%8lu", words);
	if(printChars)
		printf("%8lu", chars);
	printf(" %s\n", name);
}