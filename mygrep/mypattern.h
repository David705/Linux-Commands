struct row{
	char ascii[130];
	struct row *next;
	struct row *prev;
};

size_t regexSize;
char *regex;
struct row *head;
struct row *tail;

void retrieveRegex(char *regex_file);
void createTable();
void setAsciiToZero(struct row *row);
void freeMemory();
void printTable();
void printCharacters();