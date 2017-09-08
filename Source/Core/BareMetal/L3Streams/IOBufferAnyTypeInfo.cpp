/**
 * @file IOBufferAnyTypeInfo.cpp
 * @brief Header file for class AnyType
 * @date 7 Sep 2017
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#include "IOBuffer.h"
#include "AnyType.h"
#include "FormatDescriptor.h"
#include "DynamicCString.h"
#include "BasicType.h"

namespace MARTe{


static void PrintTypeDescriptor(DynamicCString &line,const TypeDescriptor &td){
    const char8 *constString2= "const ";
    const char8 *constString= "";
    if (td.dataIsConstant){
    	constString = constString2;
    }

    if (td.isStructuredData){
    	line.AppendN(constString);
   		ClassRegistryItem * cri = ClassRegistryDatabase::Instance()->Find(td);
        if (cri != NULL) {
        	line.AppendN(cri->GetClassName());
        }
        else {
        	line.AppendN("unknown_struct_code(");
        	uint32 idCode = td.structuredDataIdCode;
        	line.AppendNum(idCode);
        	line.Append(')');
        }
    } else {
    	if (td.IsComplexType()){
        	line.AppendN(constString);
        	line.AppendN(BasicTypeName(td.type,td.complexType));
    	} else
        if (td.IsBitType()){
        	line.AppendN(constString);
        	line.AppendN(BasicTypeName(td.type,0));
        	uint32 numberOfBits = (int)td.numberOfBits;
        	uint32 bitOffset = (int)td.bitOffset;
        	line.AppendNum(numberOfBits);
        	line.Append(':');
        	line.AppendNum(bitOffset);
        } else
        if (td.arrayProperty == SizedCArray_AP){
           	if (td.arraySize == 0){
            	line.AppendN(constString);
            	line.AppendN(BasicTypeName(td.type,0));
            	uint32 bits = BitsFromBasicObjectSize((int)td.objectSize);
            	line.AppendNum(bits);
            	line.AppendN(" [>2M]");
           	} else
           	if (td.arraySize == 1){
            	line.AppendN(constString);
            	line.AppendN(BasicTypeName(td.type,0));
            	uint32 bits = BitsFromBasicObjectSize((int)td.objectSize);
            	line.AppendNum(bits);
           	} else
           	{
            	line.AppendN(constString);
            	line.AppendN(BasicTypeName(td.type,0));
            	uint32 bits = BitsFromBasicObjectSize((int)td.objectSize);
            	uint32 size = td.arraySize;
            	line.AppendNum(bits);
            	line.AppendN(" [");
            	line.AppendNum(size);
            	line.Append(']');
           	}
        } else
        if ((td.arrayProperty == StaticZeroTermArray_AP) || (td.arrayProperty == ConstStaticZeroTermArray_AP)){
        	if (td.arrayProperty == ConstStaticZeroTermArray_AP){
            	line.AppendN(constString2);
        	}
        	line.AppendN("StaticZeroTerminatedArray<");
        	line.AppendN(constString);
        	line.AppendN(BasicTypeName(td.type,0));
        	uint32 bits = BitsFromBasicObjectSize((int)td.objectSize);
        	line.AppendNum(bits);
           	if (td.arraySize == 0){
            	line.AppendN(",>2M>");
           	} else {
            	uint32 size = td.arraySize;
            	line.Append(',');
            	line.AppendNum(size);
            	line.Append('>');
           	}
        }  else {
        	uint32 temp = td.combinedArrayType;
        	switch(temp){
            case ConstZeroTermArray:
            case ZeroTermArray:
            case DynamicZeroTermArray:
            case ConstDynamicZeroTermArray:
				{
            	if ((temp == ConstZeroTermArray) || (temp == ConstDynamicZeroTermArray)){
                	line.AppendN(constString2);
            	}
            	if ((temp == DynamicZeroTermArray) || (temp == ConstDynamicZeroTermArray)){
                	line.AppendN("Dynamic");
            	}
            	line.AppendN("ZeroTerminatedArray<");
            	line.AppendN(constString);
            	line.AppendN(BasicTypeName(td.type,0));
            	uint32 bits = BitsFromBasicObjectSize((int)td.objectSize);
            	line.AppendNum(bits);
            	line.Append('>');
            }break;
            case ConstArray1D:
            case Array1D:{
            	if (temp == ConstArray1D) {
                	line.AppendN(constString2);
            	}
            	line.AppendN("Vector<");
            	line.AppendN(constString);
            	line.AppendN(BasicTypeName(td.type,0));
            	uint32 bits = BitsFromBasicObjectSize((int)td.objectSize);
            	line.AppendNum(bits);
            	line.Append('>');

            }break;
            case ConstArray2D:
            case Array2D:{
            	if (temp == ConstArray2D) {
                	line.AppendN(constString2);
            	}
            	line.AppendN("Matrix<");
            	line.AppendN(constString);
            	line.AppendN(BasicTypeName(td.type,0));
            	uint32 bits = BitsFromBasicObjectSize((int)td.objectSize);
            	line.AppendNum(bits);
            	line.Append('>');
            }break;
            case ConstPointerArray:
            case PointerArray:{
            	if (temp == ConstPointerArray) {
                	line.AppendN(constString2);
            	}
            	line.Append('(');
            	line.AppendN(constString);
            	line.AppendN(BasicTypeName(td.type,0));
            	uint32 bits = BitsFromBasicObjectSize((int)td.objectSize);
            	line.AppendNum(bits);
            	line.AppendN(")*");
            }break;
            default:
            case ArrayUnknown:{
            	line.Append(' ');
            	line.AppendN(constString);
            	line.AppendN(BasicTypeName(td.type,0));
            	uint32 bits = BitsFromBasicObjectSize((int)td.objectSize);
            	line.AppendNum(bits);
            	line.AppendN(" ?");
            }break;
            }
        }
    }

}


bool IOBuffer::PrintAnyTypeInfo(IOBuffer &iobuff, FormatDescriptor fd, const AnyType & parIn){
	DynamicCString line;

    const VariableDescriptor &vd = parIn.GetFullVariableDescriptor();

    TypeDescriptor td;
    const void* dataPointer = NULL;
    bool ret = vd.GetTopTypeDescriptor(td,0);

    if (ret){
    	PrintTypeDescriptor(line,td);
        uint32 len = line.GetSize();

    	if (fd.size > len){

    	    // wants padding
    	    if (fd.padded){
    	    	// needs padding
            	if (fd.leftAligned){
            		ret = iobuff.WriteAll(line.GetList(),len);
            		/// write N padding

    	    	} else {
            		/// write N padding
    	    		ret = iobuff.WriteAll(line.GetList(),len);
    	    	}
    	    }

    	} else {  //fd.size <= len  // truncate
    		ret = iobuff.WriteAll(line.GetList(),fd.size-1);
    		ret = ret && iobuff.PutC('?');
    	}
	}

	return ret;

}


} //MARTe
