#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int first_run(FILE *,Symbol *, char[156][11],char[156][11],int*,int*,int[],int*);
void get_first_field(char *, char *, int *, int *);
int get_second_field(char *, char *, int *, int *, int *, int *);
int get_operands(char *, char *, char [81][81], int *, int *, int *, int *, int *);
int is_empty_line(char *);
int is_symbol_name_valid(char *);
void clear_string(char *);
