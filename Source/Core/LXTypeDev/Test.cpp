#include <stdio.h>
#include "ErrorManagement.h"
#include "FormatDescriptor.h"
#include "FloatToStream.h"
#include "IntegerToStream.h"
#include "BitSetToInteger.h"

/*
#include "TypeConversion.h"
#include "TypeDescriptor.h"
#include "AnyType.h"
#include "CStream.h"
#include "CharBuffer.h"
#include "Streamable.h"
//#include "StreamInterface.h"
*/

#define MAX_DIMENSION 64

class myStream{
private:
	char buffer[MAX_DIMENSION];
	int32 size;
public:

	myStream(){
		size=0;
	}
	
	void PutC(char c){
		size%=(MAX_DIMENSION-1);
			
		
		buffer[size]=c;
		buffer[size+1]=0;
		size++;		
	}
	
	char* Buffer(){
		return buffer;
	}
};



/*struct TestPattern{
    // the character in the printf format
    const char *format;
    // the set of flags
    FormatDescriptor fd;
};

/// 0 terminated vector of FDLookups 
static const TestPattern testPatterns[] = {
    { "d",          FormatDescriptor(0, 0,     False, False, Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { "5d",	        FormatDescriptor(5, 0,     False, False, Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { ".2d",        FormatDescriptor(0, 2,     False, False, Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { "5.2d",       FormatDescriptor(5, 2,     False, False, Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { " d", 	    FormatDescriptor(0, 0,     True,  False, Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { " 5d",	    FormatDescriptor(5, 0,     True,  False, Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { " .2d",       FormatDescriptor(0, 2,     True,  False, Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { " 5.2d",      FormatDescriptor(5, 2,     True,  False, Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { "-d",         FormatDescriptor(0, 0,     True,  True,  Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { "-5d",	    FormatDescriptor(5, 0,     True,  True,  Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { "-.2d",       FormatDescriptor(0, 2,     True,  True,  Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { "-5.2d",      FormatDescriptor(5, 2,     True,  True,  Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { " -d", 	    FormatDescriptor(0, 0,     True,  True,  Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { " -5d",	    FormatDescriptor(5, 0,     True,  True,  Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { " -.2d",      FormatDescriptor(0, 2,     True,  True,  Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { " -5.2d",     FormatDescriptor(5, 2,     True,  True,  Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { " -15.2d",    FormatDescriptor(15, 2,    True,  True,  Notation::FixedPointNotation, Notation::NormalNotation, False, False) },
    { " -15.2a",    FormatDescriptor(15, 2,    True,  True,  Notation::FixedPointNotation, Notation::HexNotation, False, False) },
    { " -15.2x",    FormatDescriptor(15, 2,    True,  True,  Notation::FixedPointNotation, Notation::HexNotation, False, False) },
    { " -15.2p",    FormatDescriptor(15, 2,    True,  True,  Notation::FixedPointNotation, Notation::HexNotation, False, False) },
    { " -15o",      FormatDescriptor(15, 0,    True,  True,  Notation::FixedPointNotation, Notation::OctalNotation, False, False) },
    { " -15o.",     FormatDescriptor(15, 0,    True,  True,  Notation::FixedPointNotation, Notation::OctalNotation, False, False) },
    { " -15.2b",    FormatDescriptor(15, 2,    True,  True,  Notation::FixedPointNotation, Notation::BitNotation, False, False) }

};

void PrintError(FormatDescriptor &fd, const FormatDescriptor expected){
    printf(" ERROR ");
    if(fd.width != expected.width){
        printf(" width:8 = %d (expected %d)", fd.width, expected.width);
    }
    if(fd.precision != expected.precision){
        printf(" precision:8 = %d (expected %d)", fd.precision, expected.precision);
    }
    if(fd.pad != expected.pad){
        printf(" pad:1 = %d (expected %d)", fd.pad, expected.pad);
    }
    if(fd.leftAlign != expected.leftAlign){
        printf(" leftAlign:1 = %d (expected %d)", fd.leftAlign, expected.leftAlign); 
    }
    if(fd.floatNotation != expected.floatNotation){
        printf(" floatNotation:2 = %d (expected %d)", fd.floatNotation, expected.floatNotation); 
    }
    if(fd.binaryNotation != expected.binaryNotation){
        printf(" binaryNotation:2 = %d (expected %d)", fd.binaryNotation, expected.binaryNotation); 
    }
    if(fd.binaryPadded != expected.binaryPadded){
        printf(" binaryPadded:1 = %d (expected %d)", fd.binaryPadded, expected.binaryPadded);
    }
    if(fd.fullNotation != expected.fullNotation){
        printf(" fullNotation:1 = %d (expected %d)", fd.fullNotation, expected.fullNotation);
    }
    if(fd.spareBits != expected.spareBits){
        printf(" spareBits:8 = %d (expected %d)", fd.spareBits, expected.spareBits);
    }
    printf("\n");
}

void TestAll(){
    int32 n = sizeof(testPatterns)/sizeof(TestPattern);
    int32 i = 0;
    for(i=0; i<n; i++){
        printf("Testing \"%s\"", testPatterns[i].format);
        FormatDescriptor fd;
        bool ok = fd.InitialiseFromString((const char *&)testPatterns[i].format);
        if(!ok){
            PrintError(fd, testPatterns[i].fd);
        }
        else if(fd != testPatterns[i].fd){
            PrintError(fd, testPatterns[i].fd);
        }
        else{
            printf(" OK!\n");
        }
    }
    
}*/

