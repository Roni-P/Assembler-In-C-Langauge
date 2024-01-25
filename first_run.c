#include "symbol_table.h"
#include "first_run.h"
#include "Functions.h"
#include "binaryData.h"
#include "binaryInst.h"

/* this function deploys the algorithm of the first run of the assembler */
int first_run(FILE *pf, Symbol *head, char data_image[156][11], char instruction_image[156][11], int *p_data_counter, int *p_instruction_counter, int arr_of_l[], int *p_indx_in_arr_of_l)
{
  /*
  variables explained:
  line - the string that every line from the files will be copied to
  field_name - the fields of the line will be copied to this string
  symbol_name - if a symbol is declared in a line, it's name will be copied to this string
  op_name - the operation name in the line will be copied to this string
  operands - an array that stores the operands of the operation in each line
  line_num - number of the line
  found_errors - if there are errors in the line, it will be one, else it will be zero
  total_errors - counts the total lines with errors
  tmp_ic - temporal index in IC
  indx_in_line - index in the line string
  indx_in_field - index in the field string
  indx_in_op - index in the operands array
  symbol_declaration - if a symbol is declared in a line, it will be one, else it will be zero
  */
  char line[81], field_name[81], symbol_name[31], op_name[81], operands[81][81];
  int line_num = 0, found_errors = 0, total_errors = 0, tmp_ic = 0, i;
  int indx_in_line, indx_in_field, indx_in_op, symbol_declaration;
  Symbol *tmp;
  
  while( fgets(line, 81, pf) != NULL )
  {
    symbol_declaration = 0;
    indx_in_line = 0;
    indx_in_field = 0;
    total_errors += found_errors;
    found_errors = 0;
    line_num++;
    clear_string(field_name);
    clear_string(symbol_name);
    while( indx_in_op != 0 )
	  {
      clear_string(operands[indx_in_op-1]);
	    indx_in_op--;
	  }
    /* if line is blank or comment, continue to next line */
    if( is_empty_line(line) )
      continue;
    
    get_first_field(line, field_name, &indx_in_line, &indx_in_field);
    
    /* 3: next field is a symbol */
    if( line[indx_in_line] == ':' )
    {
      symbol_declaration = 1; /* 4: turn on "symbol declaration" flag */
      /* check if the symbol's name is valid */
      if( !is_symbol_name_valid(field_name) )
      {
        printf("\tError in line %d: the symbol name is not valid\n", line_num);
        found_errors = 1;
        continue;
      }
      indx_in_line++;
      
      /* check if there is blank space after ':' */
      if( line[indx_in_line] != ' ' && line[indx_in_line] != '\t' )
      {
        printf("\tError in line %d: there is no blank space after colon\n", line_num);
        found_errors = 1;
        continue;
      }
      
      strcpy(symbol_name, field_name); /* save the symbol name */
      clear_string(field_name); /* clear the field string */
      indx_in_field = 0;
    }
    
    /* if "symbol declaration" flag is on */
    /* read the next field */
    if( symbol_declaration )
    {
      if( !get_second_field(line, field_name, &indx_in_line, &indx_in_field, &line_num, &found_errors) )
        continue;
    }
    
    strcpy(op_name, field_name); /* save the field name */
    clear_string(field_name); /* clear the field string */
    indx_in_field = 0;
    
    /* get operands */
    if( !get_operands(line, field_name, operands, &indx_in_line, &indx_in_field, &indx_in_op, &line_num, &found_errors) )
      continue;
    
    /* run through array of operands and check for invalid ',' like: ",," and so on */
    for(i=0; i<indx_in_op; i++)
    {
      if( i % 2 == 1 && strcmp(operands[i], ",") != 0 )
      {
        printf("\tError in line %d: operands are not valid\n", line_num);
        found_errors = 1;
        break;
      }
    }
    
    if( found_errors )
      continue;
	   
    /* 5: the field is a .data or .string or .struct instruction */
    if( strcmp(op_name, ".data") == 0 || strcmp(op_name, ".string") == 0 || strcmp(op_name, ".struct") == 0)
    {
      /* 6: if "symbol declaration" flag is on */
      if( symbol_declaration )
      {
        /* 6: the symbol is NOT in the symbol table */
        if( !search(head, symbol_name) )
        {
          /* 6: add the symbol to the symbol table */
          add_symbol(head, symbol_name, *p_data_counter, 1, 'R');
        }
        else
        {
          /* 6: Error: the symbol had already been declared */
          printf("\tError in line %d: the symbol had already been declared\n", line_num);
          found_errors = 1;
          continue;
        }
      }
      
      
      /* 7: recognize the data type */
      /* convert the data to binary */
      /* and increment data_counter */
      
      if( strcmp(op_name, ".data") == 0 )
      {
		  if(checkData(operands,indx_in_op))
	      	dataToBinary(data_image,p_data_counter,operands,indx_in_op);
		  else
		  {
      		printf("\tError in line %d: the given operands for .data are not valid\n", line_num);
      		found_errors = 1;
    	  }
      }
      
      if( strcmp(op_name, ".string") == 0 )
      {
		  if(checkStr(operands[0],indx_in_op))
	      	strToBinary(data_image,p_data_counter,operands[0]);
		  else
		  {
      		printf("\tError in line %d: the given operands for .string are not valid\n", line_num);
      		found_errors = 1;
    	  }
      }
      
      if( strcmp(op_name, ".struct") == 0 )
      {
		  if(checkStruct(operands,indx_in_op))
	      	structToBinary(data_image,p_data_counter,operands);
		  else
		  {
      		printf("\tError in line %d: the given operands for .struct are not valid\n", line_num);
      		found_errors = 1;
    	  }
      }
      
      continue;
    }
    
    /* 8: this is a .extern or .entry instruction */
    if( strcmp(op_name, ".extern") == 0 || strcmp(op_name, ".entry") == 0 )
    {
      /* if there is zero or more than one operands to .extern/.entry, it's an error */
      if( indx_in_op != 1 )
      {
        /* Error: the number of operands does not match the operation */
        printf("\tError in line %d: the number of operands does not match the operation\n", line_num);
        found_errors = 1;
        continue;
      } 
      
      /* 9: this is a .extern isntruction */
      if( strcmp(op_name, ".extern") == 0 )
      { 
        /* 9: the symbol is NOT in the symbol table */
        if( !search(head, operands[0]) )
        {
          /* 9: add the symbol to the symbol table with is_ext_or_int = 0 */
          if( !is_symbol_name_valid(operands[0]) )
          {
            printf("\tError in line %d: the symbol name is not valid\n", line_num);
            found_errors = 1;
            continue;
          }
          add_symbol(head, operands[0], -100, 0, 'E');
        }
        else
        {
          /* 9: Error: the symbol had already been declared */
          printf("\tError in line %d: the symbol had already been declared\n", line_num);
          found_errors = 1;
        }
      }
      continue;
    }
    
    /* 11: if "symbol declaration" flag is on */
    if( symbol_declaration )
    {
      /* 11: if the symbol is NOT in the symbol table */
      if( !search(head, symbol_name) )
      {
        /* 11: add the symbol to the symbol table */
        add_symbol(head, symbol_name, *p_instruction_counter, 0, 'A');
      }
      else
      {
        /* 11: Error: the symbol had already been declared */
        printf("\tError in line %d: the symbol had already been declared\n", line_num);
        found_errors = 1;
        continue;
      }
    }
    
    /* 12: if operation is in the operation table */
    if(checkInst(op_name) != -1)
    {
	    if(checkValidOp(op_name,operands,indx_in_op))
	    {
	      tmp_ic = *p_instruction_counter;
		    /* 13: convert to binary */
		    convertInstToBinary(instruction_image,p_instruction_counter,op_name,operands);
		    arr_of_l[*p_indx_in_arr_of_l] = line_num;
		    arr_of_l[++(*p_indx_in_arr_of_l)] = (*p_instruction_counter) - tmp_ic;
		    (*p_indx_in_arr_of_l)++;
	    }
		  else
		  {
        printf("\tError in line %d: the given operands are not valid\n", line_num);
      	found_errors = 1;
    	}
    }
    else
    {
      /* 12: Error: the operation is unknown */
      printf("\tError in line %d: the operation is unknown\n", line_num);
      found_errors = 1;
    }
  }
  
  /* 16: if found errors in the code */
  if( total_errors )
    return 0;
  
  /* 
  17: increment the address of symbols of the type data by instruction_counter
  */
  tmp = head;
  while( tmp != NULL )
  {
    if( tmp->is_inst_or_guide )
      tmp->address += *p_instruction_counter;
      
    tmp->address += 100;
    tmp = tmp->next;
  }

  return 1;
}

