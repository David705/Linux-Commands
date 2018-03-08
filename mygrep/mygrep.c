/*

								mygrep Command Manual

NAME
	mygrep

SYNOPSIS
	mygrep [options] filename

DESCRIPTION
	The mygrep utility searches any given file and selects the lines that
	match the pattern provided. For any given file, it is important
	to include an empty line at the bottom of the file or the line will
	not be matched by mygrep. As for pattern matching, the following are
	characters that have special meaning:

		+		Matches one or more occurrences of a character
		*		Matches zero of more occurrences of a character
		.		Matches any character
		@		Indicates a hex value
		\		Indicates that you are escaping a character
		[		Indicates the starts of a set
		]		Indicates the end of a set
		-		When placed inside a set and in between two hex
				values, it represents a range
		^		When placed at the beginning of a set, it indicates that
				you are searching for the complement of the set

OPTIONS
	-f <regex_file>, --file-name <regex_file>
		provides the mygrep utility with the file containing your regex expression

AUTHOR
	Written by David Chavarria

*/

#include <stdio.h>
#include <stdlib.h>
#include "mystrings.h"
#include "mypattern.h"
#include "myfilter.h"

void usage(){

		printf("Usage:\n");
		printf("    mygrep -f <regex_file> <filename>\n");
		printf("    mygrep --file-name <regex_file> <filename>\n");
		exit(-1);

}

int main(int argc, char *argv[], char *envp[]){

	if(argc < 4)
		usage();

	char *regex_file = NULL;
	char *filename = NULL;

	if(strEq(argv[1], "-f") || strEq(argv[1], "--file-name")){
		regex_file = argv[2];
		filename = argv[3];
	}else{
		usage();
	}

	retrieveRegex(regex_file);

	createTable();

	grepFile(filename);

	freeMemory();

	return 0;

}