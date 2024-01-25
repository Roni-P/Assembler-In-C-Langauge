#include "Functions.h"

/*
This Function checks if the given name is a saved name.
*/
int isSavedName(char* name)
{
	char savedNames[29][8] ={".data",".string",".struct",".entry",".extern","mov","cmp","add","sub","lea","not","clr","inc","dec","jmp","bne","get","prn","jsr","rts","hlt","r0","r1","r2","r3","r4","r5","r6","r7"};

	int i = 0,numOfSavedNames = 29;

	while(i<numOfSavedNames) /*check if the name is saved name*/
	{
		if(!strcmp(savedNames[i],name))
			return 1;
		i++;
	}
	
	return 0;
}

/*
This Function checks if the string is instruction.
return the index of the instruction name if yes else return -1.
*/
int checkInst(char *str)
{
	char savedInstNames[16][4] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get","prn","jsr","rts","hlt"};
	int numOfInstNames = 16,i = 0;

	while(i<numOfInstNames) /*check if the str is instruction*/
	{
		if(strcmp(str,savedInstNames[i]) == 0)
			return i;
		i++;
	}
	
	return -1;
}

/*
This Function checks if the given instruction can get the given source operand 
*/
int checkOpSrc(int instNum,int op_source)
{
	int makor[16][4] = {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

	return makor[instNum][op_source];
}

/*
This Function checks if the given instruction can get the given destination operand 
*/
int checkOpDst(int instNum,int op_dest)
{
	int yaad[16][4] = {{0,1,1,1},{1,1,1,1},{0,1,1,1},{0,1,1,1},{0,1,1,1},{0,1,1,1},{0,1,1,1},{0,1,1,1},{0,1,1,1},{0,1,1,1},{0,1,1,1},{0,1,1,1},{1,1,1,1},{0,1,1,1},{0,0,0,0},{0,0,0,0}};

	return yaad[instNum][op_dest];
}

/*
The function checks if the given string is a valid number
*/
int checkValidNum(char *num)
{
	int i = 0;

	if(num[0] == '-' || num[0] == '+')
		i+=1;

	for(;num[i] != '\0';i++)
	{
		if(isdigit(num[i]) == 0 || num[i] == '.')
			return 0;
	}	
	
	if(atoi(num) < -1023 || atoi(num) > 1023)
		return 0;
	return 1;
}
