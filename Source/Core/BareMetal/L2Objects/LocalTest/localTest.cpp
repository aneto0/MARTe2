
#include "TestSupport.h"
#include "StartupManager.h"
#include "PseudoCode.h"

using namespace MARTe;

CCString RPNCode=
		"READ A\n"
		"READ B\n"
		"SUM\n"
		"CONST float64 3.14\n"
		"PROD\n"
		"DUP\n"
		"WRITE C\n"
		"CONST float32 0.5\n"
		"POW\n"
;

int main(){

	StartupManager::Initialise();

	PseudoCode::Context context;

	ErrorManagement::ErrorType ret;

	ret = context.ExtractVariables(RPNCode);

	if (ret){
		uint32 index = 0;
		PseudoCode::Context::VariableInformation *var;

		printf ("VAR SCAN RESULT\n");
		while(context.BrowseInputVariable(index,var)){
			printf ("input  var %2i = %s\n",index,var->name.GetList());
			index++;
			if (var->name == "A"){
				var->type = TypeDescriptor("float32");
			}
			if (var->name == "B"){
				var->type = TypeDescriptor("float32");
			}
		}

		index = 0;
		while(context.BrowseOutputVariable(index,var)){
			printf ("output var %2i = %s\n",index,var->name.GetList());
			index++;
			if (var->name == "C"){
				var->type = TypeDescriptor("float32");
			}
		}

		printf("size of constant area = %i\n",context.startOfVariables);

	}

	if (ret){
		ret = context.Compile(RPNCode);
		printf("size of data area = %i\n",context.dataMemory.GetNumberOfElements());
		printf("size of code area = %i\n",context.codeMemory.GetSize());
		printf("size of stack area = %i\n",context.stack.GetNumberOfElements());
	}


 	//RunAllTests();
 	StartupManager::Terminate();

}