/*

bool TestMagnitude(){
	int number=12345;
	uint8 order=0;
	
	if((order=GetOrderOfMagnitude(number))!=4){
		printf("F order: %c", '0'+order);	
		return False;
	}
	
	printf("\nT order: %c\n", '0'+order);

}


bool TestToDecimalStreamPrivate(){
	myStream thisStream;
	int32 number=40;
	NToDecimalStreamPrivate(thisStream, number);
	printf("\nT buffer1: %s\n", thisStream.Buffer());
	return True;
} 
	

bool TestToDecimalStream(){
	myStream thisStream;
	int32 number=-40;
	NumberToDecimalStream(thisStream, number);
	printf("\nT buffer2: %s\n", thisStream.Buffer());
	return True;
}


bool TestToHexadecimalStream(){
	myStream thisStream;
	uint8 number=0x8f;
	NumberToHexadecimalStream(thisStream, number);
	printf("\nT buffer3: %s\n", thisStream.Buffer());
	return True;
}

bool TestToOctalStream(){
	myStream thisStream;
	uint8 number=0xfd;
	NumberToOctalStream(thisStream, number);
	printf("\nT buffer4: %s\n", thisStream.Buffer());
	return True;
}

bool TestToBinaryStream(){
	myStream thisStream;
	uint8 number=0xa1;
	uint8 size=GetOrderOfMagnitudeBin(number); 
	NumberToBinaryStream(thisStream, number);
	printf("\nT buffer5: %s\n", thisStream.Buffer());
	return True;
}


*/

class MyStream{
	
public:
	void PutC(char c){
		putchar(c);		
	}
	
}myStream;

