/**
 * @file ObjectRegistryDatabase.cpp
 * @brief Source file for class ObjectRegistryDatabase
 * @date 18/02/2016
 * @author Giuseppe Ferrò
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
 *
 * @details This source file contains the definition of all the methods for
 * the class ObjectRegistryDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorType.h"
#include "ObjectRegistryDatabase.h"
#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferences.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
namespace ObjectRegistryDatabase {

ReferenceT<ReferenceContainer> objectRegistryDatabase;

ReferenceT<ReferenceContainer> Access(){
	return objectRegistryDatabase;
}

INSTALL_STARTUP_MANAGER_INITIALISATION_ENTRY(ObjectRegistryDatabase,("ObjectRegistryDatabase",emptyString),("HeapManager",emptyString))

/**
	Singleton based startup and memory allocation
	Managed shutdown
*/
ErrorManagement::ErrorType ObjectRegistryDatabaseStartup::Init(){
	ErrorManagement::ErrorType ret;

	ReferenceT<ReferenceContainer> ordb(HeapManager::internalsHeapId);
	objectRegistryDatabase = ordb;

	return ret;
}

ErrorManagement::ErrorType ObjectRegistryDatabaseStartup::Finish(){
	ErrorManagement::ErrorType ret;

	objectRegistryDatabase->Purge();
	objectRegistryDatabase.RemoveReference();
	return ret;
}

Reference Find(const CCString path,const Reference current) {
    ReferenceT<ReferenceContainer> domain = current;
    bool isSearchDomain = current.IsValid();
    uint32 backSteps = 0u;
    bool ok = true;
    if (isSearchDomain) {
        while (path[backSteps] == ':') {
            backSteps++;
        }
        isSearchDomain = (backSteps > 0u);
        if (isSearchDomain) {
            uint32 stepsCounter = backSteps;
            // search the current remembering the path
            ReferenceContainerFilterReferences filterRef(1, ReferenceContainerFilterMode::PATH, current);
            ReferenceContainer resultPath;
            objectRegistryDatabase->Find(resultPath, filterRef);
            for (uint32 i = 0u; i < resultPath.Size(); i++) {
                Reference test = resultPath.Get((resultPath.Size() - i) - 1u);
                if (stepsCounter == 0u) {
                    break;
                }
                if (test.IsValid()) {
                    ok = objectRegistryDatabase->Lock();
                    if (ok) {
                        /*lint -e{613} cheking of NULL pointer done before entering here. */
                        if (test->IsDomain()) {
                            domain = test;
                            stepsCounter--;
                        }
                    }
                    objectRegistryDatabase->UnLock();
                }
            }

            if (stepsCounter > 0u) {
                REPORT_ERROR(ErrorManagement::Warning, "Find: Too many back steps in the path. The searching will start from the root");
                isSearchDomain = false;
            }
        }

    }
    // now search from the domain forward
    Reference ret;
    if (ok) {
        ReferenceContainerFilterObjectName filterName(1, ReferenceContainerFilterMode::SHALLOW, &(path.GetList()[backSteps]));
        ReferenceContainer resultSingle;

        if (isSearchDomain) {
            if (domain.IsValid()) {
                // already safe
                domain->Find(resultSingle, filterName);
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "Find: Invalid domain");
            }
        }
        else {
            // search from the beginning
        	objectRegistryDatabase->Find(resultSingle, filterName);
        }

        ok = (resultSingle.Size() > 0u);
        if (ok) {
            //Invalidate move to leafs
            ret = resultSingle.Get(resultSingle.Size() - 1u);
        }
    }
    return ret;
}

}

}
