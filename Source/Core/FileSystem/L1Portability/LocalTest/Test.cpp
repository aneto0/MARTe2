
#include "TestSupport.h"
#include "StartupManager.h"

int main(){

	MARTe::StartupManager::Initialise();
 	RunAllTests();
 	MARTe::StartupManager::Terminate();

}

