#include "Functions.h"
#include "binaryInst.h"

/*
Function that get instruction, source operand and destination operand,
checks how many operands the instruction can get and convert the instruction and operands into binary base words.
*/
void convertInstToBinary(char inst_image[156][11],int *inst_counter,char* inst,char operands[81][81])
{
	if(howManyOp(inst) == 2) /*if the instruction gets two operands*/
	{
		convertTwoOp(inst_image,inst_counter,inst,operands[0],operands[2]);
	}

	else if(howManyOp(inst) == 1) /*if the instruction gets one operands*/
	{
		convertOneOp(inst_image,inst_counter,inst,operands[0]);
	}
	else /*instruction without operands*/
	{
		convertInstWord(inst_image,inst_counter,checkInst(inst),-1,-1);
		(*inst_counter)++;
	}
}

/*
The function converts instruction and two operands into binary base array.
*/
void convertTwoOp(char inst_image[156][11],int *inst_counter,char* inst,char* op_source,char* op_dest)
{
	/*converts the first word to binary*/
	convertInstWord(inst_image,inst_counter,checkInst(inst),whatOp(op_source),whatOp(op_dest));
	(*inst_counter)++;

	/*converts the given operands to binary*/
	if(whatOp(op_source) == 0) /*מיעון מידי*/
	{
		decAddressToBinary(inst_image,inst_counter,atoi(op_source+1),'A');
		(*inst_counter)++;
	}
	else if(whatOp(op_source) == 1) /*מיעון ישיר*/
		{
			strcpy(inst_image[(*inst_counter)],op_source);
			(*inst_counter)++;
		}
	else if(whatOp(op_source) == 2) /*מיעון רשומה*/
		{
			strncpy(inst_image[(*inst_counter)],op_source,strlen(op_source)-2);
			inst_image[(*inst_counter)][strlen(op_source)-2] = '\0';
			(*inst_counter)++;
			decAddressToBinary(inst_image,inst_counter,whatStruct(op_source),'A');
			(*inst_counter)++;
		}
	else if(whatOp(op_source) == 3) /*מיעון רגיסטר ישיר*/
		{
			if(whatOp(op_dest) == 3) /*if source and dest operands are registers*/
			{
				twoRegToBinary(inst_image,inst_counter,op_source,op_dest);
				(*inst_counter)++;
				return;
			}
			else
			{
				oneSrcRegToBinary(inst_image,inst_counter,op_source);
				(*inst_counter)++;
			}
		}
	
	if(whatOp(op_dest) == 0) /*מיעון מידי*/
		{
			decAddressToBinary(inst_image,inst_counter,atoi(op_dest),'A');
			(*inst_counter)++;
		}
	else if(whatOp(op_dest) == 1) /*מיעון ישיר*/
		{
			strcpy(inst_image[(*inst_counter)],op_dest);
			(*inst_counter)++;
		}
	else if(whatOp(op_dest) == 2) /*מיעון רשומה*/
		{
			strncpy(inst_image[(*inst_counter)],op_dest,strlen(op_dest)-2);
			(*inst_counter)++;
			decAddressToBinary(inst_image,inst_counter,whatStruct(op_dest),'A');
			(*inst_counter)++;
		}
	else if(whatOp(op_dest) == 3) /*מיעון רגיסטר ישיר*/
		{
			oneDestRegToBinary(inst_image,inst_counter,op_dest);
			(*inst_counter)++;
		}
}

/*
The function converts instruction and one operands into binary base array.
*/
void convertOneOp(char inst_image[156][11],int *inst_counter,char* inst,char* op_dest)
{
	/*converts the first word to binary*/
	convertInstWord(inst_image,inst_counter,checkInst(inst),-1,whatOp(op_dest));
	(*inst_counter)++;

	/*converts the given operand to binary*/
	if(whatOp(op_dest) == 0) /*מיעון מידי*/
	{
		decAddressToBinary(inst_image,inst_counter,atoi(op_dest+1),'A');
		(*inst_counter)++;
	}
	else if(whatOp(op_dest) == 1) /*מיעון ישיר*/
		{
			strcpy(inst_image[(*inst_counter)],op_dest);
			(*inst_counter)++;
		}	
	else if(whatOp(op_dest) == 2) /*מיעון רשומה*/
		{
			strncpy(inst_image[(*inst_counter)],op_dest,strlen(op_dest)-2);
			(*inst_counter)++;
			decAddressToBinary(inst_image,inst_counter,whatStruct(op_dest),'A');
			(*inst_counter)++;
		}
	else if(whatOp(op_dest) == 3) /*מיעון רגיסטר ישיר*/
		{
			oneDestRegToBinary(inst_image,inst_counter,op_dest);
			(*inst_counter)++;
		}
}

/*
The function converts instruction and two operands into binary base word.
*/
void convertInstWord(char inst_image[156][11],int *inst_counter,int instNum,int op_source,int op_dest)
{
	int j,k = 0;/*j - run through the bits,k - run through inst_image current string*/
	
	strcpy(inst_image[*inst_counter],"0000000000");

	for(j = 1<<3;j>0;j = j>>1)
	{
		if(j & instNum)
			inst_image[*inst_counter][k] = '1';
		else
			inst_image[*inst_counter][k] = '0';
		k++;
	}
	
	convertOp(inst_image,inst_counter,op_source,op_dest);
}

/*
The function change the given binary number with the given operands.
*/
void convertOp(char inst_image[156][11],int *inst_counter,int op_source,int op_dest)
{
	int i,j; /*i - run through the bits,j - run through inst_image current string*/
	
	if(op_source != -1)
	{
		j = 4;
		for(i = 1<<1;i>0;i = i>>1)
		{
			if(op_source & i)
				inst_image[*inst_counter][j] = '1';
			else
				inst_image[*inst_counter][j] = '0';
			j++;
		}
	}

	if(op_dest != -1)
	{
		j = 6;
		for(i = 1<<1;i>0;i = i>>1)
		{
			if(op_dest & i)
				inst_image[*inst_counter][j] = '1';
			else
				inst_image[*inst_counter][j] = '0';
			j++;
		}
	}
}

