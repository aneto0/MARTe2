/**
 * @file VariableDescriptor.cpp
 * @brief Source file for class VariableDescriptor
 * @date Nov 29, 2016
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
 * the class VariableDescriptor (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "VariableDescriptor.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{



void VariableDescriptor::AddToModifiers(uint32 n,uint32 &nOfModifiers){

    uint32 test = 1;
    while (test <= n) test *=10;
    test/=10;
    do {
        char8 c = (n /test) +'0';
        AddToModifiers(c,nOfModifiers);
        n = n % test;
        test/=10;
    }  while (test > 1 );

}


void VariableDescriptor::AddToModifiers(char8 c,uint32 &nOfModifiers){
    uint32 len = nOfModifiers;
    uint32 newLen = nOfModifiers+2;
    char8 *buffer = (char8 *)realloc((void *)modifierString.GetList(),newLen);
    if (buffer != NULL_PTR(char *)){
        nOfModifiers+=2;
        buffer[len] = c;
        buffer[len+1] = 0;
    }
    modifierString.SetList(buffer);

}



VariableDescriptor::VariableDescriptor( VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifierString = StringHelper::StringDup(CCString(x.modifierString));
}

VariableDescriptor::VariableDescriptor( const VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifierString = StringHelper::StringDup(CCString(x.modifierString));
}


VariableDescriptor::VariableDescriptor(){
    typeDescriptor = InvalidType;
}

VariableDescriptor::~VariableDescriptor(){
    if (modifierString.GetList() != NULL){
        void *p = static_cast<void *>(modifierString.GetList());
        MARTe::HeapManager::Free(p);
        modifierString.SetList(static_cast<char8 *>(p));
    }
}




}


	
