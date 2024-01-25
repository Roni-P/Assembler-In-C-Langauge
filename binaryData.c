#include "binaryData.h"
#include "Functions.h"

/*
The function converts the given string into binary base.
*/
void strToBinary(char data_image[156][11],int *data_counter,char *str)
{
	int j,k,i = 1; /*j - run through bits,k - run through the string in data image,i - run through the given string*/
	
	while(i<(strlen(str)-1)) /*run through the given string*/
	{
		
		k = 0;
		strcpy(data_image[*data_counter],"0000000000");

		for (j = 1 << 9; j > 0; j = j>>1)
    	{
			if(str[i] & j)
				data_image[*data_counter][k] = '1';
			else
				data_image[*data_counter][k] = '0';
			k++;
		}

		(*data_counter)++;
		i++;
	}
	strcpy(data_image[*data_counter],"0000000000"); /*place "0000000000" in the end of the string*/
	(*data_counter)++;
}

/*
The function converts the given data into binary base.
*/
void dataToBinary(char data_image[156][11],int *data_counter,char data[81][81],int size)
{
	int j,k,i = 0; /*j - run through bits,k - run through the string in data image,i - run through the given data array*/
	
	while(i<size)
	{
		if(strcmp(data[i],","))
		{
			k = 0;
			strcpy(data_image[*data_counter],"0000000000");
			for (j = 1 << 9; j > 0; j = j>>1)
    		{
				if(atoi(data[i]) & j)
					data_image[*data_counter][k] = '1';
				else
					data_image[*data_counter][k] = '0';
				k++;
			}
			(*data_counter)++;
		}
		i++;
	}
	
}

/*
The function converts the given struct into binary base.
*/
void structToBinary(char data_image[156][11],int *data_counter,char operands[81][81])
{
	dataToBinary(data_image,data_counter,operands,1); /*send the first field to data convertor*/
	strToBinary(data_image,data_counter,operands[2]); /*send the second field to string convertor*/
}

/*
The function checks if the given operands are valid for .data
*/
int checkData(char data[81][81],int size)
{
	int i; /*run through the given data*/
	if(size == 0 || (size % 2) == 0)
		return 0;
	for(i = 0;i<size;i++)
	{
		if((strcmp(data[i],",") != 0 && checkValidNum(data[i]) == 0) || strchr(data[i],'.'))
			return 0;
	}

	return 1;
}

/*
The function checks if the given operands are valid for .struct
*/
int checkStruct(char operands[81][81],int size)
{
	if(size > 3 || size == 0)
		return 0;
	
	if(checkValidNum(operands[0]) == 0 || strchr(operands[0],'.'))
		return 0;

	if(operands[2][0] != '"' && operands[2][strlen(operands[2])-1] != '"')
		return 0;

	return 1;
}

/*
The function checks if the given operands are valid for .string
*/
int checkStr(char *str,int size)
{
	if(size > 1 || size == 0)
		return 0;
	
	if(str[0] != '"' || str[strlen(str)-1] != '"' || str[strlen(str)] != '\0')
		return 0;

	return 1;
}
