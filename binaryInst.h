#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convertInstToBinary(char [156][11],int*,char*,char [81][81]);
void convertTwoOp(char [156][11],int *,char*,char*,char*);
void convertOneOp(char [156][11],int *,char*,char*);
void convertInstWord(char [156][11],int*,int ,int ,int );
void convertOp(char [156][11],int*,int,int);
void decAddressToBinary(char [156][11],int*,int,char);
void oneDestRegToBinary(char [156][11],int*,char *);
void oneSrcRegToBinary(char [156][11],int*,char *);
void twoRegToBinary(char [156][11],int*,char *,char *);

int whatOp(char *);
int howManyOp(char *);
int whatStruct(char *);
int checkValidOp(char*,char [81][81],int);

