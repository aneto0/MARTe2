/**
 * @file ClassMethodsRegistryItem.cpp
 * @brief Source file for class ClassMethodsRegistryItem
 * @date Apr 12, 2016
 * @author fsartori
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

 * @details This source file contains the definition of all the methods for
 * the class ClassMethodsRegistryItem (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodsRegistryItem.h"
#include "StringHelper.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * TODO
 * */
int ClassMethodsRegistryItem::Find(const char8 *name){
    const char8 *names = functionNames;

    uint32 nameSize = StringHelper::Length(name);
    uint32 namesSize = StringHelper::Length(names);

    bool notFound = true;
    int functionIndex = 0;
    while ((namesSize > nameSize) && notFound){


        //Skipping of Class::
       	while ((*names !=':') && (*names !=0)){
		names++;
       	}
        if (*names == ':') names++;
        if (*names == ':') names++;


        //Matching of function,
        notFound = (StringHelper::CompareN(names,name,nameSize)!=0) ||
                 ((names[nameSize]!= 0) && (names[nameSize]!= ','));
        if (notFound){
            functionIndex++;
            names += nameSize;
            while ((names[0] != 0) && (names[0] != ',')) names++;
            namesSize = StringHelper::Length(names);
        }
    }
    if (notFound){
        functionIndex = -1;
    }
    return functionIndex;
}

/**
 * TODO
 * */
ClassMethodInterfaceMapper *ClassMethodsRegistryItem::FindFunction(const char8 *name){
    int functionIndex = Find(name);
    ClassMethodInterfaceMapper *fmp = NULL;
    if (functionIndex >= 0){
        fmp = &functionTable[functionIndex];
    }
    return fmp;
}

/**
 * TODO
 * */
ClassMethodsRegistryItem::ClassMethodsRegistryItem(ClassRegistryItem *cri,  ClassMethodInterfaceMapper * const functionTable_In,const char *functionNames_In):
        functionTable(functionTable_In){
    ;
    functionNames = functionNames_In;
    // register in Object the record
    if (cri != NULL){
        cri->RegisterMethods(this);
    }
}

ClassMethodsRegistryItem::~ClassMethodsRegistryItem() {
    /*
     * TODO
     */
}

/**
 * TODO
 * */
ReturnType ClassMethodsRegistryItem::CallFunction(Object * context, const char8 *name, ReferenceContainer &ref){
    ReturnType returnValue(true);

    if (context == NULL)  returnValue.error.notParametersError = false;
    if (name == NULL)     returnValue.error.notParametersError = false;


    ClassMethodInterfaceMapper * fmp = NULL;
    if (returnValue.AllOk()){
        fmp = FindFunction(name);
        if (fmp == NULL) returnValue.error.notUnsupportedFeature = false;
    }

    if (returnValue.AllOk()){
        returnValue = fmp->Call(context,ref);
    }

    return returnValue;
}

}

#if 0

namespace MARTe {

#include "Object.h"
//#if 0
class Dummy:public Object{

public:

    CLASS_REGISTER_DECLARATION()

    bool Test(MARTe::ReferenceContainer &ref){
          return true;
    }

    bool Test2(int i){
          return true;
    }

};

ClassMethodInterfaceMapper CMIM(&Dummy::Test);
ClassMethodInterfaceMapper CMIMS[] = {&Dummy::Test,&Dummy::Test};

CLASS_METHOD_REGISTER(Dummy,&Dummy::Test)


}
#endif
	
