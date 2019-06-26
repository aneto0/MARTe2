/**
 * @file ClassRegistryItem.cpp
 * @brief Source file for class ClassRegistryItem
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
 * the class ClassRegistryItem (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TypeDescriptor.h"
#include "VariableDescriptor.h"
#include "ClassRegistryItem.h"
#include "ClassMethodCaller.h"
#include "ClassRegistryIndex.h"
#include "ErrorManagement.h"
#include "LoadableLibrary.h"
#include "ObjectBuilder.h"
#include "SearchFilterT.h"
#include "ClassMethodInterfaceMapper.h"
#include "ClassMember.h"
#include "Atomic.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class Introspection;


ClassRegistryItem::ClassRegistryItem(CCString typeidNameIn,uint32 sizeOfClassIn,const ObjectBuilder * const objectBuilderIn):classMethods() {

    numberOfInstances = 0;
    loadableLibrary = NULL_PTR(LoadableLibrary *);
    objectBuilder = objectBuilderIn;

    sizeOfClass = sizeOfClassIn;
    typeidName = typeidNameIn;
    className = typeidName;
    classVersion = "";
//    static uint32 classId = 0;
    typeDescriptor.isStructuredData = true;

    ClassRegistryIndex* cri = ClassRegistryIndex::Instance();
    if (cri != NULL_PTR(ClassRegistryIndex*)) {
        typeDescriptor.structuredDataIdCode = cri->Add(this,sizeOfClassIn);
    }
}

void ClassRegistryItem::SetClassDetails(CCString classNameIn,CCString classVersionIn){
    className = classNameIn;
    classVersion = classVersionIn;
}

void ClassRegistryItem::SetObjectBuilder(const ObjectBuilder * const objectBuilderIn) {
    objectBuilder = objectBuilderIn;
}

const ObjectBuilder *ClassRegistryItem::GetObjectBuilder() const {
    return objectBuilder;
}

/*lint -e{1551} no exception should be thrown as loadableLibrary is properly initialised and
 * before deleting it is verified if the pointer is NULL*/
ClassRegistryItem::~ClassRegistryItem() {
    if (loadableLibrary != NULL_PTR(LoadableLibrary *)) {
        delete loadableLibrary;
    }

    loadableLibrary = NULL_PTR(LoadableLibrary *);
    objectBuilder = NULL_PTR(ObjectBuilder *);
}


const LoadableLibrary *ClassRegistryItem::GetLoadableLibrary() const {
    return loadableLibrary;
}

void ClassRegistryItem::SetLoadableLibrary(const LoadableLibrary * const loadLibrary) {
    this->loadableLibrary = loadLibrary;
}

void ClassRegistryItem::IncrementNumberOfInstances() {
    Atomic::Increment(&numberOfInstances);
}

void ClassRegistryItem::DecrementNumberOfInstances() {
    Atomic::Decrement(&numberOfInstances);
}

uint32 ClassRegistryItem::GetNumberOfInstances() const {
    return static_cast<uint32>(numberOfInstances);
}

CCString  ClassRegistryItem::GetClassName() const{
    return className;
}

CCString  ClassRegistryItem::GetTypeidName() const{
    return typeidName;
}

CCString  ClassRegistryItem::GetClassVersion() const{
    return classVersion;
}

uint32  ClassRegistryItem::GetSizeOfClass() const{
    return sizeOfClass;
}

const TypeDescriptor & ClassRegistryItem::GetTypeDescriptor() const {
    return typeDescriptor;
}

ClassMethodCaller *ClassRegistryItem::FindMethod(CCString methodName) {

    uint32 i = 0u;
    uint32 end = classMethods.ListSize();
    ClassMethodCaller *caller = NULL_PTR(ClassMethodCaller *);
    while ((i < end) && (caller == NULL_PTR(ClassMethodCaller *))) {
        ClassMethodInterfaceMapper *cmim = classMethods.ListPeek(i);
        if (cmim != NULL) {
            CCString mapperMethodName = cmim->GetMethodName();
            if (mapperMethodName == methodName) {
                caller = cmim->GetMethodCaller();
            }
        }
        i++;
    }
    return caller;
}


void ClassRegistryItem::AddMethod(ClassMethodInterfaceMapper * const method) {
    if (method != NULL) {
        classMethods.ListAdd(method);
    }
}


class ClassRegistryItemFindMember: public SearchFilterT<ClassMember>{
	CCString const memberName;

public:
	ClassRegistryItemFindMember(CCString const memberNameIn): memberName(memberNameIn){}
	bool Test(ClassMember *data){
	    bool ret = (data != NULL);
	    if (ret){
	    	CCString name = data->GetName();
	    	ret =  (name == memberName) ;
	    }
	    return ret;
	}
private:
	void operator=(const ClassRegistryItemFindMember &){}
};

class ClassRegistryItemFindInheritedMember: public SearchFilterT<ClassMember>{
	CCString const memberName;
	// the search will return the node referring to the inherited class
	// this will point to the correct node
	const ClassMember *actualMember;
public:

	ClassRegistryItemFindInheritedMember(CCString const memberNameIn): memberName(memberNameIn){
		actualMember = NULL_PTR(ClassMember *);
		// find structure documentation
	}

	const ClassMember * GetActualMember(){
		return actualMember;
	}

	bool Test(ClassMember *data){
	    if (data != NULL) {
	    	CCString name = data->GetName();
	    	if (name.GetSize() == 0){
	    		TypeDescriptor td = data->GetDescriptor().GetSummaryTypeDescriptor();
	    		ClassRegistryItem *cri = ClassRegistryDatabase::Find(td);

	    		if (cri != NULL){
	    			actualMember = cri->FindMember(memberName);
//printf ("find %s::%s\n",cri->GetClassName(),memberName );
	    		}
	    	}
	    }
	    return (actualMember != NULL_PTR(ClassMember *));
	}

private:
	void operator=(const ClassRegistryItemFindInheritedMember &){}
};


ClassMember const *ClassRegistryItem::FindMember(CCString memberName) {
	ClassRegistryItemFindMember crifm (memberName);
	const ClassMember *found = classMembers.ListSearch(&crifm);
	if (found == NULL_PTR(ClassMember *)){
		ClassRegistryItemFindInheritedMember crifim(memberName);
		found = classMembers.ListSearch(&crifim);
		// take the actual value found by recursion
		if (found != NULL_PTR(ClassMember *)){
			found = crifim.GetActualMember();
		}
	}
	return found;
}

ClassMember const *ClassRegistryItem::FindMember(uint32 index) {

    ClassMember const *member = NULL_PTR(ClassMember const *);
    if  (index < classMembers.ListSize()){
        member = classMembers.ListPeek(index);
    }
    return member;
}


void ClassRegistryItem::AddMember(ClassMember * const method) {
    if (method != NULL) {
        classMembers.ListAdd(method);
    }
}


}
