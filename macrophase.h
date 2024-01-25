#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct macroTable 
{
	char macroName[81];
	char *macroBlock[256];
	struct macroTable* next;
};

char* checkFile(FILE *,char *);
void getMacroName(char [81],char *,char *);
int checkMacroName(char *);
int isSavedName(char*);
int existMacro(struct macroTable*, char [81],int);
int checkMacro(struct macroTable *, char *);
void changeMacro(struct macroTable *, char *,FILE *);
int addMacroName(struct macroTable *, char [81],int);
int addMacroBlock(struct macroTable *, char [81],FILE *);
void freeTable(struct macroTable *);
