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
#include "Anytype.h"
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


    if (ret){
        if (fd.desiredAction == PrintInfo) {
        	ret = PrintAnyTypeInfo(iobuff, fd, parIn);
        } else
        if (fd.desiredAction == PrintStruct) {
        	ret = false; // not supported yet
        } else
        {

            const VariableDescriptor &vd = parIn.GetFullVariableDescriptor();
            const void* dataPointer = NULL;
            TypeDescriptor td;
            ret = vd.GetTopTypeDescriptor(td,0);

            if (ret){
            	dataPointer = parIn.GetVariablePointer();
            	ret = (dataPointer != NULL);
            }

            if (ret) {
                //if the element is structured, the print is not supported.
            	if (td.isStructuredData){




                    if (fd.desiredAction != PrintAnything) {
                        if (fd.desiredAction != PrintStruct) {
                            REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a struct will be printed");
                        }
                    }
                    CCString className;
               		ClassRegistryItem * cri = ClassRegistryDatabase::Instance()->Find(td);
                    if (cri != NULL) {
                        ret = iobuff.PutC('(*(');
                        ret |= iobuff.PutS(cri->GetClassName());
                        ret |= iobuff.PutS(" *)0x");
                        ret |= PointerToStream(iobuff,dataPointer );
                        ret |= iobuff.PutS(" )");
                    } else {
                        ret = iobuff.PutS("(*(unknown_struct_code(");
                        ret |= IntToStream(iobuff,(int)td.structuredDataIdCode);
                        ret |= iobuff.PutS(") *)0x");
                        ret |= PointerToStream(iobuff,dataPointer );
                        ret |= iobuff.PutS(" )");
                    }
            	} // not a structured data
                else {

                    if (td.type == UnsignedInteger) {
                        if (fd.desiredAction != PrintAnything) {
                            if (fd.desiredAction != PrintInteger) {
                                REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: an unsigned integer will be printed");
                            }
                        }
                        //native unsigned integer types.
                        if (par.GetBitAddress() == 0u) {
                            switch ((par.GetTypeDescriptor()).numberOfBits) {
                                case 8u: {
                                    uint8 *data = static_cast<uint8 *>(dataPointer);
                                    ret = IntegerToStream(iobuff, *data, fd);
                                }
                                break;
                                case 16u: {
                                    uint16 *data = static_cast<uint16 *>(dataPointer);
                                    ret = IntegerToStream(iobuff, *data, fd);
                                }
                                break;
                                case 32u: {
                                    uint32 *data = static_cast<uint32 *>(dataPointer);
                                    ret = IntegerToStream(iobuff, *data, fd);
                                }
                                break;
                                case 64u: {
                                    uint64 *data = static_cast<uint64 *>(dataPointer);
                                    ret = IntegerToStream(iobuff, *data, fd);
                                }
                                break;
                                default: {
                                    // use native standard integer
                                    uint32 *number = static_cast<uint32 *>(dataPointer);
                                    // all the remaining cases here
                                    uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                                    ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, false, fd);
                                }
                            }
                        }
                        else {
                            // use native standard integer
                            uint32 *number = static_cast<uint32 *>(dataPointer);
                            // all the remaining cases here
                            uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                            ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, false, fd);
                        }
                    }



                    if (((par.GetTypeDescriptor()).type) == SignedInteger) {
                        if (fd.desiredAction == PrintInfo) {
                            const char8* infoName = "Signed Integer";
                            AnyType info = infoName;
                            FormatDescriptor newFD = fd;
                            newFD.desiredAction = PrintString;
                            ret = PrintToStreamScalar(iobuff, info, newFD);
                        }
                        else {
                            if (fd.desiredAction != PrintAnything) {
                                if (fd.desiredAction != PrintInteger) {
                                    REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a signed integer will be printed");
                                }
                            }
                            //native signed integer types.
                            if (par.GetBitAddress() == 0u) {
                                switch ((par.GetTypeDescriptor()).numberOfBits) {
                                    case 8u: {
                                        int8 *data = static_cast<int8 *>(dataPointer);
                                        ret = IntegerToStream(iobuff, *data, fd);
                                    }
                                    break;
                                    case 16u: {
                                        int16 *data = static_cast<int16 *>(dataPointer);
                                        ret = IntegerToStream(iobuff, *data, fd);
                                    }
                                    break;
                                    case 32u: {
                                        int32 *data = static_cast<int32 *>(dataPointer);
                                        ret = IntegerToStream(iobuff, *data, fd);
                                    }
                                    break;
                                    case 64u: {
                                        int64 *data = static_cast<int64 *>(dataPointer);
                                        ret = IntegerToStream(iobuff, *data, fd);
                                    }
                                    break;
                                    default: {
                                        // use native standard integer
                                        uint32 *number = static_cast<uint32 *>(dataPointer);
                                        uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                                        // all the remaining cases here
                                        ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, true, fd);
                                    }
                                }
                            }
                            else {
                                // use native standard integer
                                uint32 *number = static_cast<uint32 *>(dataPointer);
                                uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                                // all the remaining cases here
                                ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, true, fd);
                            }
                        }
                    }


                    if (((par.GetTypeDescriptor()).type) == Float) {
                        if (fd.desiredAction == PrintInfo) {
                            const char8* infoName = "Float";
                            AnyType info = infoName;
                            FormatDescriptor newFD = fd;
                            newFD.desiredAction = PrintString;
                            ret = PrintToStreamScalar(iobuff, info, newFD);
                        }
                        else {
                            if (fd.desiredAction != PrintAnything) {
                                if (fd.desiredAction != PrintFloat) {
                                    REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a float number will be printed");
                                }
                            }
                            //native float32 types. Float 128 bit is not supported.
                            switch ((par.GetTypeDescriptor()).numberOfBits) {
                                case 32u: {
                                    float32 *data = static_cast<float32 *>(dataPointer);
                                    ret = FloatToStream(iobuff, *data, fd);
                                }
                                break;
                                case 64u: {
                                    float64 *data = static_cast<float64 *>(dataPointer);
                                    ret = FloatToStream(iobuff, *data, fd);
                                }
                                break;
                                case 128u: {
                                    REPORT_ERROR(ErrorManagement::UnsupportedFeature, "IOBuffer: Unsupported 128 bit floats");
                                    ret = false;
                                }
                                break;
                                default: {
                                    //REPORT_ERROR(ParametersError,"non standard float32 size")
                                    ret = false;
                                }
                            }
                        }
                    }

                    //pointer type.
                    if (((par.GetTypeDescriptor()).type) == Pointer) {
                        if (fd.desiredAction == PrintInfo) {
                            const char8* infoName = "Pointer";
                            AnyType info = infoName;
                            FormatDescriptor newFD = fd;
                            newFD.desiredAction = PrintString;
                            ret = PrintToStreamScalar(iobuff, info, newFD);
                        }
                        else {
                            if (fd.desiredAction != PrintAnything) {
                                if (fd.desiredAction != PrintPointer) {
                                    REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a pointer will be printed");
                                }
                            }
                            TypeDescriptor newTypeDes(par.GetTypeDescriptor().isConstant, UnsignedInteger, par.GetTypeDescriptor().numberOfBits);
                            AnyType at(newTypeDes, par.GetBitAddress(), static_cast<void *>(&dataPointer));
                            FormatDescriptor newFD = fd;
                            newFD.binaryNotation = HexNotation;
                            newFD.desiredAction = PrintInteger;
                            ret = PrintToStreamScalar(iobuff, at, newFD);
                        }
                    }
                    //const char8* string type.
                    //if in the format descriptor is specified the hex notation (%p or %x)
                    //print the value of the pointer.
                    if (((par.GetTypeDescriptor()).type) == BT_CCString) {
                        if (fd.desiredAction == PrintInfo) {
                            const char8* infoName = "Char String";
                            AnyType info = infoName;
                            FormatDescriptor newFD = fd;
                            newFD.desiredAction = PrintString;
                            ret = PrintToStreamScalar(iobuff, info, newFD);
                        }
                        else {
                            if (fd.binaryNotation == HexNotation) {

                                TypeDescriptor newTypeDes(par.GetTypeDescriptor().isConstant, UnsignedInteger, par.GetTypeDescriptor().numberOfBits);
                                AnyType at(newTypeDes, par.GetBitAddress(), static_cast<void *>(&dataPointer));
                                ret = PrintToStreamScalar(iobuff, at, fd);
                            }
                            else {
                                if (fd.desiredAction != PrintAnything) {
                                    if (fd.desiredAction != PrintString) {
                                        REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a string will be printed");
                                    }
                                }
                                const char8 *string = static_cast<const char8 *>(dataPointer);
                                bool addQuotesOnString = fd.fullNotation;
                                ret = PrintCCString(iobuff, string, fd, addQuotesOnString);
                            }
                        }
                    }

                    if (((par.GetTypeDescriptor()).type) == CArray) {
                        if (fd.desiredAction == PrintInfo) {
                            const char8* infoName = "Char Array";
                            AnyType info = infoName;
                            FormatDescriptor newFD = fd;
                            newFD.desiredAction = PrintString;
                            ret = PrintToStreamScalar(iobuff, info, newFD);
                        }
                        else {
                            if (fd.desiredAction != PrintAnything) {
                                if (fd.desiredAction != PrintString) {
                                    REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a string will be printed");
                                }
                            }
                            const char8 *string = static_cast<const char8 *>(dataPointer);
                            bool addQuotesOnString = fd.fullNotation;
                            ret = PrintCCString(iobuff, string, fd, addQuotesOnString);
                        }
                    }
                    //general stream type.
                    bool isStream=(((par.GetTypeDescriptor()).type) == Stream);
                    bool isSString=(((par.GetTypeDescriptor()).type) == SString);
                    if ((isStream) || (isSString)) {
                        if (fd.desiredAction == PrintInfo) {
                            const char8* infoName = "Stream";
                            AnyType info = infoName;
                            FormatDescriptor newFD = fd;
                            newFD.desiredAction = PrintString;
                            ret = PrintToStreamScalar(iobuff, info, newFD);
                        }
                        else {
                            if (fd.desiredAction != PrintAnything) {
                                if (fd.desiredAction != PrintString) {
                                    REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a stream will be printed");
                                }
                            }
                            StreamI * stream = static_cast<StreamI *>(dataPointer);
                            bool addQuotesOnString = fd.fullNotation;
                            ret = PrintStream(iobuff, *stream, fd, addQuotesOnString);
                        }
                    }

                    //StructuredDataInterface.
                    if (((par.GetTypeDescriptor()).type) == StructuredDataNode) {
                        StructuredDataI * structuredData = static_cast<StructuredDataI *>(dataPointer);
                        ret = PrintStructuredDataInterface(iobuff, structuredData);
                    }
                }
            }
            else {
                ret = false;
            }


        }
    }




    //REPORT_ERROR(UnsupportedError,"unsupported format")
    return ret;


}


} //MARTe
