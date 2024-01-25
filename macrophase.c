#include "macrophase.h"
#include "Functions.h"

/*
Function that get a file pointer and his name.
checks if the file contains a macros in it , 
switching every macro, checking if the initialize of the macro is ok.
create a new ".am" file with the new lines.
*/
char* checkFile(FILE *file,char *fileName)
{
	int macroFlag = 0,lineNum = 1,errors = 0; /*macroFlag - to know if found macro, lineNum - line number, errors - to know if found errors*/
	char *macroFound,*newName,name[81],line[81]; /*macroFound - save the place of the found macro,newName - new name for file,name - macro name,line - the given line*/
  	FILE *fh_input,*fh_output; /*fh_input - the given file,fh_output - the new .am file*/
	struct macroTable* head; /*head - macro table*/
	head = (struct macroTable*)malloc(sizeof(struct macroTable));
	newName = (char*)malloc(sizeof(char)*(strlen(fileName)+4));
  	fh_input = file;

	strcpy(newName,fileName);
	strcat(newName,".am");
  	fh_output = fopen(newName,"w"); /*creates the new .am file*/
	
	while(fgets(line,81,fh_input) != NULL)	/*run through every line in the file*/
	{
		if((macroFound=strstr(line,"macro"))) /*if found "macro" in the line*/
		{
			if(!checkMacroName(line)) /*if the macro initialize have an error in it*/
			{
				printf("ERROR With Macro Name (line:%d)\n",lineNum);
				errors = 1;
				while(!strstr(line,"endmacro"))
				{
					fgets(line,81,fh_input);
					lineNum++;
				}
				lineNum++;
				continue;
			}

			getMacroName(name,line,macroFound); /*get macro name from the line*/
			
			if(isSavedName(name))
			{
				printf("ERROR With Macro Name (line:%d)\n",lineNum);
				errors = 1;
				while(!strstr(line,"endmacro"))
				{
					fgets(line,81,fh_input);
					lineNum++;
				}
				lineNum++;
				continue;
			}
				
			if(existMacro(head,name,macroFlag)) /*if the macro already exist or the macro name is saved name*/
			{
				printf("ERROR , Macro already exist (line:%d)\n",lineNum);
				while(!strstr(line,"endmacro"))
				{
					fgets(line,81,fh_input);
					lineNum++;
				}
				lineNum++;
				errors = 1;
				continue;
			}
			else
			{
				macroFlag = addMacroName(head,name,macroFlag); /*add the macro name to the table*/
				lineNum += addMacroBlock(head,name,fh_input); /*add the macro block to the table*/
				
				strcpy(line,"");
			}
		}

		if(checkMacro(head,line) && !errors) /*if the line contains a macro name*/
		{
			changeMacro(head,line,fh_output); /*change the macro name to its block*/
			strcpy(line,"");
		}
		
		fputs(line,fh_output); /*put the line in the new file*/
		lineNum++;
	}

	fclose(fh_output);
	
	if(errors) /*if found errors*/
	{
		if(!macroFlag)
			free(head);
		else
		{
			free(macroFound);
			freeTable(head);
		}
		remove(newName); /*remove .am file*/
		free(newName);
		strcpy(fileName,"Error");
		return fileName; /*return "Error"*/
	}

	fclose(fh_input);

	if(!macroFlag) /*if there is no macro*/
	{
		free(head);
		remove(newName);
		free(newName);
		strcat(fileName,".as");
		return fileName; /*return .as file name*/
	}
	else
	{
		free(macroFound);
		free(newName);
		freeTable(head);
		strcat(fileName,".am");
		return fileName; /*return .am file name*/
	}
}

/*
Function that get a line, checks if the macro initialize is ok.
return 1 if ok,else return 0.
*/
int checkMacroName(char *line)
{
	int j = 0;
	int i = 0;
	char macro[] = "macro";

	while(line[i] == '\t' || line[i] == ' ') /*going through the line until reaching a character*/
	{
		i++;
	}
			
	while(j<strlen(macro)) /*check if only "macro" exist*/
	{
		if(line[i] != macro[j])
			return 0;
		i++;
		j++;
	}

	if(line[i] != '\t' && line[i] != ' ') /*if there is no space between "macro" to the macro name*/
		return 0;

	while(line[i] == '\t' || line[i] == ' ') /*going through the line until reaching a character or quit if there is over 80 characters in the line*/
	{
		if(i>81)
			return 0;
		i++;
	}

	if(line[i] == '\n' || line[i] == '\0') /*if we reach the end of the line and there is no macro name*/
		return 0;

	while(!isspace(line[i])) /*going through the name of the macro or quit if there is over 80 characters in the line*/
	{
		if(i>81)
			return 0;
		i++;
	}

	if(line[i] == '\n' || line[i] == '\0') /*if we reach the end of the line*/
		return 1;

	while(line[i] == '\t' || line[i] == ' ')/*going through the line and check if there are only spaces and tabs*/
	{
		if(i>81)
			return 0;
		i++;
	}
	
	if(line[i] == '\n' || line[i] == '\0') /*if we reach the end of the line*/
		return 1;
	else
		return 0; /*if we reach another character the initialize of the macro is wrong*/
}

