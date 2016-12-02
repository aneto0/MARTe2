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
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

static char8 toupper(char8 c){
    if ((c >='a') && (c <='z')) {
        c = c - ('a'-'A');
    }
    return c;
}

static char8 tolower(char8 c){
    if ((c >='A') && (c <='Z')) {
        c = c + ('a'-'A');
    }
    return c;
}

/**
 * returns tokens A=array P=pointer V=Vector M=Matrix
 * returns size as size of Arrays
 * buffer is input and is modified
 */
static inline void ExtractModifier(char8 &token, bool &constant, uint32 &size,char8  * &buffer){
    token = 0;
    size = 0;
    constant = false;
    if (buffer != NULL){
        char c = *buffer++;
        constant = (c >= 'a');
        c = toupper(c);
        token = c ;
        switch (c){
        case 'A':{
            token = 'A' ;
            uint8 *pSize = reinterpret_cast<uint8 *>(buffer);
            size = *pSize;
            buffer++;
        } break;
        case 'B':{
            token = 'A' ;
            uint16 *pSize = reinterpret_cast<uint16 *>(buffer);
            size = *pSize;
            buffer+=2;
        } break;
        case 'C':{
            token = 'A' ;
            uint32 *pSize = reinterpret_cast<uint32 *>(buffer);
            size = *pSize;
            buffer+=4;
        } break;
        }
    }
}

static void AddBytesToModifiers(char8 *bytes,uint32 size, uint32 &nOfModifiers,char *&modifiers){

    uint32 len = nOfModifiers;
    uint32 newLen = nOfModifiers+size+1;
    char8 *buffer = (char8 *)realloc((void *)modifiers,newLen);
    if (buffer != NULL_PTR(char *)){
        nOfModifiers+=size;

        uint32 i;
        for (i=0;i<size;i++){
            buffer[len+i] = bytes[i];
        }
        buffer[len+size] = 0;
    }
    modifiers = buffer;

}

static void AddToModifiers(char8 c, uint32 &nOfModifiers,char *&modifiers){
    uint32 len = nOfModifiers;
    uint32 newLen = nOfModifiers+2;
    char8 *buffer = (char8 *)realloc((void *)modifiers,newLen);
    if (buffer != NULL_PTR(char *)){
        nOfModifiers++;
        buffer[len] = c;
        buffer[len+1] = 0;
    }
    modifiers = buffer;

//    printf("<%s>",buffer);

}

static uint32 GetSizeOfModifier(char8 * modifier){
//    printf("???\n");
    uint32 size = 0;
    if (modifier != NULL){
        char8 c = toupper(modifier[size]);
        while (c != 0){
//            printf("????\n");
            switch (c){
                case 'A': {
                    size+=2;
                } break;
                case 'B': {
                    size+=3;
                } break;
                case 'C': {
                    size+=5;
                } break;
                default:{
                    size++;
                }
            }
//            printf("??\n");
//            printf("{%i:%c}\n",size,c);
            c = toupper(modifier[size]);
        }
    }
    // include terminator
    return size+1;
}

static char8* CloneModifier(char8 * modifier){
//    printf("!!\n");
    uint32 size = GetSizeOfModifier(modifier);
//    printf("{sizeof=%i}\n",size);
    void * modifierCopy = NULL_PTR(void *);

    if (modifier != NULL_PTR(char8 *) ){
        modifierCopy = HeapManager::Malloc(size);
        MemoryOperationsHelper::Copy(modifierCopy,static_cast<void *>(modifier),size);
    }
    return reinterpret_cast<char8*> (modifierCopy);
}


void VariableDescriptor::GetModifier(char8 &token, bool &constant, uint32 &size,uint32 depth) const {
    char8 *buffer = modifiers;
    depth++;
    do {
        ExtractModifier(token, constant, size,buffer);
        depth--;
    } while ((token != 0) && (depth > 0));
}


void VariableDescriptor::AddGenericToModifiers(char8 token, uint32 &nOfModifiers,bool &constant){
    if (constant){
        constant = false;
        AddToModifiers(tolower(token),nOfModifiers,modifiers);
    } else {
        AddToModifiers(token,nOfModifiers,modifiers);
    }
    //printf("<%s>",modifiers);

}

void VariableDescriptor::AddArrayToModifiers(uint32 vectorSize,uint32 &nOfModifiers,bool &constant){
    char c = 0;
    if (constant){
        constant = false;
        c = ('a' - 'A');
    }
    if (vectorSize < 256u) {
        c += 'A';
        uint8 size = vectorSize;
        char8 *sizePtr = reinterpret_cast<char8*>(&size);
        AddToModifiers(c,nOfModifiers,modifiers);
        AddBytesToModifiers(sizePtr,sizeof(size),nOfModifiers,modifiers);
    } else
    if (vectorSize < 65536u) {
        c += 'B';
        uint16 size = vectorSize;
        char8 *sizePtr = reinterpret_cast<char8*>(&size);
        AddToModifiers(c,nOfModifiers,modifiers);
        AddBytesToModifiers(sizePtr,sizeof(size),nOfModifiers,modifiers);
    } else
    {
        c += 'C';
        uint32 size = vectorSize;
        char8 *sizePtr = reinterpret_cast<char8*>(&size);
        AddToModifiers(c,nOfModifiers,modifiers);
        AddBytesToModifiers(sizePtr,sizeof(size),nOfModifiers,modifiers);
    }
//    printf("<%s>",modifiers);
}



VariableDescriptor::VariableDescriptor( VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers = CloneModifier(x.modifiers);
//    if (modifiers)printf("{%s:%s}",modifiers,x.modifiers);

}

VariableDescriptor::VariableDescriptor( const VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers = CloneModifier(x.modifiers);
//    if (modifiers)printf("{%s:%s}",modifiers,x.modifiers);
}


VariableDescriptor::VariableDescriptor(){
    typeDescriptor = InvalidType;
    modifiers = NULL_PTR (char8 *);
}

VariableDescriptor::~VariableDescriptor(){
    void *modifiersP = static_cast<void *>(modifiers);
    MARTe::HeapManager::Free(modifiersP);
    modifiers = NULL_PTR(char8 *);
}




}


	
