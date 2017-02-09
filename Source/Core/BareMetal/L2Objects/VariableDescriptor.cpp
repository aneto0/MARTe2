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
#include "DynamicCString.h"

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

uint32 FromHex(char8 c){
	uint32 ret = 0u;
	if ((c >='0') && (c <='9')){
		ret  = c - '0';
	} else
		if ((c >='A') && (c <='F')){
			ret  = c - 'A' + 10u;
		}
	return ret;
}

static uint32 ReadHex(char8 *&buffer,uint32 nOfChars){
	uint32 i=0u;

	uint32 ret = 0u;
	for (i=0u;i<nOfChars;i++){
		uint32 n = FromHex(*buffer++);
		ret+= n << (4u*i);
	}
	return ret;
}


/**
 * returns tokens A=array P=pointer V=Vector M=Matrix
 * returns size as size of Arrays
 * buffer is input and is modified
 */
static void BrowseModifierBuffer(char8 &token, bool &constant, uint32 &size,char8  *&buffer){
    token = 0;
    size = 0;
    constant = false;
    if (buffer != NULL){
        char8 c = *buffer++;
        constant = (c >= 'a');
        c = toupper(c);
        token = c ;
        switch (c){
        case '\0':{
        	buffer--;
//        	index--;
        	token = '\0';
        	size = 0;
        } break;
        case 'A':{
            token = 'A' ;
            size = ReadHex(buffer,2);
//            uint8 *pSize = reinterpret_cast<uint8 *>(buffer+index);
//            size = *pSize;
//            index++;
        } break;
        case 'B':{
            token = 'A' ;
//            uint16 *pSize = reinterpret_cast<uint16 *>(buffer+index);
//            size = *pSize;
//            index+=2;
            size = ReadHex(buffer,4);
        } break;
        case 'C':{
            token = 'A' ;
//            uint32 *pSize = reinterpret_cast<uint32 *>(buffer+index);
//            size = *pSize;
//            index+=4;
            size = ReadHex(buffer,8);

        } break;
        }
    }

}

void VariableDescriptor::GetModifier(char8 &token, bool &constant, uint32 &size,uint32 depth) const {
    char8 *buffer = modifiers.GetList();
    depth++;
    do {
    	//printf ("[%i]<%s>\n",depth-1,buffer);
        BrowseModifierBuffer(token, constant, size,buffer);
        depth--;
    } while ((token != 0) && (depth > 0));
}


void VariableDescriptor::AddGenericToModifiers(char8 token, uint32 &nOfModifiers,bool &constant){
    if (constant){
        constant = false;
        modifiers.Append(tolower(token));
//        AddToModifiers(tolower(token),nOfModifiers,modifiers);
    } else {
        modifiers.Append(token);
//        AddToModifiers(token,nOfModifiers,modifiers);
    }
}

static char8 ToHex(uint32 nibble){
	char8 ret = '?';
	if (nibble <10){
		ret = '0'+nibble;
	} else
		if (nibble <16){
			ret = 'A'+nibble - 10;
		}
	return ret;
}

static void AppendHex(DynamicCString &s, uint32 n, uint32 size){
	uint32 i;
	for (i=0;i<size;i++){
		uint32 nibble = n & 0x0000000F;
		n >>=4;
		s.Append(ToHex(nibble));
	}
}

void VariableDescriptor::AddArrayToModifiers(uint32 vectorSize,uint32 &nOfModifiers,bool &constant){
    char c = 0;
    if (constant){
        constant = false;
        c = ('a' - 'A');
    }
    if (vectorSize < 256u) {
        c += 'A';
//        uint8 size = vectorSize;
//        char8 *sizePtr = reinterpret_cast<char8*>(&size);
        modifiers.Append(c);
        AppendHex(modifiers,vectorSize,2);
//        modifiers.Append(static_cast<char8>(size));
//        AddToModifiers(c,nOfModifiers,modifiers);
//        AddBytesToModifiers(sizePtr,sizeof(size),nOfModifiers,modifiers);
    } else
    if (vectorSize < 65536u) {
        c += 'B';
//        uint16 size = vectorSize;
//        char8 *sizePtr = reinterpret_cast<char8*>(&size);
//        modifiers.Append(c);
//        modifiers.Append(sizePtr[0]);
//        modifiers.Append(sizePtr[1]);
        modifiers.Append(c);
        AppendHex(modifiers,vectorSize,4);
//        AddToModifiers(c,nOfModifiers,modifiers);
//        AddBytesToModifiers(sizePtr,sizeof(size),nOfModifiers,modifiers);
    } else
    {
        c += 'C';
//        uint32 size = vectorSize;
//        char8 *sizePtr = reinterpret_cast<char8*>(&size);
        modifiers.Append(c);
//        modifiers.Append(sizePtr[0]);
//        modifiers.Append(sizePtr[1]);
//        modifiers.Append(sizePtr[2]);
//        modifiers.Append(sizePtr[3]);
        AppendHex(modifiers,vectorSize,8);
//        AddToModifiers(c,nOfModifiers,modifiers);
//        AddBytesToModifiers(sizePtr,sizeof(size),nOfModifiers,modifiers);
    }
}


VariableDescriptor::VariableDescriptor(){
    typeDescriptor = InvalidType;
//    modifiers = NULL_PTR (char8 *);
}

VariableDescriptor::~VariableDescriptor(){
//    void *modifiersP = static_cast<void *>(modifiers);
//    MARTe::HeapManager::Free(modifiersP);
//    modifiers = NULL_PTR(char8 *);
}

VariableDescriptor::VariableDescriptor(const TypeDescriptor &td){
    typeDescriptor = td;
//    modifiers = NULL_PTR(char8 *);
}

VariableDescriptor::VariableDescriptor( VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
//    modifiers = CloneModifier(x.modifiers);
    modifiers.AppendN(x.modifiers.GetList());
}

VariableDescriptor::VariableDescriptor( const VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
//    modifiers = CloneModifier(x.modifiers);
    modifiers.AppendN(x.modifiers.GetList());

}

const TypeDescriptor &VariableDescriptor::GetFullTypeDescriptor() const {
    return typeDescriptor;
}


}


	
