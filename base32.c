#include "base32.h"

/*
The function gets data array and instruction array of binary numbers converting them into base 32 and creating a file with all the words.
*/
void convertToBase32(char *name,char data_image[156][11],char inst_image[156][11],int *data_counter,int *inst_counter)
{
	int i = 0; /*index to run through the array*/
	FILE *fh_output; /*the new .ob file*/
	char *newName,*base32,*address; /*newName - new name for the file ,base32 - the converted base 32 word ,address - the address of the current word*/
	newName = strtok(name,".");
	strcat(newName,".ob");
	fh_output = fopen(newName,"w");
    
	fprintf(fh_output," %s %s\n",decAddressToBase32(*inst_counter),decAddressToBase32(*data_counter));
  
	while(i < (*inst_counter))
	{
		base32 = numToBase32(inst_image[i]); /*convert word to base 32*/
		address = decAddressToBase32(i+100); /*convert address of the word to base 32*/
		fprintf(fh_output,"%s	%s\n",address,base32); /*write into the new file the converted base 32 words*/
		i++;
		free(base32);
		free(address);
	}

	i = 0;
	
	while(i < (*data_counter))
	{
		base32 = numToBase32(data_image[i]); /*convert word to base 32*/
		address = decAddressToBase32((*inst_counter)+i+100); /*convert address of the word to base 32*/
		fprintf(fh_output,"%s	%s\n",decAddressToBase32((*inst_counter)+i+100),numToBase32(data_image[i])); /*write into the new file the converted base 32 words*/
		i++;
		free(base32);
		free(address);
	}

}

/*
The function gets a string of binary number and converting him into base 32.
*/
char* numToBase32(char* num)
{
	char *newNum,*num1,*num2;
	char base32Array[32] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};

	newNum = (char*)malloc(sizeof(char)*3);
	num1 = (char*)malloc(sizeof(char)*5);
	num2 = (char*)malloc(sizeof(char)*5);
	strncpy(num1,num,5);
	strncpy(num2,num+5,5);

	newNum[0] = base32Array[convertToDecimal(atoi(num1))];
	newNum[1] = base32Array[convertToDecimal(atoi(num2))];
	newNum[2] = '\0';

	free(num1);
	free(num2);

	return newNum;
}

/*
The function gets a binary number and converting him into decimal number.
*/
int convertToDecimal(int num)
{
	int rest,base = 1,newNum = 0;

	while (num > 0)  
    {  
        rest = num % 10;  
        newNum = newNum + rest * base;  
        num = num / 10;  
        base = base * 2;  
   	} 

	return newNum;
}

/*
The function converts decimal address into base 32 word.
*/
char* decAddressToBase32(int address)
{
	int i,j = 0;
	char *num;
	num = (char*)malloc(sizeof(char)*10);
	strcpy(num,"0000000000");

    for (i = 1 << 9; i > 0; i = i>>1)
    {
		if(address & i)
			num[j] = '1';
		else
			num[j] = '0';
		j++;
	}
	
	return numToBase32(num);
}

/*
The function creates .ext file and write the .external into it
*/
void make_externals_file(char *file_name, char external_image[156][11], int indx_in_external_image)
{
  int i = 0;
	FILE *externals;
	char *new_file_name;
	new_file_name = strtok(file_name,".");
	strcat(new_file_name,".ext");
	externals = fopen(new_file_name,"w");

	while(i < indx_in_external_image)
	{
		fprintf(externals,"%s	%s\n", external_image[i], external_image[i+1]);
		i+=2;
	}
}

/*
The function creates .ent file and write the .entry words into it
*/
void make_entry_file(char *file_name, char entry_image[156][11], int indx_in_entry_image)
{
  int i = 0;
	FILE *entries;
	char *new_file_name;
	new_file_name = strtok(file_name,".");
	strcat(new_file_name,".ent");
	entries = fopen(new_file_name,"w");

	while(i < indx_in_entry_image)
	{
		fprintf(entries,"%s	%s\n", entry_image[i], entry_image[i+1]);
		i+=2;
	}
}
