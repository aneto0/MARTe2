/**
 * @file IOBufferAnyTypePrint.cpp
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
#define DLL_API
#include "AnyType.h"
#include "IOBufferPrivate.h"

namespace MARTe{


/*
 * @brief The function called by all Printf operations.
 * @details This function read the format, builds the related format
 * descriptor and then calls the PrintToStream function passing the
 * next AnyType element in the list.
 * @param[in] format is a printf-like string format.
 * @param[in] pars is a list of AnyType elements to print.
 * @return false in case of errors.
 */
DLL_API bool IOBuffer::PrintAnyType(IOBuffer &iobuff, FormatDescriptor fd, const AnyType & parIn){



	bool ret = true;
	// void anytype
	AnyType par = parIn;


	if (fd.desiredAction == PrintInfo) {
		ret = PrintAnyTypeInfo(iobuff, fd, parIn);
	} else
		if (fd.desiredAction == PrintStruct) {
			ret = false; // not supported yet
		} else
		{
			// extract variable descriptor
			const VariableDescriptor &vd = parIn.GetFullVariableDescriptor();
			const void* dataPointer = NULL;
			TypeDescriptor td;
			// extract typedescriptor
			td = vd.GetSummaryTypeDescriptor();

			if (ret){
				dataPointer = parIn.GetVariablePointer();
				ret = (dataPointer != NULL);
			}

			if (ret) {  // main block
				// if cannot deal with type just print "(actual type)?"
				bool printAsACast = false;
				//if the element is structured, the print is not supported.
				if ((td.isStructuredData) ||
						(td.IsSpecialType()))  //Stream,StructuredData,..
				{
					printAsACast = true;
				} // not a structured data
				else
				{
					if (td.IsBitType()){  // uint5 bitranges
						uint8 numberOfBits_ = static_cast<uint8>(td.numberOfBits);
						uint8 bitOffset_    = static_cast<uint8>(td.bitOffset);

						// use native standard integer
						const uint32 *number = static_cast<const uint32 *>(dataPointer);
						ret = BitSetToStream(iobuff, number, bitOffset_, numberOfBits_, false, fd);
					} else {
						TDBasicTypeSize basicTypeSize = static_cast<TDBasicTypeSize>(td.basicTypeSize);
						TD_FullType fullType = static_cast<TD_FullType>(td.fullType);
						switch (fullType){
						case TDF_UnsignedInteger:{
							switch (basicTypeSize) {
							case Size8bit: {
								const uint8 *data = static_cast<const uint8 *>(dataPointer);
								ret = IntegerToStream(iobuff, *data, fd);
							}
							break;
							case Size16bit: {
								const uint16 *data = static_cast<const uint16 *>(dataPointer);
								ret = IntegerToStream(iobuff, *data, fd);
							}
							break;
							case Size32bit: {
								const uint32 *data = static_cast<const uint32 *>(dataPointer);
								ret = IntegerToStream(iobuff, *data, fd);
							}
							break;
							case Size64bit: {
								const uint64 *data = static_cast<const uint64 *>(dataPointer);
								ret = IntegerToStream(iobuff, *data, fd);
							}
							break;
							default: {
								printAsACast = true;
							}
							}  // end case
						}break;
						case TDF_SignedInteger:{
							switch (basicTypeSize) {
							case Size8bit: {
								const int8 *data = static_cast<const int8 *>(dataPointer);
								ret = IntegerToStream(iobuff, *data, fd);
							}
							break;
							case Size16bit: {
								const int16 *data = static_cast<const int16 *>(dataPointer);
								ret = IntegerToStream(iobuff, *data, fd);
							}
							break;
							case Size32bit: {
								const int32 *data = static_cast<const int32 *>(dataPointer);
								ret = IntegerToStream(iobuff, *data, fd);
							}
							break;
							case Size64bit: {
								const int64 *data = static_cast<const int64 *>(dataPointer);
								ret = IntegerToStream(iobuff, *data, fd);
							}
							break;
							default: {
								printAsACast = true;
							}
							}

						}break;

						case TDF_Float:{
							switch (basicTypeSize){
							case Size32bit: {
								const float32 *data = static_cast<const float32 *>(dataPointer);
								ret = FloatToStream(iobuff, *data, fd);
							}
							break;
							case Size64bit: {
								const float64 *data = static_cast<const float64 *>(dataPointer);
								ret = FloatToStream(iobuff, *data, fd);
							}
							break;
							default:{
								printAsACast = true;
							}
							}// end switch

						}break;

						case TDF_Char:{
							switch (basicTypeSize){
							case Size8bit: {
								const char8 *data = static_cast<const char8 *>(dataPointer);
								iobuff.PutC(*data);
							}
							break;
							default:{
								printAsACast = true;
							}
							}// end switch
						}break;
						case TDF_DynamicCString:
						case TDF_CString:
						case TDF_CCString:{
							const CCString *data = static_cast<const CCString *>(dataPointer);
							ret = PrintCCStringFit(iobuff,*data,fd);
						}break;
						case TDF_Pointer:{
							const uintp *data = static_cast<const uintp *>(dataPointer);
							ret = IntegerToStream(iobuff, *data, fd);
						}break;
						default:{
							printAsACast = true;
						}
						}  // end case
					}
				}
				// print this as default
				if (printAsACast){
					DynamicCString line;
					line.Append('(');
					ret = vd.ToString(line);
					line.Append(')');
					line.Append('?');

					if (ret){
						ret = PrintCCStringFit(iobuff,line,fd);
					}
				}
			} // main block

		}  // desired action
	//REPORT_ERROR(UnsupportedError,"unsupported format")
	return ret;

}


} //MARTe
