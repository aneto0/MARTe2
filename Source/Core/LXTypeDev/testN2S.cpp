#include <stdio.h>

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

// returns the exponent
// tenToExponent becomes 10 ^ exponent
// positiveNumber is a the abs (number)
template <typename T> uint8 NormalizeInteger(T positiveNumber,T &tenToExponent){       
    tenToExponent = 1;
    T temp ;
    uint8 exp = 0;
    if (sizeof(T)>=8){ // max 19
        temp = tenToExponent * 10000000000; // 10 zeros 
        if (positiveNumber >= temp )  {
            tenToExponent = temp;
            exp += 10;
        }
    }
    if (sizeof(T)>=4){ // max 9 
        temp = tenToExponent * 100000; // 5 zeros
        if (positiveNumber >= temp ) {
            tenToExponent = temp;
            exp += 5;
        }
    }
    if (sizeof(T)>=2){ // max 4 zeros
        temp = tenToExponent * 100; // 2 zeros
        if (positiveNumber >= temp ) {
            tenToExponent = temp;
            exp += 2;
        }

    }
    temp = tenToExponent * 10; // 1 
    if (positiveNumber >= temp ){
            tenToExponent = temp;
            exp ++;
        }
    temp = tenToExponent * 10;  // 1
    if (temp > tenToExponent){
    	if (positiveNumber >= temp ){
            tenToExponent = temp;
            exp ++;
        }
    }
    return exp;
}



class Streamer{

public:
bool PutC(char c){
    putchar(c);
}

}streamer;

/// buffer must have sufficient size to hold a number of size exponent+1! and the trailing zero  
template <typename T> void NumberToDecimalPrivate(char *buffer, T positiveNumber,uint8 exponent){
	char *pCurrent = buffer + exponent + 1;
	*pCurrent-- = 0;
	while (pCurrent >= buffer ){
		unsigned short  digit = positiveNumber % 10;
		positiveNumber        = positiveNumber / 10;
		*pCurrent-- = '0' + digit;
	}
}


template<typename T> void Test(T number){

    for (int i=0; i < (sizeof (T)*2.5);i++){
        T tenToExponent;
        uint8 exp = NormalizeInteger(number,tenToExponent);
        char buffer[80];

        NumberToDecimalPrivate(buffer,number,exp);
printf("(%2i) %s ",i, buffer);
        NumberToDecimalPrivate(buffer,tenToExponent,exp);
printf(" %s  %02i\n",buffer, exp);
//        if (sizeof(T)==1) printf("C(%2i) % 20i % 20i  %02i\n",i,number, tenToExponent, exp);
//        if (sizeof(T)==2) printf("S(%2i) % 20i % 20i  %02i\n",i,number, tenToExponent, exp);
//        if (sizeof(T)==4) printf("I(%2i) % 20li % 20li  %02i\n",i,number, tenToExponent, exp);
//       if (sizeof(T)==8) printf("L(%2i) % 20Li % 20Li  %02i\n",i,number, tenToExponent, exp);

        number *= 10;

    }
}



/// buffer must have sufficient size to hold a number of size exponent+1! and the trailing zero  
template <typename T, class streamer> void NumberToDecimalPrivate2(streamer s, T positiveNumber,int8 numberFillLength=0){
	if (numberFillLength < 0)numberFillLength=0;
	if (sizeof(T)==8){  //19 max
		// check against 10^11
		if ((positiveNumber > 100000000000) || (numberFillLength > 11)) {
			uint32 x = positiveNumber / 100000000000;
//printf("<64,1:%i %i>",x,numberFillLength);			
			positiveNumber %= 100000000000;
			// process the upper part as uint32
			NumberToDecimalPrivate(s,x,numberFillLength-11);
			numberFillLength = 11;
		} // after this 11 max
		
		// check against 10^2
		if ((positiveNumber > 100) || (numberFillLength > 2)){
			uint32 x       = positiveNumber / 100;
//printf("<64,2:%i %i>",x,numberFillLength);			
			positiveNumber = positiveNumber %= 100;
			// process the two parts left
			NumberToDecimalPrivate(s,x,numberFillLength-2);
			numberFillLength = 2;
		}

		uint8 x  = positiveNumber;
//printf("<64,3:%i %i>",x,numberFillLength);			
		NumberToDecimalPrivate(s,x,numberFillLength);
		return;
	}  

	if (sizeof(T)==4){  //9 max
		// check against 10^5
		if ((positiveNumber > 100000) || (numberFillLength > 5)) {
			uint16 x = positiveNumber / 100000;
//printf("<32,1:%i %i>",x,numberFillLength);			
			positiveNumber %= 100000;
			// process the upper part as uint32
			NumberToDecimalPrivate(s,x,numberFillLength-5);
			numberFillLength = 5;
		} // after this 11 max
		
		// check against 10^1
		if ((positiveNumber > 10) || (numberFillLength > 1)){
			uint16 x       = positiveNumber / 10;
//printf("<32,2:%i %i>",x,numberFillLength);			
			positiveNumber = positiveNumber %= 10;
			// process the two parts left
			NumberToDecimalPrivate(s,x,numberFillLength-1);
			numberFillLength = 1;
		}

		uint8 x  = positiveNumber;
//printf("<32,3:%i %i>",x,numberFillLength);			
		NumberToDecimalPrivate(s,x,numberFillLength);
		return;
	}

	char buffer[5]; // just for  a 16 bit number
	
	uint8 index = 4;
	while (positiveNumber > 0){
		uint8 digit    = positiveNumber % 10;
		positiveNumber = positiveNumber / 10;
		buffer[index--] = '0' + digit;
	}
	uint8 i= 0;
	if (numberFillLength )
	for (i=(4-numberFillLength);i<index;i++) s.PutC('0');
	for (i=index+1;i<=4;i++) s.PutC(buffer[i]);
}

