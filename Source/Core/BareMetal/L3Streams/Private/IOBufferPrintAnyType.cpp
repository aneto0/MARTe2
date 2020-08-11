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
#include "PrintFormatter.h"
#include "ClassRegistryDatabase.h"
#include "CompositeErrorManagement.h"
#include "ClassMember.h"



namespace MARTe{

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief Prints a generic scalar AnyType object on a buffer.
 * @param[out] iobuff is the stream buffer output.
 * @param[in] parIn is the generic object to be printed.
 * @param[in] fd specifies the desired printing format.
 */
static ErrorManagement::ErrorType PrintAnySimpleType(IOBuffer &iobuff, FormatDescriptor fd, const AnyType & parIn);

/**
 * @brief Prints an object. Assumes that the parIn has a valid pointer and is a structure
 * @param[out] iobuff is the output stream buffer.
 * @param[in] is the input.
 * @return false if the object is not introspectable, true otherwise.
 * @pre
 *   The object represented by parIn must be introspectable.
 */
static ErrorManagement::ErrorType PrintObject(IOBuffer & iobuff, FormatDescriptor fd,TypeDescriptor &td, const AnyType & parIn);

/*---------------------------------------------------------------------------*/
/*                           Static implementations                          */
/*---------------------------------------------------------------------------*/

ErrorManagement::ErrorType PrintObject(IOBuffer &iobuff, FormatDescriptor fd, TypeDescriptor &td, const AnyType & parIn) {
    ErrorManagement::ErrorType ret;


    const ClassRegistryItem *item = ClassRegistryDatabase::Find(td);

    ret.unsupportedFeature = (item == NULL);
    COMPOSITE_REPORT_ERROR(ret,"Class id=",td.GetStructuredDataIdCode()," is not registered");

    uint32 nOfMembers = 0;
    if (ret) {
        nOfMembers = item->NumberOfMembers();
        ret.unsupportedFeature = (nOfMembers == 0);
        COMPOSITE_REPORT_ERROR(ret,"Class ",item->GetClassName()," has 0 members");
    }

    // write start of block
    if (ret){
        ret = PrintFormatter::OpenBlock(fd.desiredGrammar,iobuff);
    }

    // write class name
    if (ret){
        ret = PrintFormatter::OpenAssignMent(fd.desiredGrammar,"Class",iobuff);
        ret.OSError = ret.OSError || !iobuff.PutS(item->GetClassName().GetList());
        ret = ret & PrintFormatter::CloseAssignMent(fd.desiredGrammar,"Class",iobuff);
        REPORT_ERROR(ret,"Failed writing classname");
    }

    for (uint32 i = 0 ; (i < nOfMembers)&& ret; i++){
        const ClassMember * cm = item->FindMember(i);

        ret.internalSetupError = (cm == NULL);
        COMPOSITE_REPORT_ERROR(ret,"Class ",item->GetClassName()," member ", i, "is  NULL? " );

        // empty name means inheritance
        if (ret && (cm->GetName().GetSize() == 0)){
            const uint8* ptr = static_cast<const uint8*>(parIn.GetVariablePointer());
            ptr += cm->GetOffset();
            AnyType at(cm->GetDescriptor(),ptr += cm->GetOffset());

            ret.fatalError = !IOBuffer::PrintAnyType(iobuff,fd,at);
            REPORT_ERROR(ret,"PrintAnyType Failed");
        } else {
            AnyType at = parIn;
            if (ret){
                ret = at.Dereference(cm->GetName());
                COMPOSITE_REPORT_ERROR(ret,"Failed dereferencing element ",i," name = (",cm->GetName(),") " );
            }

            if (ret){
                ret = PrintFormatter::OpenAssignMent(fd.desiredGrammar,cm->GetName(),iobuff);
            }

            if (ret){
                ret.fatalError = !IOBuffer::PrintAnyType(iobuff,fd,at);
                REPORT_ERROR(ret,"PrintAnyType Failed");
            }

            if (ret){
                ret = PrintFormatter::CloseAssignMent(fd.desiredGrammar,cm->GetName(),iobuff);
            }

        }

    }

    // write start of block
    if (ret){
        ret = PrintFormatter::CloseBlock(fd.desiredGrammar,iobuff);
    }

    return ret;

}

ErrorManagement::ErrorType PrintAnySimpleType(IOBuffer &iobuff, FormatDescriptor fd, const AnyType & parIn){
    ErrorManagement::ErrorType ret;
//    bool ret = true;
    // void anytype
    AnyType par = parIn;

    if (fd.desiredAction == PrintTypeInformation) {
        ret.fatalError = !IOBuffer::PrintAnyTypeInfo(iobuff, fd, parIn);
        REPORT_ERROR(ret,"PrintAnyTypeInfo  failed");
    } else
    if ((fd.desiredAction == PrintInfo)|| (fd.desiredAction == PrintInfoRecursive)) {
        ret.fatalError = !IOBuffer::PrintAnyTypeInfo(iobuff, fd, parIn);
        REPORT_ERROR(ret,"PrintAnyTypeInfo  failed");
    } else {
        // extract variable descriptor
        const VariableDescriptor &vd = parIn.GetFullVariableDescriptor();
        const void* dataPointer = NULL;
        TypeDescriptor td;
        // extract typedescriptor
        td = vd.GetSummaryTypeDescriptor();

        if (ret){
            dataPointer = parIn.GetVariablePointer();
            ret.fatalError = (dataPointer == NULL);
            REPORT_ERROR(ret,"dataPointer == NULL");
        }

        if (ret) {  // main block
            // if cannot deal with type just print "(actual type)?"
            bool printTypeInformation = false;
            //if the element is structured, the print is not supported.
            if (td.IsStructuredData()){
                ret = PrintObject(iobuff, fd, td, parIn);
                REPORT_ERROR(ret,"PrintObject failed");
            } else
            if (td.IsSpecialType())  //Stream,StructuredData,..
            {
                printTypeInformation = true;
            } // not a structured data
            else
            {
                TD_FullType fullType = td.GetFullTypeCode();
                uint32 storageSize = td.StorageSize();

                switch (fullType){
                case TDF_UnsignedInteger:{
                    if (td.IsBitType()){  // uint5 bitranges
                        uint8 numberOfBits_ = static_cast<uint8>(td.GetNumericBitSize());
                        uint8 bitOffset_    = static_cast<uint8>(td.GetNumericBitOffset());

                        // use native standard integer
                        const uint32 *number = static_cast<const uint32 *>(dataPointer);
                        ret.fatalError = !BitSetToStream(iobuff, number, bitOffset_, numberOfBits_, false, fd);
                        REPORT_ERROR(ret,"BitSetToStream failed");
                    } else {
                        switch (storageSize) {
                        case 1: {
                            const uint8 *data = static_cast<const uint8 *>(dataPointer);
                            ret.fatalError = !IntegerToStream(iobuff, *data, fd);
                            REPORT_ERROR(ret,"IntegerToStream failed");
                       }
                        break;
                        case 2: {
                            const uint16 *data = static_cast<const uint16 *>(dataPointer);
                            ret.fatalError = !IntegerToStream(iobuff, *data, fd);
                            REPORT_ERROR(ret,"IntegerToStream failed");
                        }
                        break;
                        case 4: {
                            const uint32 *data = static_cast<const uint32 *>(dataPointer);
                            ret.fatalError = !IntegerToStream(iobuff, *data, fd);
                            REPORT_ERROR(ret,"IntegerToStream failed");
                        }
                        break;
                        case 8: {
                            const uint64 *data = static_cast<const uint64 *>(dataPointer);
                            ret.fatalError = !IntegerToStream(iobuff, *data, fd);
                            REPORT_ERROR(ret,"IntegerToStream failed");
                        }
                        break;
                        default: {
                            printTypeInformation = true;
                        }
                        }  // end case
                    }
                }break;
                case TDF_SignedInteger:{
                    if (td.IsBitType()){  // uint5 bitranges
                        uint8 numberOfBits_ = static_cast<uint8>(td.GetNumericBitSize());
                        uint8 bitOffset_    = static_cast<uint8>(td.GetNumericBitOffset());

                        // use native standard integer
                        const uint32 *number = static_cast<const uint32 *>(dataPointer);
                        ret.fatalError = !BitSetToStream(iobuff, number, bitOffset_, numberOfBits_, true, fd);
                        REPORT_ERROR(ret,"BitSetToStream failed");
                    } else {
                        switch (storageSize) {
                        case 1: {
                            const int8 *data = static_cast<const int8 *>(dataPointer);
                            ret.fatalError = !IntegerToStream(iobuff, *data, fd);
                            REPORT_ERROR(ret,"IntegerToStream failed");
                        }
                        break;
                        case 2: {
                            const int16 *data = static_cast<const int16 *>(dataPointer);
                            ret.fatalError = !IntegerToStream(iobuff, *data, fd);
                            REPORT_ERROR(ret,"IntegerToStream failed");
                        }
                        break;
                        case 4: {
                            const int32 *data = static_cast<const int32 *>(dataPointer);
                            ret.fatalError = !IntegerToStream(iobuff, *data, fd);
                            REPORT_ERROR(ret,"IntegerToStream failed");
                        }
                        break;
                        case 8: {
                            const int64 *data = static_cast<const int64 *>(dataPointer);
                            ret.fatalError = !IntegerToStream(iobuff, *data, fd);
                            REPORT_ERROR(ret,"IntegerToStream failed");
                        }
                        break;
                        default: {
                            printTypeInformation = true;
                        }
                        }
                    }
                }break;

                case TDF_Float:{
                    switch (storageSize){
                    case 4: {
                        const float32 *data = static_cast<const float32 *>(dataPointer);
                        ret.fatalError = !FloatToStream(iobuff, *data, fd);
                        REPORT_ERROR(ret,"FloatToStream failed");
                    }
                    break;
                    case 8: {
                        const float64 *data = static_cast<const float64 *>(dataPointer);
                        ret.fatalError = !FloatToStream(iobuff, *data, fd);
                        REPORT_ERROR(ret,"FloatToStream failed");
                    }
                    break;
                    default:{
                        printTypeInformation = true;
                    }
                    }// end switch

                }break;

                case TDF_Char:{
                    switch (storageSize){
                    case 1: {
                        const char8 *data = static_cast<const char8 *>(dataPointer);
//                        ret = PrintFormatter::CharField(fd.desiredGrammar,*data,iobuff);
                        //iobuff.PutC(*data);
                        char8 miniString[2] = {*data,'\0' };
                        ret.fatalError = !PrintCCStringFit(iobuff,&miniString[0],fd);
                        REPORT_ERROR(ret,"PrintCCStringFit failed");
                    }
                    break;
                    default:{
                        printTypeInformation = true;
                    }
                    }// end switch
                }break;
                case TDF_DynamicCString:
                case TDF_CString:
                case TDF_CCString:{
                    const CCString *data = static_cast<const CCString *>(dataPointer);
//                    ret = PrintFormatter::StringField(fd.desiredGrammar,*data,iobuff);
                    ret.fatalError = !PrintCCStringFit(iobuff,*data,fd);
                    REPORT_ERROR(ret,"PrintCCStringFit failed");
                }break;
                case TDF_Pointer:{
                    const uintp *data = static_cast<const uintp *>(dataPointer);
                    ret.fatalError = !IntegerToStream(iobuff, *data, fd);
                    REPORT_ERROR(ret,"IntegerToStream failed");
                }break;
                default:{
                    printTypeInformation = true;
                }
                }  // end case
            }
            // print this as default
            if (printTypeInformation){
                ret.fatalError = !IOBuffer::PrintAnyTypeInfo(iobuff, fd, parIn);
                REPORT_ERROR(ret,"PrintAnyTypeInfo  failed");
            }
        } // main block

    }  // desired action
    //REPORT_ERROR(UnsupportedError,"unsupported format")
    return ret;

}


/*---------------------------------------------------------------------------*/
/*                           Member implementations                          */
/*---------------------------------------------------------------------------*/



DLL_API bool IOBuffer::PrintAnyType(IOBuffer &iobuff, FormatDescriptor fd, const AnyType & parIn){
    ErrorManagement::ErrorType ret;

    TypeDescriptor td;
    uint32 nDims = 3;
    uint32 dims[3];
    ret = parIn.GetVariableInformation(td,nDims,dims );
    REPORT_ERROR(ret,"GetVariableInformation failed");

    if (ret && (nDims == 0) ){
        ret = PrintAnySimpleType(iobuff, fd, parIn);
        REPORT_ERROR(ret,"PrintAnySimpleType failed");
    } else
    if (ret ){

        // open block
        if (nDims > 2){
            ret = PrintFormatter::OpenBlock(fd.desiredGrammar,iobuff);
            // structure field N =
        } else {
            ret = PrintFormatter::OpenArray(fd.desiredGrammar,iobuff);
        }

        if (ret && (dims[0] == 0)){
            ret = PrintCCStringFit(iobuff,"null",fd);
        }

        for (uint32 i = 0;ret && (i < dims[0]); i++){
            AnyType at = parIn;
            ret = at.Dereference(i);
            REPORT_ERROR(ret,"Dereference failed");

            if ((i > 0) && ret){
                // separator
                ret = PrintFormatter::Separator(fd.desiredGrammar,i,iobuff);
            }

            DynamicCString ns;
            if ((nDims > 2) && ret){
                ns().Append(i);
                // structure field N =
                ret = PrintFormatter::OpenAssignMent(fd.desiredGrammar,ns,iobuff);
            }

            if (ret && (nDims >= 1)){
                ret.OSError = !PrintAnyType(iobuff, fd, at);
                REPORT_ERROR(ret,"PrintAnyType  failed");
            } else
            if (ret){
                ret = PrintAnySimpleType(iobuff, fd, at);
                REPORT_ERROR(ret,"PrintAnySimpleType failed");
            }

            if (ret && (nDims > 2)){
                // structure field N =
                ret = PrintFormatter::CloseAssignMent(fd.desiredGrammar,ns,iobuff);
            }
        }

        if (ret){
            // close block
            if (nDims > 2){
                ret = PrintFormatter::CloseBlock(fd.desiredGrammar,iobuff);
                // structure field N =
            } else {
                ret = PrintFormatter::CloseArray(fd.desiredGrammar,iobuff);
            }
        }
    }

    return ret;
}


} //MARTe
