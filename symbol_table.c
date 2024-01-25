#include "symbol_table.h"

/* This function creates a new symbol and returns a pointer to it */
Symbol *new_symbol(char *name, int address, int is_inst_or_guide, char are)
{
  Symbol *tmp = NULL;
  tmp = (Symbol *)malloc(sizeof(Symbol));
  if(tmp != NULL)
  {
    strcpy(tmp->name,name);
    tmp->address = address;
    tmp->is_inst_or_guide = is_inst_or_guide;
    tmp->are = are;
    tmp->next = NULL;
  }
  return tmp;
}

/* This function deletes a symbol table */
void destroy_table(Symbol *head)
{
  if(head == NULL)
    return;
  
  while(head != NULL)
  {
    Symbol *tmp = head;
    head = head->next;
    free(tmp);
  }
}

/* This function prints a symbol table in the consule */
void print(Symbol *head)
{
  while(head != NULL)
  {
    printf("Name: %s\nAddress: %d\nInst Or Guide: %d\nARE: %c\n\n", head->name, head->address, head->is_inst_or_guide, head->are);
    head = head->next;
  }
}

/* 
This function searches a symbol table 
for a given symbol name.
If the symbol is found it returns the symbol's place in the table.
If the symbol is not found it returns 0.
*/
int search(Symbol *head, char *name)
{
  int pos = 1;
  
  while(head != NULL)
  {
    if(strcmp(head->name, name) == 0)
      return pos;
    head = head->next;
    pos++;
  }
  
  return 0;
}

/* 
This function searches a symbol table 
for a given symbol name that is defined as entry.
If the symbol is found it changes ARE to 'e' and returns the symbol's place in the table.
If the symbol is not found it returns 0.
*/
int search_entry(Symbol *head, char *name)
{
  int pos = 1;
  
  while(head != NULL)
  {
    if(strcmp(head->name, name) == 0)
    {
      if( head->are == 'e' || head->are == 'E' )
        return 0;
      head->are = 'e';
      return pos;
    }
    head = head->next;
    pos++;
  }
  
  return 0;
}

/* This function returns the length of a symbol table */
int length(Symbol *head)
{
  int length = 0;
  while(head != NULL)
  {
    length+=1;
    head = head->next;
  }
  return length;
}

/* This function adds a given symbol to the end of a given symbol table  */
void add_symbol(Symbol *head, char *name, int address, int is_inst_or_guide, char are)
{
  Symbol *tmp = head;
  
  while(tmp->next != NULL)
    tmp = tmp->next;
    
  tmp->next = new_symbol(name, address, is_inst_or_guide, are);
}

/* 
This function removes the last symbol of a given symbol table
IF THE SYMBOL TABLE HAS ONE OR ZERO SYMBOLS, USE destroy_table
 */
void remove_last_symbol(Symbol *head)
{
  Symbol *tmp = head;
  while(tmp->next->next != NULL)
    tmp = tmp->next;
  free(tmp->next);
  tmp->next = NULL;
}