/* this function reads the first field of the line into given string field name */
void get_first_field(char *line, char *field_name, int *p_indx_in_line, int *p_indx_in_field)
{
  /* skip white spaces */
  while( *(line + *p_indx_in_line) == ' ' || *(line + *p_indx_in_line) == '\t' )
    (*p_indx_in_line)++;
    
  /* run through the line until found seperator for first field */
  while( *(line + *p_indx_in_line) != '\0' && *(line + *p_indx_in_line) != '\n' && *(line + *p_indx_in_line) != ' ' && *(line + *p_indx_in_line) != '\t' && *(line + *p_indx_in_line) != ':' )
  {
    *(field_name + *p_indx_in_field) = *(line + *p_indx_in_line);
    (*p_indx_in_line)++;
    (*p_indx_in_field)++;
  }
    
  *(field_name + *p_indx_in_field) = '\0';
}

/* this function reads the second field of the line into given string field name */
int get_second_field(char *line, char *field_name, int *p_indx_in_line, int *p_indx_in_field, int *p_line_num, int *p_found_errors)
{
  /* skip white spaces */
  while( *(line + *p_indx_in_line) != '\0' && ( *(line + *p_indx_in_line) == ' ' || *(line + *p_indx_in_line) == '\t' ) )
    (*p_indx_in_line)++;
      
  if( *(line + *p_indx_in_line) == '\0' )
  {
    printf("\tError in line %d: there is no operation writen\n", *p_line_num);
    *p_found_errors = 1;
    return 0;
  }
      
  /* run through the line until found seperator for second field */
  while( *(line + *p_indx_in_line) != '\0' && *(line + *p_indx_in_line) != ' ' && *(line + *p_indx_in_line) != '\t')
  {
    *(field_name + *p_indx_in_field) = *(line + *p_indx_in_line);
    (*p_indx_in_line)++;
    (*p_indx_in_field)++;
  }
    
  *(field_name + *p_indx_in_field) = '\0';
  
  return 1;
}

