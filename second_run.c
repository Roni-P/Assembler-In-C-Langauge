#include "symbol_table.h"
#include "base32.h"
#include "first_run.h"
#include "second_run.h"

/* this function deploys the algorithm of the second run of the assembler */
int second_run(FILE *pf, char *file_name, Symbol *head, char data_image[156][11], char instruction_image[156][11], char entry_image[156][11], char external_image[156][11], int *p_data_counter, int *p_instruction_counter, int *p_indx_in_entry_image, int *p_indx_in_external_image, int arr_of_l[])
{
  /*
  variables explained:
  line - the string that every line from the files will be copied to
  field_name - the fields of the line will be copied to this string
  line_num - number of the line
  found_errors - if there are errors in the line, it will be one, else it will be zero
  indx_in_arr_of_l - index in array of L's
  indx_in_ic - temporal index in IC
  indx_in_line - index in the line string
  indx_in_field - index in the field string
  */
  char line[81], field_name[81];
  int line_num = 0, found_errors = 0, indx_in_arr_of_l = 0, indx_in_ic = 0;
  int indx_in_line, indx_in_field, i, j, symbol_address, binary_digit, indx_in_conversion;
  Symbol *tmp;
  
  /* 2: read the next line of the file */
  while( fgets(line, 81, pf) != NULL )
  {
    line_num++;
    indx_in_line = 0;
    indx_in_field = 0;
    clear_string(field_name);
    tmp = head;
    
    /* if line is blank or comment, continue to next line */
    if( is_empty_line(line) )
      continue;
    
    if( arr_of_l[indx_in_arr_of_l] == line_num )
    {
      indx_in_arr_of_l++;
      /* 7: complete the missing coding */
      /* run through the instruction image */
      for(i=0; i<arr_of_l[indx_in_arr_of_l]; i++)
      {
        /* check if the cell is filled with a symbol name */
        if( instruction_image[indx_in_ic][0] != '0' && instruction_image[indx_in_ic][0] != '1' )
        {
          /* check if the symbol is in the symbol table */
          if( (j = search(head, instruction_image[indx_in_ic])) )
          {
            tmp = head;
            while( j > 1 )
            {
              tmp = tmp->next;
              j--;
            }
            
            clear_string( instruction_image[indx_in_ic] );
            
            /* if the symbol is extern, it should be 00000000-01 */
            if( tmp->are == 'E' )
            {
              strcpy(external_image[(*p_indx_in_external_image)++], tmp->name);
              strcpy(external_image[(*p_indx_in_external_image)++], decAddressToBase32(indx_in_ic + 100));
              strcpy(instruction_image[indx_in_ic], "0000000001");
            }
            else /* else, convert the symbol address to binary with 10 at the end */
            {
              symbol_address = tmp->address;
              binary_digit = 0;
              indx_in_conversion = 7;
			        strcpy(instruction_image[indx_in_ic],"0000000010");
			        while( symbol_address != 0 )
			        {
			          binary_digit = symbol_address % 2;
			          instruction_image[indx_in_ic][indx_in_conversion] = binary_digit + '0';
			          symbol_address /= 2;
			          indx_in_conversion--;
			        }
            }
          }
          else
          {
            /* Error: symbol does not exist in the symbol table */
            printf("\tError in line %d: symbol \"%s\" does not exist in the symbol table\n", line_num, instruction_image[indx_in_ic]);
            found_errors = 1;
          }
        }
        
        indx_in_ic++;
      }
      
      indx_in_arr_of_l++;
      continue;
    }
    
    get_first_field(line, field_name, &indx_in_line, &indx_in_field);
    
    /* 3: next field is a symbol */
    if( line[indx_in_line] == ':' )
    {
      indx_in_line++;
      clear_string(field_name); /* clear the field string */
      indx_in_field = 0;
      /* 3: skip the symbol */
      second_get_second_field(line, field_name, &indx_in_line, &indx_in_field);
    }
    
    /* if it's a .entry instruction */
    if( strcmp(field_name, ".entry") == 0 )
    {
      /* get the symbol into the field_name string */
      clear_string(field_name); /* clear the field string */
      indx_in_field = 0;
      second_get_second_field(line, field_name, &indx_in_line, &indx_in_field);
      
      /* mark it as entry in symbol table */
      if( (j = search_entry(head, field_name)) )
      {
        while( j > 1 )
        {
          tmp = tmp->next;
          j--;
        }
        
        strcpy( entry_image[(*p_indx_in_entry_image)++], tmp->name );
        strcpy( entry_image[(*p_indx_in_entry_image)++], decAddressToBase32(tmp->address) );
      }
      else
      {
        /* Error: symbol after .entry does not exist in the symbol table */
        printf("\tError in line %d: symbol after .entry does not exist in the symbol table as needed\n", line_num);
        found_errors = 1;
      }
    }
  }
  
  /* 9: if found errors in second run, stop */
  if(found_errors == 1)
    return 0;
      
  
  /* 11: open and write the machine code file */
  convertToBase32(file_name, data_image, instruction_image, p_data_counter, p_instruction_counter); /*.ob file*/
  
  /* 11: open and write the external symbols file */
  if( (*p_indx_in_external_image) != 0 )
    make_externals_file(file_name, external_image, *p_indx_in_external_image);
  
  /* 11: open and write the entries file */
  if( (*p_indx_in_entry_image) != 0 )
    make_entry_file(file_name, entry_image, *p_indx_in_entry_image);
    
  return 1;
}

/* this second run modified function copies to string field_name the second field in the line */
void second_get_second_field(char *line, char *field_name, int *p_indx_in_line, int *p_indx_in_field)
{
  /* skip white spaces */
  while( *(line + *p_indx_in_line) != '\0' && *(line + *p_indx_in_line) != '\n' && ( *(line + *p_indx_in_line) == ' ' || *(line + *p_indx_in_line) == '\t' ) )
    (*p_indx_in_line)++;
      
  /* run through the line until found seperator for second field */
  while( *(line + *p_indx_in_line) != '\0' && *(line + *p_indx_in_line) != '\n' && *(line + *p_indx_in_line) != ' ' && *(line + *p_indx_in_line) != '\t')
  {
    *(field_name + *p_indx_in_field) = *(line + *p_indx_in_line);
    (*p_indx_in_line)++;
    (*p_indx_in_field)++;
  }
    
  *(field_name + *p_indx_in_field) = '\0';
}
