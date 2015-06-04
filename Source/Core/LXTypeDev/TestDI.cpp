#include <stdio.h>
#include "FormatDescriptor.h"
#include "FloatToStream.h"
#include "IntegerToStream.h"
#include "BitSetToInteger.h"


#define MAX_DIMENSION 64


class MyStream{
	
public:
	void PutC(char c){
		putchar(c);		
	}
	
}myStream;

void print128(DoubleInteger<uint64>n){
	FormatDescriptor	format,format2;
	const char *pFormat;
	pFormat = "#0x";	
	format.InitialiseFromString(pFormat);
	pFormat = "0x";	
	format2.InitialiseFromString(pFormat);
	
	IntegerToStream(myStream,n.upper,format);
	IntegerToStream(myStream,n.lower,format2);
	putchar('\n');		
}

void TestShiftR(DoubleInteger<uint64>n){
	for (int shiftAmount=1;shiftAmount<80;shiftAmount++){
		int shifted = 0;
		DoubleInteger<uint64>nn = n;
		printf("\nshifting step = %i\n",shiftAmount);		
		print128(nn);
		while(shifted<128){
			nn >>=  shiftAmount;
			shifted += shiftAmount; 
			print128(nn);
		}		
	}
}
void TestShiftL(DoubleInteger<uint64>n){
	for (int shiftAmount=1;shiftAmount<80;shiftAmount++){
		int shifted = 0;
		DoubleInteger<uint64>nn = n;
		printf("\nshifting step = %i\n",shiftAmount);		
		print128(nn);
		while(shifted<128){
			nn <<=  shiftAmount;
			shifted += shiftAmount; 
			print128(nn);
		}		
	}
}



int main(int argc, char **argv){
	DoubleInteger<uint64> dd;
	dd.upper=0x123456789ABCDEF0;
	dd.lower=0x0FEDCBA987654321;

	TestShiftR(dd);
	TestShiftL(dd);


	return 0;
}

