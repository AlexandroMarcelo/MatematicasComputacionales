#include <stdio.h>
#include <stdlib.h>
#define MAX 100

char toDecimal(char givenValue[], int base);
char fromDecimal(int decimalValue, int base);
void displayConvertedNumber(int length, char convertedNumber[]);

int main(int argc, char const *argv[])
{
	char * n1 = NULL;

	while ((c = getopt (argc, argv, "i:o:")) != -1){
	    switch (c)
	    	{
	    		case 'i':
	        		aflag = 1;
	        		break;
	      		case 'o':
	        		bflag = 1;
	        		break;
		      	default:
		        	printf("The argument has to be correct\n");
	    }
	}
	//printf("i= %d & o= %d.\n", );
    for (int i = optind; i < argv; ++i)
    {
    	printf("The argument %d is: %s.\n", i, argv[i]);
    }

s
	//char givenValue[MAX];
	int decimalValue;
	int base;
	printf("Give me the maternal base\n");
	//scanf("%d", &decimalValue);
	printf("Give me the decimal value\n");
	scanf("%d", &decimalValue);
	printf("Give me the base to be converted\n");
	scanf("%d", &base);
	fromDecimal(decimalValue, base);
	return 0;
}


char fromDecimal(int decimalValue, int base){
	int digits, j = 0;
	char convertedNumber[MAX];
	while(decimalValue>0){
		int remainder = decimalValue%base;
		if (remainder<10)
		{
			convertedNumber[j++] = 48 + remainder; //because is in ascii code, so the 48 represents the 0, and adding the remainder gets the decimal number
		}
		else{
			convertedNumber[j++] = 55 + remainder; //this because 55 represents the letters (55='A'), when adding the reminder gets the corresponding letter for the given base
		}
		decimalValue = decimalValue / base;
	}
	char hola = 'h';
	displayConvertedNumber(j, convertedNumber);
	return hola;
}

char toDecimal(char givenValue[], int base){
	
}

void displayConvertedNumber(int length, char convertedNumber[]){
	for (int i = length; i >= 0; i--){
		printf("%c", convertedNumber[i]);
	}
	printf("\n");
}