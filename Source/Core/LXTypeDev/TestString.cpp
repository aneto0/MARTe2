#include "StreamString.h"
#include "ErrorManagement.h"
#include <stdio.h>

void MyErrorReportFunction(ErrorInformation &errorInfo,const char *errorDescription){
	if ((errorInfo.fileName!=NULL) && (errorInfo.header.lineNumber!=0)){
		printf ("[%s:%i:%s()] ", errorInfo.fileName,errorInfo.header.lineNumber,errorInfo.functionName);		
	}
	
	printf ("%s : %s \n", ErrorManagement::ErrorName(errorInfo.header.errorType),errorDescription);	
	
}


int main(){
	ErrorManagement::SetErrorMessageProcessFunction(MyErrorReportFunction);

	uint32 one=1;
	uint32 two=2;
	uint32 zero=0;

	if((one-two) > zero) printf("ahahah");

	printf("TEST\n");
	
	StreamString s;
	printf("buffer = %x\n",s.Buffer());
	printf("size = %i\n",s.Size());
	
	s = "Ciao begli stronzi";
	
	printf("TEST\n");
	printf("size = %i\n",s.Size());
	printf("value = %s\n",s.Buffer());
	
	s.Seek(0);
        s.PutC('H');
	printf("value = %s\n",s.Buffer());

	s.Seek(20);
        s.PutC('?');
	printf("value = %s\n",s.Buffer());
	
	s.Seek(0);
        s.Printf("woopah = %i\n",32768);
        s.Printf("o = (%f,%5.3f) \n",2.1,0.2);

        s.Printf("address of s = (%p) \n",&s);

        const char *constantChar= "ULLALAH!";

        s.Printf("A constant char (%7s) \n",constantChar);

        s.Printf("The pointer to a constant char  (%x) \n",(void *)constantChar);
        s.Printf("A constant char as a pointer (%p) \n",constantChar);

	StreamString ss("urca loca!");
        s.Printf("A StreamString (%s) \n",ss);

        ss.Seek(5);
        s.Printf("A StreamInterface (%s) \n",*((Streamable *)&ss));

	printf("value = %s\n",s.Buffer());


	return 0;
}
