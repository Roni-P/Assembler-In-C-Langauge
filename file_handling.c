#include "file_handling.h"

/*
this function gets an array that contains the FULL names
of the files that need to be opened and returns
a pointer to an array of pointers to files
*/
FILE **open_files(int argc, char *argv[])
{
  int i;
  
  FILE **files = (FILE **)malloc(sizeof(FILE *) * (argc-1));
  
  for(i=1; i<argc; i++)
  {
    char *tmp = (char *)malloc(strlen(argv[i])+3);
    strcpy(tmp, argv[i]);
    strcat(tmp, ".as");
    *(files+i-1) = fopen(tmp, "r");
    free(tmp);
  }
  
  return files;
}

/*
This function close all opened files
*/
void close_files(FILE **files, int argc)
{
  int i;
  
  for(i=0; i<argc-1; i++)
  {
    if(*(files+i) != NULL)
      fclose(*(files+i));
  }
  
  free(files);
}
