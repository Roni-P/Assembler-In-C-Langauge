#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_handling.h"
#include "symbol_table.h"
#include "macrophase.h"
#include "first_run.h"
#include "second_run.h"
#include "base32.h"
#include "Functions.h"
#include "binaryData.h"
#include "binaryInst.h"

int main(int argc, char *argv[])
{
  int i, j, arr_of_l[256], data_counter = 0, instruction_counter = 0, indx_in_entry_image = 0, indx_in_external_image = 0, indx_in_arr_of_l = 0;
  char data_image[156][11], instruction_image[156][11], entry_image[156][11], external_image[156][11], file_name[81];
  FILE **files = NULL;
  Symbol *head;
  
  files = open_files(argc, argv);
  
  for(i=0; i<argc-1; i++)
  {
    for(j=0; j<data_counter; j++)
      clear_string(data_image[j]);
      
    for(j=0; j<instruction_counter; j++)
      clear_string(instruction_image[j]);
      
    for(j=0; j<indx_in_arr_of_l; j++)
      arr_of_l[j] = 0;
    
    for(j=0; j<indx_in_entry_image; j++)
      clear_string(entry_image[j]);
      
    for(j=0; j<indx_in_external_image; j++)
      clear_string(external_image[j]);
      
    data_counter = 0;
    instruction_counter = 0;
    indx_in_arr_of_l = 0;
    indx_in_entry_image = 0;
    indx_in_external_image = 0;
    
    destroy_table(head);
    head = new_symbol("!!!", -200, 1, 'A');
    strcpy(file_name,argv[i+1]);
    printf("File %s.as:\n", argv[i+1]);
    
    if(*(files+i) != NULL)
    {
	  rewind(*(files+i));
    /*
    pre-assembler phase
    */
	if(strcmp(checkFile(*(files+i),file_name),"Error"))
		*(files+i) = fopen(file_name,"r"); /*.am file (if have macro)*/
	else
		continue;
    /*
    first run of the assembler
    */
	if(first_run(*(files+i), head, data_image, instruction_image, &data_counter, &instruction_counter, arr_of_l, &indx_in_arr_of_l))
	{
	  /*
      second run of the assembler
      */
      rewind(*(files+i));
	  if( second_run(*(files+i), file_name, head, data_image, instruction_image, entry_image, external_image ,&data_counter, &instruction_counter, &indx_in_entry_image, &indx_in_external_image, arr_of_l) )
	      printf("\tSuccess!\n");
	   }
    }
    else
      printf("\tError: file %s.as does not exist\n", argv[i+1]);
  }
  
  destroy_table(head);
  close_files(files, argc);
  
  return 1;
}
