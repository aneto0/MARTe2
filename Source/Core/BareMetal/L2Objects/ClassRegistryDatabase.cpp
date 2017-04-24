/**
 * @file ClassRegistryDatabase.cpp
 * @brief Source file for class ClassRegistryDatabase
 * @date 04/08/2015
 * @author Andre Neto
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
 * the class ClassRegistryDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryItem.h"
#include "HeapManager.h"
#include "Object.h"
#include "DynamicCString.h"
#include "StringHelper.h"
#include "MemoryOperationsHelper.h"
#include "LoadableLibrary.h"
#include "ClassRegistryDatabase.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ClassRegistryDatabase *ClassRegistryDatabase::Instance() {
    static ClassRegistryDatabase *instance = NULL_PTR(ClassRegistryDatabase *);
    if (instance == NULL_PTR(ClassRegistryDatabase *)) {
        instance = new ClassRegistryDatabase();
        GlobalObjectsDatabase::Instance()->Add(instance, NUMBER_OF_GLOBAL_OBJECTS - 2u);
    }
    return instance;
}

ClassRegistryDatabase::ClassRegistryDatabase() {

}

ClassRegistryDatabase::~ClassRegistryDatabase() {

}

void ClassRegistryDatabase::Add(ClassRegistryItem * const p) {
    if (p != NULL) {

        if (!Lock()) {
            REPORT_ERROR(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
        }

        // add at the end
        classDatabase.ListAdd(p);

        UnLock();
    }
}

/**************************************************************/

// TODO
class ClassRegistryDatabaseFindByTypeDescriptor: public SearchFilterT<ClassRegistryItem>{
	TypeDescriptor td;

public:
	ClassRegistryDatabaseFindByTypeDescriptor(const TypeDescriptor &tdIn): td (tdIn){}
	bool Test(ClassRegistryItem *data){
	    bool ret = (data != NULL);
	    if (ret){
	    	ret = (data->GetTypeDescriptor() == td);
	    }
	    return ret;
	}
};

// TODO
class ClassRegistryDatabaseFindByTypeId: public SearchFilterT<ClassRegistryItem>{
	CCString const typeidName;

public:
	ClassRegistryDatabaseFindByTypeId(CCString const typeidNameIn): typeidName(typeidNameIn){}
	bool Test(ClassRegistryItem *data){
	    bool ret = (data != NULL);
	    if (ret){
	    	ret =  (StringHelper::Compare(data->GetTypeidName(), typeidName) == 0) ;
	    }
	    return ret;
	}
};

// TODO
class ClassRegistryDatabaseFindByClassName: public SearchFilterT<ClassRegistryItem>{
	CCString const className;

public:
	ClassRegistryDatabaseFindByClassName(CCString const classNameIn): className(classNameIn){}
	bool Test(ClassRegistryItem *data){
	    bool ret = (data != NULL);
	    if (ret){
	    	ret =  (StringHelper::Compare(data->GetClassName(), className) == 0) ;
	    }
	    return ret;
	}
};


/**************************************************************/

ClassRegistryItem *ClassRegistryDatabase::Find( SearchFilterT<ClassRegistryItem> &  finder) {
	ClassRegistryItem *registryItem = NULL_PTR(ClassRegistryItem *);
	if (!Lock()) {
		REPORT_ERROR(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
	} else {
		registryItem = classDatabase.ListSearch(&finder);

		UnLock();
	}
	return registryItem;
}


ClassRegistryItem *ClassRegistryDatabase::Find(const TypeDescriptor &targetTd) {
	return Find(ClassRegistryDatabaseFindByTypeDescriptor( targetTd));
}

ClassRegistryItem *ClassRegistryDatabase::FindTypeIdName(CCString const typeidName) {
	return Find(ClassRegistryDatabaseFindByTypeId( typeidName));
}

ClassRegistryItem *ClassRegistryDatabase::Find(CCString className) {

	// search full name
	ClassRegistryItem *cri = Find(ClassRegistryDatabaseFindByClassName( className));
	if (cri == NULL){

		// search class within container container::class
		CCString classOnlyPartName = StringHelper::SearchString(className, "::");
		if (classOnlyPartName.GetSize() > 0){
			cri = Find(ClassRegistryDatabaseFindByClassName(classOnlyPartName.GetList()+2));

			// there was a container - search and load a dll with this name
			if (cri == NULL){
				// extract the first part up to ::
				DynamicCString dllName;
				uint32 dllNameBaseSize = className.GetSize() - classOnlyPartName.GetSize();
				dllName.AppendN(className.GetList(),dllNameBaseSize);

				// try open a dll with this name
	            LoadableLibrary *loader = new LoadableLibrary();
	            bool dllOpened = false;
	            uint32 osExtIndex = 0;
	            //Check for all known operating system extensions.
	            while ((operatingSystemDLLExtensions[osExtIndex] != 0) && (!dllOpened)) {
	                dllName.Truncate(dllNameBaseSize);
					dllName.AppendN(operatingSystemDLLExtensions[osExtIndex]);
					osExtIndex++;
	                dllOpened = loader->Open(dllName.GetList());
	            }

	            //If the dll was successfully opened than it is likely that more classes were registered
	            //in the database. Search again.
	            if (dllOpened) {
	    			cri = Find(ClassRegistryDatabaseFindByClassName(classOnlyPartName.GetList()+2));

	    			// if found store the dll object with the database - to be destroyed later
	    			if (cri != NULL){
                        cri->SetLoadableLibrary(loader);
                        loader = NULL;
	    			}
	            }

	            // if still pointing to loader object than destroy it
	            if (loader != NULL){
	                delete loader;
	            }
			}
		}
	}

	return cri;
}


uint32 ClassRegistryDatabase::GetSize() {
    uint32 size = 0u;
    if (Lock()) {
        size = classDatabase.ListSize();
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
    }
    UnLock();
    return size;
}

const ClassRegistryItem *ClassRegistryDatabase::Peek(const uint32 &idx) {
    ClassRegistryItem *item = NULL_PTR(ClassRegistryItem *);
    if (Lock()) {
        item = classDatabase.ListPeek(idx);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
    }
    UnLock();
    return item;
}

CCString const ClassRegistryDatabase::GetClassName() const {
    return "ClassRegistryDatabase";
}

void ClassRegistryDatabase::CleanUp() {
    classDatabase.CleanUp();
}

bool ClassRegistryDatabase::Lock() {
    return (mux.FastLock() == ErrorManagement::NoError);
}

void ClassRegistryDatabase::UnLock() {
    mux.FastUnLock();
}

}
