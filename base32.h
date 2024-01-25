#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void convertToBase32(char*,char [156][11],char [156][11],int*,int*);

char* decAddressToBase32(int);
char* numToBase32(char*);

int convertToDecimal(int);
void make_externals_file(char *, char [156][11], int );
void make_entry_file(char *, char [156][11], int );
