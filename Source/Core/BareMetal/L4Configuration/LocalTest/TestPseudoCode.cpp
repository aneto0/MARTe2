
#include "StartupManager.h"
#include "PseudoCode.h"
#include "HighResolutionTimer.h"
#include "BasicConsole.h"


using namespace MARTe;

CCString RPNCode=
		"READ A\n"
		"READ B\n"
		"SUB\n"
		"CONST float32 3.140000\n"
		"MUL\n"
		"DUP\n"
		"WRITE C\n"
		"CONST float32 0.500000\n"
		"POW\n"
		"TAN\n"
		"CAST int32\n"
		"DUP\n"
		"WRITE D\n"
		"READ C\n"
		"LOG\n"
		"CAST int32\n"
		"GT\n"
		"WRITE E\n"
		"READ N1\n"
		"READ N2\n"
		"READ N3\n"
		"READ N4\n"
		"ADD\n"
		"MUL\n"
		"SUB\n"
		"WRITE N5\n"
;

int main(){

	StartupManager::Initialise();

	PseudoCode::Context context;

	ErrorManagement::ErrorType ret;

	ret = context.ExtractVariables(RPNCode);

	if (ret){
		uint32 index = 0U;
		PseudoCode::VariableInformation *var;

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
			if (var->name == "N1"){
				var->type = TypeDescriptor("int8");
			}
			if (var->name == "N2"){
				var->type = TypeDescriptor("int8");
			}
			if (var->name == "N3"){
				var->type = TypeDescriptor("int8");
			}
			if (var->name == "N4"){
				var->type = TypeDescriptor("int8");
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
				var->type = TypeDescriptor("int32");
			}
			if (var->name == "E"){
				var->type = TypeDescriptor("uint8");
			}
			if (var->name == "N5"){
				var->type = TypeDescriptor("int8");
			}
		}
	}

	if (ret){
		printf ("COMPILE\n");
		ret = context.Compile(RPNCode);
	}

	if (ret){
		printf("SUCCESSFUL\n");
		printf("size of constant area = %i\n",context.startOfVariables);
		printf("size of data area = %i\n",context.dataMemory.GetNumberOfElements());
		printf("size of code area = %i\n",context.codeMemory.GetSize());
		printf("size of stack area = %i\n",context.stack.GetNumberOfElements());
	}

	if (ret){
		printf ("ASSIGN INPUTS\n");
		int32 index = 0;
		PseudoCode::VariableInformation *var;

		while(context.BrowseInputVariable(index,var)){
			index++;
			if (var->name == "A"){
				float *x  = reinterpret_cast<float *>(&context.dataMemory[var->location]);
				*x = 1.0;
			}
			if (var->name == "B"){
				float *x  = reinterpret_cast<float *>(&context.dataMemory[var->location]);
				*x = 2.0;
			}
			if (var->name == "N1"){
				int8 *x  = reinterpret_cast<int8 *>(&context.dataMemory[var->location]);
				*x = 1;
			}
			if (var->name == "N2"){
				int8 *x  = reinterpret_cast<int8 *>(&context.dataMemory[var->location]);
				*x = 2;
			}
			if (var->name == "N3"){
				int8 *x  = reinterpret_cast<int8 *>(&context.dataMemory[var->location]);
				*x = 3;
			}
			if (var->name == "N4"){
				int8 *x  = reinterpret_cast<int8 *>(&context.dataMemory[var->location]);
				*x = 4;
			}
		}
	}

	if (ret){
		printf ("VAR ALLOCATION RESULT\n");
		uint32 index = 0;
		PseudoCode::VariableInformation *var;

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


	if (ret){
		printf ("DEBUG MODE EXECUTION \n");
		// let stdout catch up..
		fflush(stdout);
		DynamicCString dcs;
		CStringTool cst = dcs();
//		BasicConsole console;
		ret = context.Execute(PseudoCode::Context::debugMode,&cst,0);
		if (dcs.GetSize() > 0){
			printf("%s\n",dcs.GetList());
		}
	}

	if (ret){
		printf ("FAST MODE EXECUTION \n");
		printf ("Executes 1 Million times ");
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

	if (ret){
		printf ("SAFE MODE EXECUTION \n");
		printf ("Executes 1 Million times ");
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

	if (ret){
		printf ("DECOMPILE showing types pCode\n");
		DynamicCString RPNCodeRev;
		ret = context.DeCompile(RPNCodeRev,true);

		printf("Decompiled:\n%s\n",RPNCodeRev.GetList());
	}

	if (ret){
		printf ("DECOMPILE not showing types pCode\n");
		DynamicCString RPNCodeRev;
		ret = context.DeCompile(RPNCodeRev,false);

		printf("Decompiled:\n%s\n",RPNCodeRev.GetList());
		if (RPNCode == RPNCodeRev){
			printf("identical to the original source\n");
		} else {
			printf("not identical to the original source\n");
		}
	}

	if (!ret){
		printf ("FAILED - see log\n");
	}

 	StartupManager::Terminate();

}