template <typename T> void Test_FloatToStream2(T number, int size = 12){

	FormatDescriptor	format;
	const char *pFormat;
	int spacing = size;

	while (size > 0){
		pFormat= "- f";	
		format.InitialiseFromString(pFormat);	
		format.size = size;
		format.precision = 0;
		putchar('>');		
		FloatToStream(myStream,number,format);
		putchar('<');
		for (int i=size;i<spacing; i++) putchar(' ');		
		
		pFormat= "- F";	
		format.InitialiseFromString(pFormat);	
		format.size = size;
		format.precision = 0;
		putchar('>');		
		FloatToStream(myStream,number,format);
		putchar('<');		
		for (int i=size;i<spacing; i++) putchar(' ');		

		pFormat = "- e";	
		format.InitialiseFromString(pFormat);	
		format.size = size;
		format.precision = 0;
		putchar('>');		
		FloatToStream(myStream,number,format);
		putchar('<');		
		for (int i=size;i<spacing; i++) putchar(' ');		
		
		pFormat = "- E";	
		format.InitialiseFromString(pFormat);
		format.size = size;
		format.precision = 0;
		putchar('>');		
		FloatToStream(myStream,number,format);
		putchar('<');		
		for (int i=size;i<spacing; i++) putchar(' ');		

		pFormat = "- g";	
		format.InitialiseFromString(pFormat);	
		format.size = size;
		format.precision = 0;
		putchar('>');		
		FloatToStream(myStream,number,format);
		putchar('<');		
		for (int i=size;i<spacing; i++) putchar(' ');		
		
		pFormat = "- G";	
		format.InitialiseFromString(pFormat);	
		format.size = size;
		format.precision = 0;
		putchar('>');		
		FloatToStream(myStream,number,format);
		putchar('<');		
		
		size--;
		putchar('\n');	
	}
	
}

template <typename T> void Test_FloatToStream(T number, int size = 12){

	FormatDescriptor	format;
	const char *pFormat;
	
	for (int i = 0;i<32;i++){
		pFormat= " f";	
		format.InitialiseFromString(pFormat);	
		format.size = size;
		format.precision = 7;
		putchar('>');		
		FloatToStream(myStream,number,format);
		putchar('<');		
		
		pFormat= "- F";	
		format.InitialiseFromString(pFormat);	
		format.size = size;
		format.precision = 7;
		putchar('>');		
		FloatToStream(myStream,number,format);
		putchar('<');		

		pFormat = " e";	
		format.InitialiseFromString(pFormat);	
		format.size = size;
		format.precision = 7;
		putchar('>');		
		FloatToStream(myStream,number,format);
		putchar('<');		
		
		pFormat = "- E";	
		format.InitialiseFromString(pFormat);
		format.size = size;
		format.precision = 7;
		putchar('>');		
		FloatToStream(myStream,number,format);
		putchar('<');		

		pFormat = " g";	
		format.InitialiseFromString(pFormat);	
		format.size = size;
		format.precision = 7;
		putchar('>');		
		FloatToStream(myStream,number,format);
		putchar('<');		
		
		pFormat = " G";	
		format.InitialiseFromString(pFormat);	
		format.size = size;
		format.precision = 7;
		putchar('>');		
		FloatToStream(myStream,number,format);
		putchar('<');		

		putchar('\n');	
		number = number/10;
	}

}

template <typename T> void Test_IntegerToStream(T n){
	FormatDescriptor	format;
	const char *pFormat;
	
	while (n != 0){
		
		putchar('\n');
		pFormat= "- 22i";	
		format.InitialiseFromString(pFormat);	 
		putchar('>');		
		IntegerToStream(myStream,n,format);
		putchar('<');		
		pFormat= " #022x";	
		format.InitialiseFromString(pFormat);	
		putchar('>');		
		IntegerToStream(myStream,n,format);
		putchar('<');		
		pFormat= "- #026o";	
		format.InitialiseFromString(pFormat);	
		putchar('>');		
		IntegerToStream(myStream,n,format);
		putchar('<');		
		pFormat= " #66b";	
		format.InitialiseFromString(pFormat);	
		putchar('>');		
		IntegerToStream(myStream,n,format);
		putchar('<');		
		
		putchar('\n');	
		n = n / 2;
	}
}

void TestBitSetPrint(void *data,int dataBitSize){
	FormatDescriptor	format,format2;
	const char *pFormat;
	pFormat = "#0b";	
	format.InitialiseFromString(pFormat);
	pFormat = "0b";	
	format2.InitialiseFromString(pFormat);
	
	int data64WordSize = (dataBitSize+63)/64;
	int data64BitSize  = data64WordSize * 64;
	uint64 *source = (uint64 *)data;
	
	//from size =1 to size = 64
	for(int size=1;size<64;size++){
		for(int shift=0;shift<data64BitSize;shift+=size){
			IntegerToStream(myStream,source[data64WordSize-1],format);
			for (int i=(data64WordSize-1);i>0;i--){
				IntegerToStream(myStream,source[i-1],format2);
			}
			putchar('\n');
			
			for(int k=0;k<(data64BitSize-(shift+size));k++)putchar(' ');
			putchar(' ');						
			putchar(' ');						
			BitSetToStream(myStream,(unsigned int *)&source[0],shift,size,false,format2);
			putchar('\n');						
		}
	}
}

