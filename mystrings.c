#include <stdio.h>

typedef enum {false, true} bool;

int main(int argc, char *argv[], char *envp[]){
	char str[5] = {0,0,0,0,0}; int i = 0;
	int c = 0;
	while((c = getchar()) != EOF){
		// check if printable
		if((c == 9) || (32 <= c && c <= 126)){
			str[i] = c;
			i++;
			if(i > 3){
				i = 0;
				printf("%s", str);
				while((c = getchar()) != EOF){
					// check if printable
					if((c == 9) || (32 <= c && c <= 126)){
						printf("%c", c);
					}else{
						break;
					}
				}
				printf("\n");
				if(c == EOF) goto FINISH;
			}
		}else{
			i = 0;
		}
	}

	FINISH:
	return argc;
}