/// buffer must have sufficient size to hold a number of size exponent+1! and the trailing zero  
template <typename T, class streamer> void NumberToDecimalPrivate(streamer s, T positiveNumber,int8 numberFillLength=0){

	if (numberFillLength < 0)numberFillLength=0;
	if (sizeof(T)==8){  //19 max
		const uint64 tests[4] = {10000000000000000,1000000000000,100000000,10000};
		int i;
		for (i=0;i<4;i++){
			if (positiveNumber > tests[i])  {
				uint16 x = positiveNumber / tests[i];
				positiveNumber %= tests[i];
				// process the upper part as uint32
				NumberToDecimalPrivate(s,x,numberFillLength);
				numberFillLength = 4;
			} 
		}
		uint16 x = positiveNumber;
		NumberToDecimalPrivate(s,x,numberFillLength);
		return;
	}  

	if (sizeof(T)==4){  //9 max
		const uint32 tests[2] = {100000000,10000};
		int i;
		for (i=0;i<2;i++){
			if (positiveNumber > tests[i])  {
				uint16 x = positiveNumber / tests[i];
				positiveNumber %= tests[i];
				// process the upper part as uint32
				NumberToDecimalPrivate(s,x,numberFillLength);
				numberFillLength = 4;
			} // after this 11 max
		}
		uint16 x = positiveNumber;
		NumberToDecimalPrivate(s,x,numberFillLength);
		return;
	}

	if (sizeof(T)<=2){ 
		char buffer[5]; // just for  a 16 - 8 bit number
		
		uint8 index = 4;
		while (positiveNumber > 0){
			uint8 digit    = positiveNumber % 10;
			positiveNumber = positiveNumber / 10;
			buffer[index--] = '0' + digit;
		}
		uint8 i= 0;
		if (numberFillLength > 0)
		for (i=(5-numberFillLength);i<=index;i++) s.PutC('0');
		for (i=index+1;i<=4;i++) s.PutC(buffer[i]);
	}
}


int main(){
int N = 1;

/*
Test((unsigned char)N);
Test((unsigned short)N);
Test((unsigned int)N);
Test((unsigned long long)N);
Test((char)N);
Test((short)N);
Test((int)N);
Test((long long)N);
*/

NumberToDecimalPrivate(streamer, (uint64) 3); 			streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 45);			streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 678);			streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 9012);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 34567);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 890123);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 4567890);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 12345678);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 901234567);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 8901234567);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 89012345678);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 901234567890);	streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 1234567890123);	streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 45678901234567);	streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 890123456789012);	streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 3456789012345678);	streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 90123456789012345);	streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 678901234567890123);	streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 1234567890123456789);	streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint64) 12345678901234567891);streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint32) 3); 			streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint32) 45);			streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint32) 678);			streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint32) 9012);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint32) 34567);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint32) 890123);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint32) 4567890);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint32) 12345678);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint32) 901234567);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint32) 3456789012);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint16) 3); 			streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint16) 45);			streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint16) 678);			streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint16) 9012);		streamer.PutC('\n');
NumberToDecimalPrivate(streamer, (uint16) 56789);		streamer.PutC('\n');
return 0;

}