/* this function reads the operands of the line into given array of strings */
int get_operands(char *line, char *field_name, char operands[81][81], int *p_indx_in_line, int *p_indx_in_field, int *p_indx_in_op, int *p_line_num, int *p_found_errors)
{
  /* skip white spaces */
  while( *(line + *p_indx_in_line) != '\0' && *(line + *p_indx_in_line) != '\n' && (*(line + *p_indx_in_line) == ' ' || *(line + *p_indx_in_line) == '\t') )
    (*p_indx_in_line)++;
    
  /* if found ',' without operand before it */ 
  if( *(line + *p_indx_in_line) == ',' )
  {
    printf("\tError in line %d: there is no operands before ','\n", *p_line_num);
    *p_found_errors = 1;
    return 0;
  }
   
   
  /* get operands */
  while( *(line + *p_indx_in_line) != '\0' ) 
  {
      
    /* if found start of string run through line until end or '"' */
    if( *(line + *p_indx_in_line) == '"' )
    {
      *(field_name + *p_indx_in_field) = *(line + *p_indx_in_line);
      *(line + *p_indx_in_line) = '\0';
      (*p_indx_in_line)++;
      (*p_indx_in_field)++;
        
      while( *(line + *p_indx_in_line) != '\0' && *(line + *p_indx_in_line) != '\n' ) 
      {
        *(field_name + *p_indx_in_field) = *(line + *p_indx_in_line);
        (*p_indx_in_line)++;
        (*p_indx_in_field)++;
      }
      
      (*p_indx_in_line)--;
      
      while( *(line + *p_indx_in_line) != '\0' && *(line + *p_indx_in_line) != '"' )
      {
        *(field_name + *p_indx_in_field) = '\0';
        (*p_indx_in_line)--;
        (*p_indx_in_field)--;
      }
      
      if( *(line + *p_indx_in_line - 1) == '\0' )
      {
        printf("\tError in line %d: there is no end to str\n", *p_line_num);
        *p_found_errors = 1;
        break;
      }
        
      *(field_name + *p_indx_in_field) = '\0';
      strcpy(*(operands + *p_indx_in_op), field_name); /* save the field name */
      (*p_indx_in_op)++;
      clear_string(field_name); /* clear the field string */
      *p_indx_in_field = 0;
    }
    /* if not string run through line until found seperators */
    else
    {
      while( *(line + *p_indx_in_line) != '\0' && *(line + *p_indx_in_line) == ',' && *(line + *p_indx_in_line) != ' ' && *(line + *p_indx_in_line) != '\t' && *(line + *p_indx_in_line) != '\n' ) 
      {
        *(field_name + *p_indx_in_field) = *(line + *p_indx_in_line);
        (*p_indx_in_line)++;
        (*p_indx_in_field)++;
      }
        
      if( *p_indx_in_field != 0 ) 
      {
        *(field_name + *p_indx_in_field) = '\0';
        strcpy(*(operands + *p_indx_in_op), field_name); /* save the field name */
        (*p_indx_in_op)++;
        clear_string(field_name); /* clear the field string */
        *p_indx_in_field = 0;
      }
        
      while( *(line + *p_indx_in_line) != '\0' && *(line + *p_indx_in_line) != ',' && *(line + *p_indx_in_line) != ' ' && *(line + *p_indx_in_line) != '\t' && *(line + *p_indx_in_line) != '\n' )
      {
        *(field_name + *p_indx_in_field) = *(line + *p_indx_in_line);
        (*p_indx_in_line)++;
        (*p_indx_in_field)++;
      }
        
      if( *p_indx_in_field != 0 ) 
      {
        *(field_name + *p_indx_in_field) = '\0';
        strcpy(*(operands + *p_indx_in_op), field_name); /* save the field name */
        (*p_indx_in_op)++;
        clear_string(field_name); /* clear the field string */
        *p_indx_in_field = 0;
      }
    }
    if( *(line + *p_indx_in_line) != ',' ) 
      (*p_indx_in_line)++;
  }
    
  if(*p_found_errors)
    return 0;
  
  return 1;
}