/*
Function that get a line and a pointer into the "macro" place.
return the name of the macro.
*/
void getMacroName(char name[81],char *line,char *p)
{
	int i = 0;

	p+=6;
			
	while(*p == ' ' || *p == '\t')
		p+=1;
	while(*p != ' ' && *p != '\t' && *p != '\n') /*run through the line and get the name of the macro*/
	{
		name[i] = *p;
		p+=1;
		i++;
	}
	name[i] = '\0';
}

/*
Function that get a pointer to the macro table and macro name.
checks if the macro exist in the table.
return 1 if yes,else return 0.
*/
int existMacro(struct macroTable *head, char name[81],int macroFlag)
{
	if(macroFlag == 0) /*if the table is empty*/
		return 0;
	else
	{
		while(head != NULL) /*run through the macro table and check if macro name exist*/
		{
			if(strcmp(head->macroName,name) == 0) /*if macro exist*/
				return 1;
			head = head->next;
		}
	}
	return 0;
}

/*
Function that get a pointer to the macro table and macro name.
adds the macro name to the table.
*/
int addMacroName(struct macroTable *head, char name[81],int macroFlag)
{
	if(macroFlag == 0) /*if macro table is empty*/
		strcpy(head->macroName,name);
	else
	{
		while(head->next !=NULL) /*run through the macro table*/
			head = head->next;
		head->next = (struct macroTable*)malloc(sizeof(struct macroTable));
		strcpy(head->next->macroName,name);
	}

	return 1;
}

/*
Function that get a pointer to the macro table, macro name and file pointer.
adds the macro block to the macro table.
return how many lines the block has.
*/
int addMacroBlock(struct macroTable *head, char name[81],FILE *input)
{
	int i = 0;
	char* line;
	line = (char*)malloc(sizeof(char)*81);

	while(strcmp(head->macroName,name) != 0) /*run through the macro table until reaching the macro name*/
	{
		head = head->next;
	}

	fgets(line,81,input); /*get the next line in the file*/

	while(!strstr(line,"endmacro"))/*run through the file, place every line in the table until reaching "endmacro"*/
	{
		head->macroBlock[i] = (char*)malloc(sizeof(char)*81);
		strcpy(head->macroBlock[i],line);
		fgets(line,81,input);
		i++;
	}

	free(line);
	return (i+1);
}

/*
Function that get macro table and a line.
checks if the line contains a macro name.
return 1 if yes,else return 0;
*/
int checkMacro(struct macroTable *head, char *line)
{
	int i = 0;
	int j = 0;

	if(head->macroName == NULL) /*if there is no macro*/
		return 0;

	while(head != NULL) /*run through the macro table*/
	{
		if(strstr(line,head->macroName)) /*if found a macro name in the line*/
		{
			while(line[i] == '\t' || line[i] == ' ') /*run through the line until reaching a character*/
			{
				i++;
			}
		
			while(j<strlen(head->macroName)) /*compare the macro name*/
			{
				if(line[i] != head->macroName[j]) /*if the name is not the same*/
					return 0;
				i++;
				j++;
			}
			
			if(line[i] == '\0' || line[i] == '\n') /*if reach the end of the line*/
				return 1;
			if(line[i] != '\t' && line[i] != ' ') /*if there is a character after the macro name*/
				return 0;

			while(line[i] == '\t' || line[i] == ' ') /*run through the line until reaching a character*/
			{
				if(line[i+1] == '\0' || line[i+1] == '\n' || i == 80) /*if reach the end of the line or over 80 characters*/
					return 1;
				i++;
			}

			if(line[i] != '\t' && line[i] != ' ') /*if there is a character after the macro name*/
				return 0;
		}
		head = head->next;
	}
	return 0;
}

/*
Function that gets a macro table , line and a file pointer.
checks which macro is in the line and swap him with the macro block.
*/
void changeMacro(struct macroTable *head, char *line,FILE *output)
{
	int i = 0;

	while(!(strstr(line,head->macroName))) /*run through the macro table ubtil reaching the currect macro*/
	{
		head = head->next;
	}

	while(head->macroBlock[i] != NULL) /*change the macro line into his block*/
	{
		fputs(head->macroBlock[i],output);
		i++;
	}
}

/*
Function that free the macro table.
*/
void freeTable(struct macroTable *head)
{
	int i;
	struct macroTable *first;
	
	if(head->next != NULL)
	{
		first = head;
		for(i = 0;i<=256 && head->macroBlock[i] != NULL;i++)
			free(head->macroBlock[i]);
		head = head->next;
		free(first);
	}
	for(i = 0;i<=256 && head->macroBlock[i] != NULL;i++)
		free(head->macroBlock[i]);
	free(head);
}
