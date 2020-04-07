
#include "StartupManager.h"
#include "PseudoCode.h"
#include "HighResolutionTimer.h"
#include "BasicConsole.h"


using namespace MARTe;

CCString RPNCode=
		"READ A\n"
		"READ B\n"
		"SUB\n"
		"CONST float32 3.14\n"
		"MUL\n"
		"DUP\n"
		"WRITE C\n"
		"CONST float32 0.5\n"
//		"DIV\n"
		"POW\n"
		"TAN\n"
		"WRITE D\n"
;

int main(){

	StartupManager::Initialise();

	PseudoCode::Context context;

	ErrorManagement::ErrorType ret;

	ret = context.ExtractVariables(RPNCode);

	if (ret){
		int32 index = 0;
		PseudoCode::Context::VariableInformation *var;

		printf ("VAR SCAN RESULT\n");
		while(context.BrowseInputVariable(index,var)){
			printf ("input  var %2i @%04x = %s \n",index,var->location,var->name.GetList());
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
			printf ("output var %2i @%04x = %s\n",index,var->location,var->name.GetList());
			index++;
			if (var->name == "C"){
				var->type = TypeDescriptor("float32");
			}
			if (var->name == "D"){
				var->type = TypeDescriptor("float32");
			}
		}
	}

	if (ret){
		ret = context.Compile(RPNCode);
		printf("size of constant area = %i\n",context.startOfVariables);
		printf("size of data area = %i\n",context.dataMemory.GetNumberOfElements());
		printf("size of code area = %i\n",context.codeMemory.GetSize());
		printf("size of stack area = %i\n",context.stack.GetNumberOfElements());
	}

	if (ret){
		int32 index = 0;
		PseudoCode::Context::VariableInformation *var;

		printf ("VAR ALLOCATION RESULT\n");
		while(context.BrowseInputVariable(index,var)){
			printf ("input  var %2i @%04x = %s \n",index,var->location,var->name.GetList());
			index++;
		}

		index = 0;
		while(context.BrowseOutputVariable(index,var)){
			printf ("output var %2i @%04x = %s\n",index,var->location,var->name.GetList());
			index++;
		}
	}

	printf ("DEBUG MODE EXECUTION \n");

	if (ret){
		// let stdout catch up..
		fflush(stdout);
		DynamicCString dcs;
		CStringTool cst = dcs();
//		BasicConsole console;
		ret = context.Execute(PseudoCode::Context::debugMode,&cst,0);
		printf("%s\n",dcs.GetList());
	}

	printf ("FAST MODE EXECUTION \n");
	printf ("Executes 1 Million times ");
	{
		Ticks t1,t2;
		t1 = HighResolutionTimer::GetTicks();
		if (ret){
			for (int j = 0;(j<10) && ret;j++) {
				printf (".");
				fflush(stdout);
				for (int i = 0;(i<100000) && ret;i++) {
					ret = context.Execute(PseudoCode::Context::fastMode,NULL,0);
				}
			}
		}
		t2 = HighResolutionTimer::GetTicks();
		MilliSeconds dT = t2-t1;
		printf (" in %i milliseconds\n",dT.GetTimeRaw());
		fflush(stdout);
	}

	printf ("SAFE MODE EXECUTION \n");
	printf ("Executes 1 Million times ");
	{
		Ticks t1,t2;
		t1 = HighResolutionTimer::GetTicks();
		if (ret){
			for (int j = 0;(j<10) && ret;j++) {
				printf (".");
				fflush(stdout);
				for (int i = 0;(i<100000) && ret;i++) {
					ret = context.Execute(PseudoCode::Context::safeMode,NULL,0);
				}
			}
		}
		t2 = HighResolutionTimer::GetTicks();
		MilliSeconds dT = t2-t1;
		printf (" in %i milliseconds\n",dT.GetTimeRaw());
		fflush(stdout);

	}

	printf ("DECOMPILE pCode\n");
	if (ret){
		DynamicCString RPNCodeRev;
		ret = context.DeCompile(RPNCodeRev);

		printf("Decompiled:\n%s\n",RPNCodeRev.GetList());
	}

	if (!ret){
		printf ("Failed somewhere\n");
	}

 	StartupManager::Terminate();

}