/*
This Function checks if the line is empty or start with ';'.
return 1 if yes.(means we should ignore this line).
*/
int is_empty_line(char *line)
{
	int i = 0;
	while(i<strlen(line))
	{
		if(line[i] == '\n' || line[i] == '\0')
			return 1;
		if(line[i] == ';')
			return 1;
		if(line[i] != ' ' && line[i] != '\t')
			return 0;
		i++;
	}

	if(line[i] == '\n' || line[i] == '\0')
		return 1;
	else return 0;
}

/* This function checks if the given symbol name is valid */
int is_symbol_name_valid(char *symbol_name)
{
  int i;
  
  /* 
  if the first char is not a letter or length is over 30 
  or it's a saved name, it's not a valid name, return 0
  */
  if( ( !isalpha(*symbol_name) ) || strlen(symbol_name) > 30 || isSavedName(symbol_name) )
    return 0;
  
  /* 
  scan the name and check if every character is a letter or a number 
  if it is not a not alphanumeric, return 0
  */
  for(i=0; i<strlen(symbol_name); i++)
    if( !isalnum(*(symbol_name+i)) )
      return 0;
  
  /* the conditions are all met, the name is valid, return 1 */
  return 1;
}

/* This function resets a string back to blank string */
void clear_string(char *str)
{
  int i;
  
  for(i=0; i<strlen(str); i++)
    *(str+i) = '\0';
}
