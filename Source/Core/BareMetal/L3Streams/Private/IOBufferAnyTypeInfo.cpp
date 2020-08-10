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

#define DLL_API

#include "IOBuffer.h"
#include "IOBufferPrivate.h"
#include "AnyType.h"
#include "FormatDescriptor.h"
#include "DynamicCString.h"
#include "CompositeErrorManagement.h"
#include "PrintFormatter.h"
#include "ClassMember.h"



namespace MARTe{

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief Prints the introspection related to the object in input.
 * @param[out] iobuff is the output stream buffer.
 * @param[in] parIn is the input.
 * @return true if the object is registered and introspectable, false otherwise.
 * @pre
 *   The object represented by \a parIn must be introspectable
 */
static ErrorManagement::ErrorType PrintObjectIntrospection(IOBuffer &iobuff, FormatDescriptor fd, TypeDescriptor &td, const AnyType & parIn);


/*---------------------------------------------------------------------------*/
/*                           Static implementations                          */
/*---------------------------------------------------------------------------*/


ErrorManagement::ErrorType PrintObjectIntrospection(IOBuffer &iobuff, FormatDescriptor fd, TypeDescriptor &td, const AnyType & parIn) {
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

    if (ret){
        ret = PrintFormatter::OpenBlock(fd.desiredGrammar,iobuff);

        // write class name assignment
        ret = ret & PrintFormatter::OpenAssignMent(fd.desiredGrammar,"class",iobuff);
        ret.OSError = ret.OSError || !PrintCCStringFit(iobuff,item->GetClassName(),fd);
//        error = error && PrintFormatter::StringField(fd.desiredGrammar,item->GetClassName(),iobuff);
        ret = ret & PrintFormatter::CloseAssignMent(fd.desiredGrammar,"class",iobuff);

    // write members start assignment
        ret = ret & PrintFormatter::OpenAssignMent(fd.desiredGrammar,"members",iobuff);
        ret = ret & PrintFormatter::OpenBlock(fd.desiredGrammar,iobuff);
        REPORT_ERROR(ret,"Error writing to iobuff ");
    }

    for (uint32 i = 0 ; (i < nOfMembers)&& ret; i++){
        const ClassMember * cm = item->FindMember(i);

        ret.internalSetupError = (cm == NULL);
        COMPOSITE_REPORT_ERROR(ret,"Class ",item->GetClassName()," member ", i, "is NULL? " );

        AnyType at = parIn;
        if (ret){
            ret = at.Dereference(cm->GetName());
        }

        if (ret){
            ret = PrintFormatter::OpenAssignMent(fd.desiredGrammar,cm->GetName(),iobuff);

            // unless a recursive description is requested
            // just show the type of the members
            FormatDescriptor fd2 = fd;
            if (fd.desiredAction != PrintInfoRecursive){
                fd2.desiredAction = PrintTypeInformation;
            }

            if (ret){
                ret.OSError = !IOBuffer::PrintAnyTypeInfo(iobuff,fd,at);

                ret = ret & PrintFormatter::CloseAssignMent(fd.desiredGrammar,cm->GetName(),iobuff);

                REPORT_ERROR(ret,"Failed writing classname");
            }
        }
    }

    // write members end assignment
    if (ret){
        ret = PrintFormatter::CloseBlock(fd.desiredGrammar,iobuff);
        ret = ret & PrintFormatter::CloseAssignMent(fd.desiredGrammar,"members",iobuff);
    }


    // write end of block
    if (ret){
        ret = PrintFormatter::CloseBlock(fd.desiredGrammar,iobuff);
    }

    return ret;

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



DLL_API bool IOBuffer::PrintAnyTypeInfo(IOBuffer &iobuff, FormatDescriptor fd, const AnyType & parIn){
    ErrorManagement::ErrorType ret;

    // extract variable descriptor
    const VariableDescriptor &vd = parIn.GetFullVariableDescriptor();
    TypeDescriptor td;
    // extract typedescriptor
    td = vd.GetSummaryTypeDescriptor();

    if (td.IsStructuredData() && ((fd.desiredAction == PrintInfo)||(fd.desiredAction == PrintInfoRecursive))){
        ret = PrintObjectIntrospection(iobuff,  fd, td,parIn);
    } else {
        DynamicCString line;
        ret = vd.ToString(line);

        if (ret){
            CCString lineC = line.GetList();
            ret.fatalError = PrintCCStringFit(iobuff,lineC,fd);
//            ret.fatalError = PrintFormatter::StringField(fd.desiredGrammar,line,iobuff);

        }
    }
    return ret;
}


} //MARTe
