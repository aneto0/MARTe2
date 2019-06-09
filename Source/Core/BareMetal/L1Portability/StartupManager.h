/**
 * @file StartupManager.h
 * @brief Header file for class AnyType
 * @date 4 Dec 2018
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

 * @details This header file contains the declaration of the components
 * necessary to manage the controlled startup of the MARTe framework.
 **/

#ifndef STARTUPMANAGER_H_
#define STARTUPMANAGER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorType.h"
#include "CompilerTypes.h"
#include "CCString.h"


/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{
namespace StartupManager {

/**
 * class to be used as base class to build an InitialisationEntry
 */
class InitialisationEntry{
public:
	/**
	 * called by the Start function
	 */
	ErrorManagement::ErrorType Initialise();

	/**
	 * called by the End function.
	 * will call Finish if the startUpOrder is matched and > 0
	 */
	ErrorManagement::ErrorType Terminate(uint16 startupOrder);

	/**
	 * to be specialised by derived class
	 */
	virtual ErrorManagement::ErrorType Init()=0;

	/**
	 * to be specialised by derived class.
	 * can be omitted if not needed
	 */
	virtual ErrorManagement::ErrorType Finish();

	/**
	 * true if the following tag is present
	 */
	bool HasTag(CCString tag);

	/**
	 * specialised class to call passing tags and dependencies
	 */
	InitialisationEntry( CCString name,
				         CCString *tags,
			             CCString* dependencies);

	/**
	 *
	 */
	virtual ~InitialisationEntry();

	/**
	 *
	 */
	inline InitialisationEntry *Next();
private:
	/**
	 * true if this node is being processed
	 */
	bool initialisationInProgress;

	/**
	 * true if this node is being processed
	 */
	bool initialised;

	/** 1 for the first 0 for non initialised */
	uint16 startupOrder;

	/**
	 * next StartupManaged objects as grabbed from the startupManagerHook
	 */
	InitialisationEntry *next;

	/**
	 * the name of this initialisation entry
	 */
	CCString name;

	/**
	 * specifies the tags that this entry will initialise
	 * a tag is initialised if all the entries possessing
	 * it have been initialised.
	 */
	const ZeroTerminatedArray<CCString> initialisationTags;

	/**
	 * specified all the tags that need to be initialised
	 * before this
	 */
	const ZeroTerminatedArray<CCString> dependencies;

private:
	//
	void operator =(const InitialisationEntry &h){}
};

/**
 * This pointer is set to NULL at the module initialisation before
 * than any object initialisation. Startup Managed Objects will self link to
 * this pointer to be processed by the MarteStart() and MarteEnd() static calls
 */
extern InitialisationEntry *startupManagerHook;

/**
 * to be incremented once an initialisation has been completed and to be
 * used as own startup order
 */
extern uint16 startupOrder;

/**
 * This static function will process all the StartupManagedObjects
 * invoking their Start() method. The order of execution depends on
 * the intermodule dependencies. If tag is not an empty string then
 * the Start will try Start all entries possessing this tag.
 */
ErrorManagement::ErrorType Initialise(CCString tag=emptyString);

/**
 * This static function will process all the StartupManagedObjects invoking their End() method.
 * The order of execution will be exactly the opposite of the order of startup.
 */
ErrorManagement::ErrorType Terminate();

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 *
 */
InitialisationEntry *InitialisationEntry::Next(){
	return next;
}


} // StartupManager
} // MARTe


#define SM_BRACED_INIT_LIST(...) {__VA_ARGS__}

#include <stdio.h>

/**
 * Macro to install startup managers for a given class or object
 * One has to define the custom virtual ErrorManagement::ErrorType Init() and
 * virtual ErrorManagement::ErrorType Finish(); methods of the class
 * className ## Startup.
 */
#define INSTALL_STARTUP_MANAGER_INITIALISATION_ENTRY(className,listOfTags,listOfDepends)\
class className ## Startup: public StartupManager::InitialisationEntry { 				\
public:																					\
	className ## Startup();																\
	~className ## Startup(){};															\
	virtual ErrorManagement::ErrorType Init();											\
	virtual ErrorManagement::ErrorType Finish();										\
private:																				\
	void operator =(const className ## Startup &h){}									\
};																						\
className ## Startup className ## StartupObject;										\
static CCString cri_tags[] = SM_BRACED_INIT_LIST listOfTags;							\
static CCString cri_depend[] = SM_BRACED_INIT_LIST listOfDepends;						\
className ## Startup::className ## Startup():											\
	StartupManager::InitialisationEntry(CCString(#className ),&cri_tags[0],&cri_depend[0]){}\
DLL_API StartupManager::InitialisationEntry *className ## StartupLoader(){ return &className ## StartupObject ; }


#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_STARTUPMANAGER_H_ */