void MyErrorReportFunction(ErrorInformation &errorInfo,const char *errorDescription){
	if ((errorInfo.fileName!=NULL) && (errorInfo.header.lineNumber!=0)){
		printf ("[%s:%i:%s()] ", errorInfo.fileName,errorInfo.header.lineNumber,errorInfo.functionName);		
	}
	
	printf ("%s : %s \n", ErrorManagement::ErrorName(errorInfo.header.errorType),errorDescription);	
	
}

int main(int argc, char **argv){
	ErrorManagement::SetErrorMessageProcessFunction(MyErrorReportFunction);
	
	REPORT_ERROR(Debug,"Testing Error")
	{
		int64 source[5] =   { 0x13579BDF02468ACE,0x13579BDF02468ACE,0x123456789ABCDEF0,0xDEADBABEBAB00111,0xABBA00CACCA00123};
		TestBitSetPrint(&source[0],320);
/*		
		FormatDescriptor	format,format2;
		const char *pFormat;
		pFormat = "#0b";	
		format.InitialiseFromString(pFormat);
		pFormat = "0b";	
		format2.InitialiseFromString(pFormat);
		
		//from size =1 to size = 64
		for(int size=1;size<64;size++){
			for(int shift=0;shift<320;shift+=size){				
				IntegerToStream(myStream,source[4],format);
				IntegerToStream(myStream,source[3],format2);
				IntegerToStream(myStream,source[2],format2);
				IntegerToStream(myStream,source[1],format2);
				IntegerToStream(myStream,source[0],format2);
				putchar('\n');
				
				for(int k=0;k<(320-(shift+size));k++)putchar(' ');
				putchar(' ');						
				putchar(' ');						
				BitSetToStream(myStream,(unsigned int *)&source[0],shift,size,false,format2);
				putchar('\n');						
			}
			
		}
*/		
		return 0;
	}
	if (0){
		
		FormatDescriptor	format,format2;
		const char *pFormat;
		pFormat = "#0x";	
		format.InitialiseFromString(pFormat);
		pFormat = "0x";	
		format2.InitialiseFromString(pFormat);
		int64 destination[5] =   { 0,0,0};                    
		int64 source[5] =   { 0x13579BDF02468ACE,0x13579BDF02468ACE,0x123456789ABCDEF0,0xDEADBABEBAB00111,0xABBA00CACCA00123};
		PutS(myStream,"source      = ");
		IntegerToStream(myStream,source[4],format);
		IntegerToStream(myStream,source[3],format2);
		IntegerToStream(myStream,source[2],format2);
		IntegerToStream(myStream,source[1],format2);
		IntegerToStream(myStream,source[0],format2);
		putchar('\n');		
		
		uint64 *p= (uint64 *)&source[0];
		uint8 pShift=0;
		uint8 pSize=59;
		uint64 *q= (uint64 *)&destination[0];
		uint8 qShift=0;
		uint8 qSize=pSize;
		for (int i = 0;i<6;i++){
			
			BitSetToBitSet(q,qShift,qSize,false,p,pShift,pSize,false);
			PutS(myStream,"destination = ");
			IntegerToStream(myStream,destination[4],format);
			IntegerToStream(myStream,destination[3],format2);
			IntegerToStream(myStream,destination[2],format2);
			IntegerToStream(myStream,destination[1],format2);
			IntegerToStream(myStream,destination[0],format2);
			putchar('\n');		
//printf("sourc shift=%i address=%i\n",pShift,(int)(p-(uint8 *)&source[0]));
//printf("dest  shift=%i address=%i\n",qShift,(int)(q-(uint8 *)&destination[0]));
		}
		
		return 0;
	}
	
/*	
	if (debug){
		FormatDescriptor	format;
		const char *pFormat;
		double nnn = 9.9E-0;
		pFormat = "2e";	
		format.InitialiseFromString(pFormat);
		FloatToStream(myStream,nnn,format);
		return 0;
	}
*/	
	putchar('\n');	
	double nn0 = 9.94560995409945;
	Test_FloatToStream2(nn0,20);

	putchar('\n');	
	double nn1 = 99456.0995409945;
	Test_FloatToStream2(nn1,20);

	
	putchar('\n');	
	double n0 = 9999995054321000;
	Test_FloatToStream(n0);
	putchar('\n');	
	Test_FloatToStream(n0,6);
	putchar('\n');	
	Test_FloatToStream(n0,5);
	putchar('\n');	
	Test_FloatToStream(n0,4);
	putchar('\n');	
	Test_FloatToStream(n0,3);
	putchar('\n');	
	Test_FloatToStream(n0,2);
	putchar('\n');	
	Test_FloatToStream(n0,1);
	
	
	putchar('\n');	
	float n1 = 1234567800;	
	Test_FloatToStream(n1);

	putchar('\n');	
	double n2 = 1234567800;	
	Test_FloatToStream(n2);
	
	putchar('\n');	
	__float128 n3 = 1234567800;
	Test_FloatToStream(n3);


	putchar('\n');	
	uint64 in1 = 1234567890123456789;
	Test_IntegerToStream(in1);	

	putchar('\n');	
	int64 in2 =  1234567890123456789;
	Test_IntegerToStream(in2);	

	putchar('\n');	
	int64 in3 = -1234567890123456789;
	Test_IntegerToStream(in3);	

	putchar('\n');	
	uint32 in4 = 3456789012;
	Test_IntegerToStream(in4);	

	putchar('\n');	
	int32 in5 =  1234567890;
	Test_IntegerToStream(in5);	

	putchar('\n');	
	int32 in6 = -1234567890;
	Test_IntegerToStream(in6);	

	putchar('\n');	
	uint16 in7 = 65432;
	Test_IntegerToStream(in7);	

	putchar('\n');	
	int16 in8 =  32109;
	Test_IntegerToStream(in8);	

	putchar('\n');	
	int16 in9 = -32109;
	Test_IntegerToStream(in9);	
	
	putchar('\n');	
	uint8 in10 = 210;
	Test_IntegerToStream(in10);	

	putchar('\n');	
	int8 in11 =  109;
	Test_IntegerToStream(in11);	

	putchar('\n');	
	int8 in12 = -109;
	Test_IntegerToStream(in12);	
	/*	
    char buffer[100];
    double d = 1.2345;
    int32  n = 12345;
    uint16 size = 0;
	
	TestMagnitude();
	TestToDecimalStreamPrivate();
	TestToDecimalStream();
	TestToHexadecimalStream();
	TestToOctalStream();
	TestToBinaryStream();

    printf("%s\n", NumberToDecimal(size, buffer, sizeof(buffer), n, false));
    printf("%s\n", NumberToHexadecimal(size, buffer, sizeof(buffer), n, false));
    printf("%s\n", NumberToOctal(size, buffer, sizeof(buffer), n, false));
    printf("%s\n", NumberToBinary(size, buffer, sizeof(buffer), n, false));
   
    printf("%s\n", NumberToDecimal(size, buffer, sizeof(buffer), n, false));
    printf("%s\n", NumberToHexadecimal(size, buffer, sizeof(buffer), n, false, true));
    printf("%s\n", NumberToOctal(size, buffer, sizeof(buffer), n, false, true));
    printf("%s\n", NumberToBinary(size, buffer, sizeof(buffer), n, false, true));

    const char *retString = FloatToFixed(size, buffer, sizeof(buffer), d, 8);
    printf("->%s<-size=%d\n", retString, size);

//    TestAll();
//    TestFormatDescriptor("d");

//    FormatDescriptor expected();
//    TestFormatDescriptor("5d");
	*/
    return 0;
}