/*
The function checks how many operand each operation can get.
*/
int howManyOp(char *inst)
{
	if(strstr(inst,"mov") || strstr(inst,"cmp") || strstr(inst,"add") || strstr(inst,"sub") || strstr(inst,"lea"))
	return 2;

	if(strstr(inst,"not") || strstr(inst,"clr") || strstr(inst,"inc") || strstr(inst,"dec") || strstr(inst,"jmp") || strstr(inst,"bne") || strstr(inst,"get") || strstr(inst,"prn") || strstr(inst,"jsr"))
	return 1;
	
	return 0;
}

/*
The function checks what operand is the given operand.
*/
int whatOp(char *operand)
{
	if(operand[0] == '#') /*מיעון מידי*/
	{
		if(checkValidNum(operand+1) || (operand[1] == '0' && strlen(operand) == 2))
			return 0;
		else
			return -1;
	}
	
	if(strchr(operand,'.') || strchr(operand,'.')) /*מיעון רשומה*/
	{
		if(operand[strlen(operand)-2] == '.' && (operand[strlen(operand)-1] == '1' || operand[strlen(operand)-1] == '2'))
			return 2;
		else
			return -1;
	} 
	
	if(operand[0] == 'r') /*מיעון רגיסטר ישיר*/
	{
		if(strlen(operand) == 2 && checkValidNum(operand+1) && !strchr(operand,'.') && atoi(operand+1) <= 7)
			return 3;
		else
			return -1;
	}

	return 1; /*מיעון ישיר*/
}

/*
The function checks what field the struct pointed to.
*/
int whatStruct(char *strct)
{
	if(strct[strlen(strct)-1] == '1')
		return 1;
	return 2;
}

/*
The function converts decimal address with A.R.E into binary base word.
*/
void decAddressToBinary(char inst_image[156][11],int *inst_counter,int address,char are)
{
	int i,j = 0; /*i - run through the bits,j - run through inst_image current string*/

	strcpy(inst_image[*inst_counter],"0000000000");

    for (i = 1 << 7; i > 0; i = i>>1)
    {
		if(address & i)
			inst_image[*inst_counter][j] = '1';
		else
			inst_image[*inst_counter][j] = '0';
		j++;
	}

	if(are == 'A')
	{
		inst_image[*inst_counter][j] = '0';
		inst_image[*inst_counter][j+1] = '0';
	}

	if(are == 'R')
	{
		inst_image[*inst_counter][j] = '0';
		inst_image[*inst_counter][j+1] = '1';
	}

	if(are == 'E')
	{
		inst_image[*inst_counter][j] = '1';
		inst_image[*inst_counter][j+1] = '0';
	}
}

/*
The function converts destination register into binary base word.
*/
void oneDestRegToBinary(char inst_image[156][11],int *inst_counter,char *reg)
{
	int i = 0; /*i - run between 0-7*/

	while(i<=7)
	{
		if(strchr(reg,i+'0'))
		{
			decAddressToBinary(inst_image,inst_counter,i,'A');
		}
		i++;
	}
}

/*
The function converts source register into binary base word.
*/
void oneSrcRegToBinary(char inst_image[156][11],int *inst_counter,char *reg)
{
	int j,k = 0,i = 0; /*j - run through the bits,k - run through inst_image current string,i - run between 0-7*/
	
	strcpy(inst_image[*inst_counter],"0000000000");

	while(i<=7)
	{
		if(strchr(reg,i+'0'))
		{
			for(j = 1<<3;j>0;j = j>>1)
			{
				if(j & i)
					inst_image[*inst_counter][k] = '1';
				else
					inst_image[*inst_counter][k] = '0';
				k++;
			}
		}

		i++;
	}
}

/*
The function converts two registers into binary base word.
*/
void twoRegToBinary(char inst_image[156][11],int *inst_counter,char *reg,char *reg2)
{
	int j,k = 0,i = 0; /*j - run through bits,k - run through inst_image current string,i - run between 0-7*/
	
	while(i<=7)
	{
		if(strchr(reg2,i+'0'))
		{
			decAddressToBinary(inst_image,inst_counter,i,'A');
		}
		i++;
	}
	
	i = 0;

	while(i<=7)
	{
		if(strchr(reg,i+'0'))
		{
			for(j = 1<<3;j>0 && k<4;j = j>>1)
			{
				if(j & i)
					inst_image[*inst_counter][k] = '1';
				else
					inst_image[*inst_counter][k] = '0';
				k++;
			}
		}
		i++;
	}
}

/*
This function checks if the given operands are valid for the given instruction.
*/
int checkValidOp(char *inst,char operands[81][81],int size)
{
	if(size == 0) /*no operands*/
	{	
		if(howManyOp(inst) == 0)
			return 1;
		else
			return 0;
	}
	else if(size == 3 && (size-1) == howManyOp(inst)) /*two operands*/
	{
		if(whatOp(operands[0]) != -1 && whatOp(operands[2]) != -1)
		{
			if(checkOpSrc(checkInst(inst),whatOp(operands[0])) && strcmp(operands[1], ",") == 0 && checkOpDst(checkInst(inst),whatOp(operands[2])))
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}
	else if(size == 1 && size == howManyOp(inst)) /*one operand*/
	{
		if(whatOp(operands[0]) != -1)
		{
			if(checkOpDst(checkInst(inst),whatOp(operands[0])))
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}
	
	return 0;
}
