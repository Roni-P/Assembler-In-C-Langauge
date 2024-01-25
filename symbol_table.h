#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Symbol
{
  char name[80];
  int address;
  int is_inst_or_guide;
  char are;
  struct Symbol *next;
} Symbol;

Symbol *new_symbol(char *,int ,int ,char);
void destroy_table(Symbol *);
void print(Symbol *);
int search(Symbol *, char *);
int search_entry(Symbol *, char *);
int length(Symbol *);
void add_symbol(Symbol *, char*, int, int, char);
void remove_last_symbol(Symbol *);
