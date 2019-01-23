/**
 * @file StartupManager.cpp
 * @brief Header file for class AnyType
 * @date 8 Dec 2018
 * @author Filippo Sartori
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This file contains the code required to managed the startup of
 * MARTe systems.
 * */

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StartupManager.h"
#include "CompositeErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{
namespace StartupManager{

/**
 * The hook to which all startup objects will link to
 */
InitialisationEntry *startupManagerHook = NULL_PTR(InitialisationEntry *);

/**
 * to assign unique startup order to objects
 */
uint16 startupOrder = 0;

InitialisationEntry::InitialisationEntry(CCString nameIn,
				   CCString *tags,
			       CCString * dependenciesIn)
:name(nameIn),initialisationTags(tags),dependencies(dependenciesIn)
{
	startupOrder = 0;
	next = startupManagerHook;
	startupManagerHook = this;
	initialisationInProgress = false;
	initialised = false;
}

InitialisationEntry::~InitialisationEntry(){
}

ErrorManagement::ErrorType InitialisationEntry::Finish(){
	ErrorManagement::ErrorType ret;
	return ret;
}

ErrorManagement::ErrorType InitialisationEntry::Initialise(){
	ErrorManagement::ErrorType ret;
	COMPOSITE_REPORT_ERROR_STATIC(ErrorManagement::Information,80,"Check ",name);

	if (!initialised){
		ret.internalSetupError = initialisationInProgress;
		COMPOSITE_REPORT_ERROR_STATIC(ret,64,"Dependency loop in object ",name);
		if (ret){
			initialisationInProgress = true;

			COMPOSITE_REPORT_ERROR_STATIC(ErrorManagement::Information,80,"Initialising ",name);

			// start each dependency
			uint32 nOfDependencies = dependencies.GetSize();
			while ((nOfDependencies > 0) && ret){
				nOfDependencies--;
				ret = StartupManager::Initialise(dependencies[nOfDependencies]);
			}

			if (ret){
				ret = Init();
				COMPOSITE_REPORT_ERROR_STATIC(ret,64,"Init of ",name ," Failed");
			}

			COMPOSITE_REPORT_ERROR_STATIC(ErrorManagement::Information,80,"Initialising ",name, " ended");

			startupOrder = ++StartupManager::startupOrder;
			initialisationInProgress = false;
			initialised = true;
		}
	} else {
		COMPOSITE_REPORT_ERROR_STATIC(ErrorManagement::Information,80,name, "Initialised OK");
	}


	return ret;
}

ErrorManagement::ErrorType InitialisationEntry::Terminate(uint16 startupOrderIn){
	ErrorManagement::ErrorType ret;

	if ((startupOrderIn > 0)  && (startupOrder == startupOrderIn)){
		COMPOSITE_REPORT_ERROR_STATIC(ErrorManagement::Information,80,"Terminating ",name);
		ret = Finish();
		initialised = false;
		startupOrder = 0;
	}

	return ret;
}

bool InitialisationEntry::HasTag(CCString tag){
	bool ret = false;
	uint32 nOfTags = initialisationTags.GetSize();
    while ((nOfTags > 0u) && (ret == false)){
    	nOfTags--;
    	ret = (initialisationTags[nOfTags] == tag);
    }
	return ret;
}

ErrorManagement::ErrorType Initialise(CCString tag){
	ErrorManagement::ErrorType ret;

	bool doAll = (tag.GetSize() == 0);

	if (doAll){
		REPORT_ERROR(ErrorManagement::Information,"Initialising All");
	} else {
		COMPOSITE_REPORT_ERROR_STATIC(ErrorManagement::Information,80,"Initialising tag:",tag);
	}

	InitialisationEntry *entry = startupManagerHook;
	while ((entry != NULL_PTR(InitialisationEntry *)) && ret){
		if (doAll || (entry->HasTag(tag))){
			ret = entry->Initialise();
		}
		entry = entry->Next();
	}
	if (doAll){
		REPORT_ERROR(ret,"Initialising of All Failed");
	} else {
		COMPOSITE_REPORT_ERROR_STATIC(ret,80,"Initialise of ",tag," Failed");
	}

	if (ret){
		if (doAll){
			REPORT_ERROR(ErrorManagement::Information,"Initialising All finished ok");
		} else {
			COMPOSITE_REPORT_ERROR_STATIC(ErrorManagement::Information,80,"Initialising tag:",tag," completed");
		}
	}

	return ret;
}

ErrorManagement::ErrorType Terminate(){
	ErrorManagement::ErrorType ret;

	for (uint16 i = startupOrder; i > 0;i--){
		InitialisationEntry *entry = startupManagerHook;
		while ((entry != NULL_PTR(InitialisationEntry *)) && ret){
			ret = entry->Terminate(i);
			entry = entry->Next();
		}
		REPORT_ERROR(ret,"Terminate Failed");
	}

	return ret;
}


} //StartupManager
} //MARTe
